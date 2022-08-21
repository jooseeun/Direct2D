#include "PreCompile.h"
#include "GameEngineIndexBuffer.h"
#include <GameEngineBase/GameEngineString.h>

GameEngineIndexBuffer::GameEngineIndexBuffer() 
	: Buffer(nullptr)
	, Offset(0)
{
}

GameEngineIndexBuffer::~GameEngineIndexBuffer() 
{
	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}
}


GameEngineIndexBuffer* GameEngineIndexBuffer::Create(const std::string& _Name, const void* _Data, UINT _IndexSize, UINT _IndexCount)
{
	GameEngineIndexBuffer* NewRes = CreateResName(_Name);
	NewRes->BufferCreate(_Data, _IndexSize, _IndexCount);
	return NewRes;
}

void GameEngineIndexBuffer::BufferCreate(const void* _Data, UINT _IndexSize, UINT _IndexCount)
{
	IndexSize = _IndexSize;

	IndexCount = _IndexCount;

	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = _Data;

	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.ByteWidth = _IndexSize * _IndexCount;
	BufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	BufferDesc.CPUAccessFlags = 0;

	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferDesc, &Data, &Buffer))
	{
		MsgBoxAssert("���ؽ� ���� ������ �����߽��ϴ�.");
	}
}

void GameEngineIndexBuffer::Setting()
{
	if (nullptr == Buffer)
	{
		MsgBoxAssert("���ؽ� ���ۿ� ������ �ֽ��ϴ�.");
	}

	// 0, 1,

	// [0][1][2][2][3][2][]

	// GameEngineDevice::GetContext()->RSSetViewports()

	// ������ �׸��� �ִ� ����� ����������
	GameEngineDevice::GetContext()->IASetIndexBuffer(
		Buffer,
		DXGI_FORMAT::DXGI_FORMAT_R32_UINT,
		Offset);
}