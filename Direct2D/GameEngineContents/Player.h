#pragma once
#include <GameEngineCore/CoreMinimal.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class Player : public GameEngineActor
{
private:
	static Player* MainPlayer;

public:
	static Player* GetMainPlayer()
	{
		return MainPlayer;
	}

public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

	inline void SetMapSize(float4 _MapSize)
	{
		MapSize = _MapSize;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

	void CameraCheck();
	bool MapPixelCheck();
	void Gravity();
	GameEngineTextureRenderer* PlayerRenderer;
	GameEngineTextureRenderer* SkilRenderer;

	float4 Color;
	float4 MapSize;

	PLAYERDIR CurDir;

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void MoveStart(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);

	void MoveToIdleStart(const StateInfo& _Info);
	void MoveToIdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void JumpStart(const StateInfo& _Info);
	void JumpUpdate(float _DeltaTime, const StateInfo& _Info);

	void AttackStart(const StateInfo& _Info);
	void AttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void UpAttackStart(const StateInfo& _Info);
	void UpAttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void DownAttackStart(const StateInfo& _Info);
	void DownAttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void FallStart(const StateInfo& _Info);
	void FallUpdate(float _DeltaTime, const StateInfo& _Info);

	void HardLandStart(const StateInfo& _Info);
	void HardLandUpdate(float _DeltaTime, const StateInfo& _Info);

private:
	float Speed;
	float JumpTime;
	float FallTime;
	int AttackNum;
	GameEngineStateManager StateManager;
};

