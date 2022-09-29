#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class ThornTrap : public GameEngineActor
{
public:
	// constrcuter destructer
	ThornTrap();
	~ThornTrap();

	// delete Function
	ThornTrap(const ThornTrap& _Other) = delete;
	ThornTrap(ThornTrap&& _Other) noexcept = delete;
	ThornTrap& operator=(const ThornTrap& _Other) = delete;
	ThornTrap& operator=(ThornTrap&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:

	GameEngineTextureRenderer* ThornTrapRenderer;
	GameEngineCollision* ThornTrapCol;
};

