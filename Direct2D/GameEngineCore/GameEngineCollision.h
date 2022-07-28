#pragma once
#include <GameEngineBase/GameEngineTransform.h>



// Ό³Έν :
class GameEngineCollision
{
public:
	// static bool SphereToSphere(const GameEngineTransform&);

public:
	// constrcuter destructer
	GameEngineCollision();
	~GameEngineCollision();

	// delete Function
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

protected:

private:

};

