#include "PreCompile.h"
#include "GameEngineShader.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"

void GameEngineConstantBufferSetter::Setting() const
{
	Res->ChangeData(SetData, Size);
	SettingFunction();
}

void GameEngineTextureSetter::Setting() const
{
	SettingFunction();
}

void GameEngineSamplerSetter::Setting() const
{
	SettingFunction();
}

void GameEngineShader::AutoCompile(const std::string& _Path)
{
	GameEngineFile File = GameEngineFile(_Path);
	File.Open(OpenMode::Read, FileMode::Text);

	std::string AllHlslCode = File.GetString();

	size_t VSEntryIndex = AllHlslCode.find("_VS(");
	// 0123456789
	//"a Color_VS("
	// VSEntryIndex == 7
	if (std::string::npos != VSEntryIndex)
	{
		// size_t VSEntryIndex = AllHlslCode.find("_VS(");
		// 7���� ã�ƶ� ��������
		// 1
		size_t FirstIndex = AllHlslCode.find_last_of(" ", VSEntryIndex);
		// "01234567"
		// substr(2, 3); "234"

		// ' 'Color_VS 
		std::string EntryName = AllHlslCode.substr(FirstIndex + 1, VSEntryIndex - FirstIndex - 1);
		EntryName += "_VS";
		GameEngineVertexShader::Load(_Path, EntryName);
	}

	size_t PSEntryIndex = AllHlslCode.find("_PS(");
	if (std::string::npos != PSEntryIndex)
	{
		size_t FirstIndex = AllHlslCode.find_last_of(" ", PSEntryIndex);
		std::string EntryName = AllHlslCode.substr(FirstIndex + 1, PSEntryIndex - FirstIndex - 1);
		EntryName += "_PS";
		GameEnginePixelShader::Load(_Path, EntryName);
	}

	// File.GetFileSize();

}

GameEngineShader::GameEngineShader() 
	: Version("")
	, BinaryPtr(nullptr)
{
}

GameEngineShader::~GameEngineShader() 
{
	if (nullptr != BinaryPtr)
	{
		BinaryPtr->Release();
		BinaryPtr = nullptr;
	}
}



void GameEngineShader::CreateVersion(const std::string& _ShaderType, UINT _VersionHigh, UINT _VersionLow)
{
	Version = "";
	Version += _ShaderType + "_"; // vs_
	Version += std::to_string(_VersionHigh); // vs_5
	Version += "_"; // vs_5_
	Version += std::to_string(_VersionLow); // vs_5_0
}

// ���̴����� ������۸� ����ߴ��� �ؽ�ó�� �����
void GameEngineShader::ShaderResCheck()
{
	// BinaryPtr ������ ����� ���̴� ������ 2�� �޸�
	if (nullptr == BinaryPtr)
	{
		MsgBoxAssert("���̴� ���ҽ��� ��������� �ʾҴµ� ���ҽ�(������� and �ؽ�ó) üũ�� �Ϸ��� �߽��ϴ�.");
		return;
	}

	// Reflection 
	// Ŭ������ ���� ������ ����������� �츮���� �������ٶ� �� Ŭ������ �Լ����� �̷� �̸��� ������ �ִ�.
	// �̷� �̸��� �������̽� 
	ID3D11ShaderReflection* CompileInfo = nullptr;

	if (S_OK != D3DReflect(
		BinaryPtr->GetBufferPointer(), 
		BinaryPtr->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		reinterpret_cast<void**>( & CompileInfo)
	))
	{
		MsgBoxAssert("���̴� ���̴� ���÷����� �߸� �ž����ϴ�.");
		return;
	}


	D3D11_SHADER_DESC Info;
	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResInfo;

	// Info.BoundResources �̰� �� ���̴����� ���� �� ���ҽ� ��
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		CompileInfo->GetResourceBindingDesc(i, &ResInfo);
		// ���ҽ��� �����Ѵ�.
		std::string Name = GameEngineString::ToUpperReturn(ResInfo.Name);

		// ResInfo

		D3D_SHADER_INPUT_TYPE Type = ResInfo.Type;

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{

			// ���ҽ��� ������۶��
			ID3D11ShaderReflectionConstantBuffer* CBufferPtr = CompileInfo->GetConstantBufferByName(ResInfo.Name);

			D3D11_SHADER_BUFFER_DESC BufferDesc;
			CBufferPtr->GetDesc(&BufferDesc);

			// 5���� ���õǴ� 
			// ResInfo.BindPoint;

			GameEngineConstantBufferSetter NewSetter;

			// �ߺ����� ��������� ����� �ȵǴϱ�.
			// ����� �� ������ �ϴ°� 
			NewSetter.ParentShader = this;
			NewSetter.SetName(Name);
			NewSetter.ShaderType = ShaderSettingType;
			NewSetter.Res = GameEngineConstantBuffer::CreateAndFind(Name, BufferDesc, CBufferPtr);
			NewSetter.BindPoint = ResInfo.BindPoint;
			ConstantBufferMap.insert(std::make_pair(Name, NewSetter));

			break;
		}
		case D3D_SIT_TEXTURE:
		{
			GameEngineTextureSetter NewSetter;
			NewSetter.ParentShader = this;
			NewSetter.SetName(Name);
			NewSetter.ShaderType = ShaderSettingType;
			NewSetter.Res = GameEngineTexture::Find("NSet.png");
			NewSetter.BindPoint = ResInfo.BindPoint;
			TextureMap.insert(std::make_pair(Name, NewSetter));
			break;
		}
		case D3D_SIT_SAMPLER:
		{
			GameEngineSamplerSetter NewSetter;
			NewSetter.ParentShader = this;
			NewSetter.SetName(Name);
			NewSetter.ShaderType = ShaderSettingType;
			NewSetter.Res = GameEngineSampler::Find("EngineSamplerLinear");
			NewSetter.BindPoint = ResInfo.BindPoint;
			SamplerMap.insert(std::make_pair(Name, NewSetter));
			break;
		}
		default:
			MsgBoxAssert("���� ó������ ���� ���̴� ���ҽ�");
			break;
		}


		// �� ���� ������۰� ��������� �մϴ�.

		int a = 0;

	}

	ConstantBufferMap;
	TextureMap;

	// ������� � ������ ũ��� ������ �̷��͵��� �˾Ƴ��ݴϴ�.
	// CompileInfo

	// CompileInfo->Release();
}

GameEngineConstantBufferSetter& GameEngineShader::GetConstantBufferSetter(std::string _Name)
{
	std::string Name = GameEngineString::ToUpperReturn(_Name);

	if (ConstantBufferMap.end() == ConstantBufferMap.find(Name))
	{
		MsgBoxAssert("�������� �ʴ� ������۸� ã������ �߽��ϴ�.");
	}

	return ConstantBufferMap[Name];
}