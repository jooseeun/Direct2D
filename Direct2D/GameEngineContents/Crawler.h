#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class Crawler : public GameEngineActor
{
public:
	// constrcuter destructer
	Crawler();
	~Crawler();

	// delete Function
	Crawler(const Crawler& _Other) = delete;
	Crawler(Crawler&& _Other) noexcept = delete;
	Crawler& operator=(const Crawler& _Other) = delete;
	Crawler& operator=(Crawler&& _Other) noexcept = delete;

	GameEngineSoundPlayer SoundPlayer;

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

