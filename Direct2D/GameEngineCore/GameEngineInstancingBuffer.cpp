#include "PreCompile.h"
#include "GameEngineInstancingBuffer.h"
#include "GameEngineDevice.h"

GameEngineInstancingBuffer::GameEngineInstancingBuffer() 
{
}

GameEngineInstancingBuffer::~GameEngineInstancingBuffer() 
{
	BufferRelease();
}

void GameEngineInstancingBuffer::BufferRelease()
{
	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}
}


GameEngineInstancingBuffer* GameEngineInstancingBuffer::Create(int _Count, int _Size)
{
	GameEngineInstancingBuffer* Buffer = CreateResUnName();

	Buffer->BufferCreate(_Count, _Size);

	return Buffer;
}

void GameEngineInstancingBuffer::BufferCreate(int _Count, int Size) 
{
	BufferRelease();

	BufferCount = _Count;

	BufferDesc = {};
	BufferDesc.ByteWidth = _Count * Size;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferDesc, nullptr, &Buffer))
	{
		MsgBoxAssert("버텍스 버퍼 생성에 실패했습니다.");
	}
}