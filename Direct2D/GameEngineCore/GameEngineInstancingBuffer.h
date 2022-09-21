#pragma once
#include "GameEngineRes.h"
#include <vector>
#include <string>
#include <map>
#include <list>
#include "GameEngineVertexs.h"

// Ό³Έν :
class GameEngineInstancingBuffer : public GameEngineRes<GameEngineInstancingBuffer>
{
public:
	// constrcuter destructer
	GameEngineInstancingBuffer();
	~GameEngineInstancingBuffer();

	// delete Function
	GameEngineInstancingBuffer(const GameEngineInstancingBuffer& _Other) = delete;
	GameEngineInstancingBuffer(GameEngineInstancingBuffer&& _Other) noexcept = delete;
	GameEngineInstancingBuffer& operator=(const GameEngineInstancingBuffer& _Other) = delete;
	GameEngineInstancingBuffer& operator=(GameEngineInstancingBuffer&& _Other) noexcept = delete;

	static GameEngineInstancingBuffer* Create(int _Count, int Size);

	int GetBufferCount()
	{
		return BufferCount;
	}

	void BufferCreate(int _Count, int _Size);

protected:

private:
	D3D11_BUFFER_DESC BufferDesc;
	ID3D11Buffer* Buffer;
	int BufferCount;

	void BufferRelease();

};

