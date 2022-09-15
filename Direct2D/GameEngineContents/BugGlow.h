#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class BugGlow : public GameEngineActor
{
public:
	// constrcuter destructer
	BugGlow();
	~BugGlow();

	// delete Function
	BugGlow(const BugGlow& _Other) = delete;
	BugGlow(BugGlow&& _Other) noexcept = delete;
	BugGlow& operator=(const BugGlow& _Other) = delete;
	BugGlow& operator=(BugGlow&& _Other) noexcept = delete;



protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineTextureRenderer* BugGlowRenderer;
	GameEngineTextureRenderer* LightRenderer;

};

