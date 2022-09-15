#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class GeoCoin : public GameEngineActor
{
public:
	// constrcuter destructer
	GeoCoin();
	~GeoCoin();

	// delete Function
	GeoCoin(const GeoCoin& _Other) = delete;
	GeoCoin(GeoCoin&& _Other) noexcept = delete;
	GeoCoin& operator=(const GeoCoin& _Other) = delete;
	GeoCoin& operator=(GeoCoin&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

	float UpTime;
	int SpringNum;
	void Up();
	void Gravity();
	void DeathCheck();
	bool DeathCoin(GameEngineCollision* _This, GameEngineCollision* _Other);

private:

	GameEngineTextureRenderer* GeoCoinRenderer;
	GameEngineCollision* GeoCoinCol;
};

