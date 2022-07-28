#include "PreCompile.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineVertexs.h"
#include "GameEngineVertexShader.h"

GameEngineInputLayOut::GameEngineInputLayOut() 
	: InputLayOut(nullptr)
{
}

GameEngineInputLayOut::~GameEngineInputLayOut() 
{
	if (nullptr != InputLayOut)
	{
		InputLayOut->Release();
	}
}


void GameEngineInputLayOut::Setting()
{
	if (nullptr == InputLayOut)
	{
		MsgBoxAssert("��ǲ���̾ƿ��� ������ �ֽ��ϴ�.");
	}

	GameEngineDevice::GetContext()->IASetInputLayout(InputLayOut);

	// GameEngineDevice::GetContext()->VSSetConstantBuffers(0, 1, nullptr);

	// GameEngineDevice::GetContext()->VSSetConstantBuffers(5, 1, nullptr);
}

void GameEngineInputLayOut::Create(const GameEngineLayOutDesc& _Desc, GameEngineVertexShader* _Shader)
{
	if (
		S_OK != GameEngineDevice::GetDevice()->CreateInputLayout(
		&_Desc.InputLayOutDesc[0],
		static_cast<UINT>(_Desc.InputLayOutDesc.size()),
		_Shader->BinaryPtr->GetBufferPointer(),
		_Shader->BinaryPtr->GetBufferSize(),
			&InputLayOut)
		)
	{
		MsgBoxAssert("��ǲ���̾ƿ� ������ �����߽��ϴ�");
	}
}
