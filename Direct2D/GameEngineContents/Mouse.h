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
	GameEngineUIRenderer* MouseRenderer;
	GameEngineCollision* MouseCol;
};

