#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "GlobalContentsValue.h"

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


protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

	void Gravity();
	bool MapPixelCheck();
	
	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void MoveStart(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);

	void AttackStart(const StateInfo& _Info);
	void AttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);

	MonsterDIR CurDir;
	float Speed;
private:

	GameEngineTextureRenderer* MonsterRenderer;
	GameEngineCollision* MonsterCollision;
	GameEngineCollision* TriggerCollision;

	GameEngineStateManager StateManager;
};

