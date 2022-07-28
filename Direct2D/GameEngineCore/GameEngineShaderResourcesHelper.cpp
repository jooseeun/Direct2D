#include "PreCompile.h"
#include "GameEngineShaderResourcesHelper.h"
#include "GameEnginePixelShader.h"
#include "GameEngineVertexShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"

GameEngineShaderResourcesHelper::GameEngineShaderResourcesHelper() 
{
}

GameEngineShaderResourcesHelper::~GameEngineShaderResourcesHelper() 
{
}

void GameEngineShaderResourcesHelper::AllResourcesSetting()
{
	for (const std::pair<std::string, GameEngineConstantBufferSetter>& Setter : ConstantBufferSettingMap)
	{
		Setter.second.Setting();
	}

	for (const std::pair<std::string, GameEngineTextureSetter>& Setter : TextureSettingMap)
	{
		Setter.second.Setting();
	}

	for (const std::pair<std::string, GameEngineSamplerSetter>& Setter : SamplerSettingMap)
	{
		Setter.second.Setting();
	}
}

void GameEngineShaderResourcesHelper::ResourcesCheck(GameEngineRenderingPipeLine* _Line)
{
	ShaderCheck(_Line->GetVertexShader());
	ShaderCheck(_Line->GetPixelShader());

}

void Test() {
	int a = 0;
	// �� �ȵǴ°ž�!!!!
}

void GameEngineShaderResourcesHelper::ShaderCheck(GameEngineShader* _Shader)
{
	// �ȼ����̴��� ���ؽ� ���̴����� transform������ ���� �߿� ��������� �̸��� �Ȱ��� �ؼ� ����ϰ� �ʹٸ�??????
	for (const std::pair<std::string, GameEngineConstantBufferSetter>& Data : _Shader->ConstantBufferMap)
	{
		std::multimap<std::string, GameEngineConstantBufferSetter>::iterator InsertIter = 
			ConstantBufferSettingMap.insert(std::make_pair(Data.first, Data.second));

		BindConstantBuffer(InsertIter->second, Data.second.Res);
	}

	for (const std::pair<std::string, GameEngineTextureSetter>& Data : _Shader->TextureMap)
	{
		std::multimap<std::string, GameEngineTextureSetter>::iterator InsertIter =
		TextureSettingMap.insert(std::make_pair(Data.first, Data.second));

		BindTexture(InsertIter->second, Data.second.Res);

	}


	for (const std::pair<std::string, GameEngineSamplerSetter>& Data : _Shader->SamplerMap)
	{
		std::multimap<std::string, GameEngineSamplerSetter>::iterator InsertIter =
			SamplerSettingMap.insert(std::make_pair(Data.first, Data.second));

		BindSampler(InsertIter->second, Data.second.Res);
	}

	
}

bool GameEngineShaderResourcesHelper::IsConstantBuffer(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	if (ConstantBufferSettingMap.end() != ConstantBufferSettingMap.find(Key))
	{
		return true;
	}

	return false;
}

void GameEngineShaderResourcesHelper::SetConstantBufferNew(const std::string& _Name, const void* _Data, UINT _Size)
{
	if (false == IsConstantBuffer(_Name))
	{
		MsgBoxAssertString(_Name + "���̴����� �̷��� �̸��� ������۸� ����� ���� �����ϴ�.");
		return;
	}

	if (16 > _Size)
	{

		MsgBox("�ּ��� 16����Ʈ �̻��� ������ ����� �մϴ�.");
		return;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameStartIter
		= ConstantBufferSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameEndIter
		= ConstantBufferSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		// Ʈ�������� �ٲ��

		if (0 == NameStartIter->second.OriginalData.size()
			|| NameStartIter->second.OriginalData.size() != _Size)
		{
			NameStartIter->second.OriginalData.resize(_Size);
		}

		NameStartIter->second.SetData = &NameStartIter->second.OriginalData[0];
		memcpy_s(&NameStartIter->second.OriginalData[0], _Size, _Data, _Size);

		NameStartIter->second.Size = _Size;
	}
}

void GameEngineShaderResourcesHelper::SetConstantBufferLink(
	const std::string& _Name, 
	const void* _Data, 
	UINT _Size)
{
	if (false == IsConstantBuffer(_Name))
	{
		MsgBoxAssertString(_Name + "���̴����� �̷��� �̸��� ������۸� ����� ���� �����ϴ�.");
		return;
	}

	if (16 > _Size)
	{

		MsgBox("�ּ��� 16����Ʈ �̻��� ������ ����� �մϴ�.");
		return;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameStartIter
		= ConstantBufferSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameEndIter
		= ConstantBufferSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter ; ++NameStartIter)
	{
		// Ʈ�������� �ٲ��
		NameStartIter->second.SetData = _Data;
		NameStartIter->second.Size = _Size;
	}

}

