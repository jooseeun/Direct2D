#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"
// Ό³Έν :
class GameEngineUIRenderer;
class GameEngineCollision;
class Mouse : public GameEngineActor
{
public:
	static Mouse* MainMouse;
public:
	// constrcuter destructer
	Mouse();
	~Mouse();

	// delete Function
	Mouse(const Mouse& _Other) = delete;
	Mouse(Mouse&& _Other) noexcept = delete;
	Mouse& operator=(const Mouse& _Other) = delete;
	Mouse& operator=(Mouse&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:

	float4 GetMouseWorldRePosition(float4 ReSize);
	void CheckClickCollision();

	bool CheckStart(GameEngineCollision* _This, GameEngineCollision* _Other);
	bool CheckEnd(GameEngineCollision* _This, GameEngineCollision* _Other);
	//bool CheckSet(GameEngineCollision* _This, GameEngineCollision* _Other);

	GameEngineUIRenderer* MouseRenderer;
	GameEngineCollision* MouseCol;
};

