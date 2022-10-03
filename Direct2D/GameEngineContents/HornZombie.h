#pragma once

#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class HornZombie : public GameEngineActor
{
public:
	// constrcuter destructer
	HornZombie();
	~HornZombie();

	// delete Function
	HornZombie(const HornZombie& _Other) = delete;
	HornZombie(HornZombie&& _Other) noexcept = delete;
	HornZombie& operator=(const HornZombie& _Other) = delete;
	HornZombie& operator=(HornZombie&& _Other) noexcept = delete;

	inline void SetMonsterDir(MonsterDIR _CurDir)
	{
		CurDir = _CurDir;
	}
	inline bool IsDeathReturn()
	{
		return Death_;
	}
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

	void AttackStart(const StateInfo& _Info);
	void AttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);

	MonsterDIR CurDir;
	float Speed;
	float FallTime;
	float StopTime;
	float MoveTime;
	bool OnGround;
	int Health;
	bool Death_;
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

