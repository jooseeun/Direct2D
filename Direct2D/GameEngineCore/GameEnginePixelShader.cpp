#include "PreCompile.h"
#include "GameEnginePixelShader.h"

GameEnginePixelShader::GameEnginePixelShader() 
	: ShaderPtr(nullptr)
{
	ShaderSettingType = ShaderType::Pixel;
}

GameEnginePixelShader::~GameEnginePixelShader() 
{
	if (nullptr != ShaderPtr)
	{
		ShaderPtr->Release();
		ShaderPtr = nullptr;
	}
}


// Color.hlsl
// ���� ���̴����ϳ���
// ���ؽ����̴��� 2�� �ۼ����մϴ�.

// Color_VS

GameEnginePixelShader* GameEnginePixelShader::Load(
	const std::string& _Path,
	const std::string& _EntryPoint,
	UINT _VersionHigh /*= 5*/, UINT _VersionLow /*= 0*/)
{
	return Load(_Path, GameEnginePath::GetFileName(_Path), _EntryPoint, _VersionHigh, _VersionLow);
}


GameEnginePixelShader* GameEnginePixelShader::Load(
	const std::string& _Path,
	const std::string& _Name,
	const std::string& _EntryPoint,
	UINT _VersionHigh = 5, UINT _VersionLow = 0)
{
	GameEnginePixelShader* NewRes = CreateResName(_Name);
	NewRes->ShaderCompile(_Path, _EntryPoint, _VersionHigh, _VersionLow);
	return NewRes;
}


void GameEnginePixelShader::ShaderCompile(
	const std::string& _Path,
	const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	CreateVersion("ps", _VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	// ���̴������� �⺻������ ����� �Ʒ��� ���� ��ġ�� �Ǿ ���� �Ǿ��ִµ�
	// 1000
	// 0100
	// 2010
	// 0301

	// 1020
	// 0103
	// 0010
	// 0001


	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	ID3DBlob* Error;

	std::wstring UnicodePath = GameEngineString::AnsiToUnicodeReturn(_Path);

	// ���̴� 
	if (S_OK != D3DCompileFromFile(
		UnicodePath.c_str(), // ���� ���
		nullptr,  // ��ũ�� ()
		D3D_COMPILE_STANDARD_FILE_INCLUDE,  // ��� ()
		_EntryPoint.c_str(), // ������ COLOR_VS(
		Version.c_str(),  // vs_5_0
		Flag,
		0,
		&BinaryPtr,
		&Error)
		)
	{
		std::string ErrorText = reinterpret_cast<char*>(Error->GetBufferPointer());
		MsgBoxAssertString(ErrorText);
		return;
	}

	// �̹� �� �����ϵ� ���̴� �ڵ��� ���̳ʸ��� �־��༭ �����ϴ� ����� �٤��ϴ�.
	if (S_OK != GameEngineDevice::GetDevice()->CreatePixelShader(
		BinaryPtr->GetBufferPointer(),
		BinaryPtr->GetBufferSize(),
		nullptr,
		&ShaderPtr))
	{
		MsgBoxAssert("�ȼ� ���̴� �ڵ� ������ �����߽��ϴ�.");
	}

	ShaderResCheck();
}


void GameEnginePixelShader::Setting()
{
	if (nullptr == ShaderPtr)
	{
		MsgBoxAssert("���̴� ���� ����");
	}

	// �ι�° ���ڴ� #include�� #define�� hlsl���� ����� ����� �������� ��ü�� �־��ټ� �ִ�.
	GameEngineDevice::GetContext()->PSSetShader(ShaderPtr, nullptr, 0);
}