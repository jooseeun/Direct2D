#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class Buzzer : public GameEngineActor
{
public:
	// constrcuter destructer
	Buzzer();
	~Buzzer();

	// delete Function
	Buzzer(const Buzzer& _Other) = delete;
	Buzzer(Buzzer&& _Other) noexcept = delete;
	Buzzer& operator=(const Buzzer& _Other) = delete;
	Buzzer& operator=(Buzzer&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

	bool MapPixelCheck();
	CollisionReturn CheckTrigger(GameEngineCollision* _This, GameEngineCollision* _Other);
	CollisionReturn CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other);

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void MoveStart(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);

	void StartleStart(const StateInfo& _Info);
	void StartleUpdate(float _DeltaTime, const StateInfo& _Info);

	void BackStart(const StateInfo& _Info);
	void BackUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);
	void Gravity();

	MonsterDIR CurDir;
	float Speed;
	float FallTime;
	bool OnGround;
	int Health;
private:

	GameEngineTextureRenderer* MonsterRenderer;
	GameEngineCollision* MonsterCollision;
	GameEngineCollision* TriggerCollision;

	GameEngineStateManager StateManager;
};

