#pragma once

#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class BasicZombie : public GameEngineActor
{
public:
	// constrcuter destructer
	BasicZombie();
	~BasicZombie();

	// delete Function
	BasicZombie(const BasicZombie& _Other) = delete;
	BasicZombie(BasicZombie&& _Other) noexcept = delete;
	BasicZombie& operator=(const BasicZombie& _Other) = delete;
	BasicZombie& operator=(BasicZombie&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

	void Gravity();
	bool MapPixelCheck();
	CollisionReturn CheckTrigger(GameEngineCollision* _This, GameEngineCollision* _Other);
	CollisionReturn CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other);

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void MoveStart(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);

	void BackStart(const StateInfo& _Info);
	void BackUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);

	MonsterDIR CurDir;
	float Speed;
	float FallTime;
	float StopTime;
	bool OnGround;
	int Health;
private:
	void EffectUpdate();
	GameEngineTextureRenderer* MonsterRenderer;
	GameEngineTextureRenderer* HPEffect1;
	GameEngineTextureRenderer* HPEffect2;
	GameEngineTextureRenderer* HPEffect3;
	GameEngineCollision* MonsterCollision;
	GameEngineCollision* TriggerCollision;


	GameEngineStateManager StateManager;

};