bool GameEngineShaderResourcesHelper::IsTexture(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	if (TextureSettingMap.end() != TextureSettingMap.find(Key))
	{
		return true;
	}

	return false;
}


GameEngineTexture* GameEngineShaderResourcesHelper::SetTexture(const std::string& _Name, const std::string& _TextureName)
{
	if (false == IsTexture(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	return SetTexture(_Name, GameEngineTexture::Find(_TextureName));
}

GameEngineTexture* GameEngineShaderResourcesHelper::SetTexture(const std::string& _Name, GameEngineTexture* _Texture)
{
	std::string Name = GameEngineString::ToUpperReturn(_Name);

	if (false == IsTexture(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::multimap<std::string, GameEngineTextureSetter>::iterator NameStartIter
		= TextureSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineTextureSetter>::iterator NameEndIter
		= TextureSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		BindTexture(NameStartIter->second, _Texture);
	}

	return _Texture;
}


bool GameEngineShaderResourcesHelper::IsSampler(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	if (SamplerSettingMap.end() != SamplerSettingMap.find(Key))
	{
		return true;
	}

	return false;
}


GameEngineSampler* GameEngineShaderResourcesHelper::SetSampler(const std::string& _Name, const std::string& _TextureName)
{
	if (false == IsTexture(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	return SetSampler(_Name, GameEngineSampler::Find(_TextureName));

}

GameEngineSampler* GameEngineShaderResourcesHelper::SetSampler(const std::string& _Name, GameEngineSampler* _Res)
{
	std::string Name = GameEngineString::ToUpperReturn(_Name);

	if (false == IsTexture(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameStartIter
		= SamplerSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameEndIter
		= SamplerSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		BindSampler(NameStartIter->second, _Res);
	}

	return _Res;
}

void GameEngineShaderResourcesHelper::BindTexture(GameEngineTextureSetter& _Setter, GameEngineTexture* _Res)
{
	_Setter.Res = _Res;

	if (nullptr == _Res)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� ����Ϸ��� �߽��ϴ�.");
	}

	switch (_Setter.ShaderType)
	{
	case ShaderType::Vertex:
		_Setter.SettingFunction = std::bind(&GameEngineTexture::VSSetting, _Setter.Res, _Setter.BindPoint);
		break;
	case ShaderType::Pixel:
		_Setter.SettingFunction = std::bind(&GameEngineTexture::PSSetting, _Setter.Res, _Setter.BindPoint);
		break;
	default:
		break;
	}
}

void GameEngineShaderResourcesHelper::BindConstantBuffer(GameEngineConstantBufferSetter& _Setter, GameEngineConstantBuffer* _Res)
{
	_Setter.Res = _Res;

	if (nullptr == _Res)
	{
		MsgBoxAssert("�������� �ʴ� ������۸� ����Ϸ��� �߽��ϴ�.");
	}

	switch (_Setter.ShaderType)
	{
	case ShaderType::Vertex:
		_Setter.SettingFunction = std::bind(&GameEngineConstantBuffer::VSSetting, _Setter.Res, _Setter.BindPoint);
		break;
	case ShaderType::Pixel:
		_Setter.SettingFunction = std::bind(&GameEngineConstantBuffer::PSSetting, _Setter.Res, _Setter.BindPoint);
		break;
	default:
		break;
	}
}

void GameEngineShaderResourcesHelper::BindSampler(GameEngineSamplerSetter& _Setter, GameEngineSampler* _Res)
{
	_Setter.Res = _Res;

	if (nullptr == _Res)
	{
		MsgBoxAssert("�������� �ʴ� ���÷��� ����Ϸ��� �߽��ϴ�.");
	}

	switch (_Setter.ShaderType)
	{
	case ShaderType::Vertex:
		_Setter.SettingFunction = std::bind(&GameEngineSampler::VSSetting, _Setter.Res, _Setter.BindPoint);
		break;
	case ShaderType::Pixel:
		_Setter.SettingFunction = std::bind(&GameEngineSampler::PSSetting, _Setter.Res, _Setter.BindPoint);
		break;
	default:
		break;
	}
}
