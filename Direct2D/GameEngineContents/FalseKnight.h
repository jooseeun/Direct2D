#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class FalseKnight : public GameEngineActor
{
public:
	// constrcuter destructer
	FalseKnight();
	~FalseKnight();

	// delete Function
	FalseKnight(const FalseKnight& _Other) = delete;
	FalseKnight(FalseKnight&& _Other) noexcept = delete;
	FalseKnight& operator=(const FalseKnight& _Other) = delete;
	FalseKnight& operator=(FalseKnight&& _Other) noexcept = delete;


protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

private:

	void Gravity();
	bool MapPixelCheck();
	void CheckMonsterDir();

	void MakeWave();
	void DropBall();
	void EffectUpdate();
	void KeyCheck();

	void UpdateDamage();
	CollisionReturn CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other);

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void JumpStart(const StateInfo& _Info);
	void JumpUpdate(float _DeltaTime, const StateInfo& _Info);

	void BackJumpStart(const StateInfo& _Info);
	void BackJumpUpdate(float _DeltaTime, const StateInfo& _Info);

	void FallStart(const StateInfo& _Info);
	void FallUpdate(float _DeltaTime, const StateInfo& _Info);

	void JumpAttackStart(const StateInfo& _Info);
	void JumpAttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void GroundAttackStart(const StateInfo& _Info);
	void GroundAttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void HitAttackStart(const StateInfo& _Info);
	void HitAttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void DropAttackStart(const StateInfo& _Info);
	void DropAttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void LandStart(const StateInfo& _Info);
	void LandUpdate(float _DeltaTime, const StateInfo& _Info);

	void StunStart(const StateInfo& _Info);
	void StunUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);


	MonsterDIR CurDir;
	float Speed;
	float JumpTime;
	float StopTime;
	float SkillTime;
	bool OnGround;
	bool IsMakeWave;
	int Health;
	int DeathNum;
	int SkillNum;
	int AttackNum;
	float AttackTime;
	bool Death;

	GameEngineStateManager StateManager;
	GameEngineTextureRenderer* MonsterRenderer;
	GameEngineTextureRenderer* MonsterHeadRenderer;
	GameEngineTextureRenderer* HPEffect1;
	GameEngineTextureRenderer* HPEffect2;
	GameEngineTextureRenderer* HPEffect3;
	GameEngineCollision* MonsterCollision;
	GameEngineCollision* MonsterHeadCollision;
	GameEngineCollision* LeftSkillCol;
	GameEngineCollision* RightSkillCol;
};

