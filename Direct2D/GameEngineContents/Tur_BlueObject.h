#pragma once

#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class Tur_BlueObject : public GameEngineActor
{
public:
	// constrcuter destructer
	Tur_BlueObject();
	~Tur_BlueObject();

	// delete Function
	Tur_BlueObject(const Tur_BlueObject& _Other) = delete;
	Tur_BlueObject(Tur_BlueObject&& _Other) noexcept = delete;
	Tur_BlueObject& operator=(const Tur_BlueObject& _Other) = delete;
	Tur_BlueObject& operator=(Tur_BlueObject&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineCollision* BlueObjectCollision;
	GameEngineTextureRenderer* BlueObjectRenderer;

	GameEngineTextureRenderer* BlueFlower1;
	GameEngineTextureRenderer* BlueFlower2;
	GameEngineTextureRenderer* BlueFlower3;
	GameEngineTextureRenderer* BlueFlower4;

	float AniTime;
	CollisionReturn CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other);
};

