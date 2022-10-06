#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class Climber : public GameEngineActor
{
public:
	// constrcuter destructer
	Climber();
	~Climber();

	// delete Function
	Climber(const Climber& _Other) = delete;
	Climber(Climber&& _Other) noexcept = delete;
	Climber& operator=(const Climber& _Other) = delete;
	Climber& operator=(Climber&& _Other) noexcept = delete;


protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

	void Gravity();
	bool MapPixelCheck();
	CollisionReturn CheckTrigger(GameEngineCollision* _This, GameEngineCollision* _Other);
	CollisionReturn CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other);

	void TrunStart(const StateInfo& _Info);
	void TrunUpdate(float _DeltaTime, const StateInfo& _Info);

	void MoveStart(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);

	MonsterDIR CurDir;
	float Speed;
	float StopTime;
	int Health;
	bool OnGround;
	bool IsDeath_;
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

