#pragma once

#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class SmallSmoke : public GameEngineActor
{
public:
	// constrcuter destructer
	SmallSmoke();
	~SmallSmoke();

	// delete Function
	SmallSmoke(const SmallSmoke& _Other) = delete;
	SmallSmoke(SmallSmoke&& _Other) noexcept = delete;
	SmallSmoke& operator=(const SmallSmoke& _Other) = delete;
	SmallSmoke& operator=(SmallSmoke&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineTextureRenderer* Smoke;
};

