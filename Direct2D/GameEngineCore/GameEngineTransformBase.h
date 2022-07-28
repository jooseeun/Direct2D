#pragma once
#include <GameEngineBase/GameEngineTransform.h>

// ���� :
class GameEngineTransformBase 
{
public:
	// constrcuter destructer
	GameEngineTransformBase();
	virtual ~GameEngineTransformBase();

	// delete Function
	GameEngineTransformBase(const GameEngineTransformBase& _Other) = delete;
	GameEngineTransformBase(GameEngineTransformBase&& _Other) noexcept = delete;
	GameEngineTransformBase& operator=(const GameEngineTransformBase& _Other) = delete;
	GameEngineTransformBase& operator=(GameEngineTransformBase&& _Other) noexcept = delete;

	/////////////////////////////////////////////////// ���ϰ���
private:
	GameEngineTransform Transform;

public:
	GameEngineTransform& GetTransform()
	{
		return Transform;
	}

	const TransformData& GetTransformData()
	{
		return Transform.GetTransformData();
	}
};

