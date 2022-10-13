#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"
// Ό³Έν :
class TitleParticle : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleParticle();
	~TitleParticle();

	// delete Function
	TitleParticle(const TitleParticle& _Other) = delete;
	TitleParticle(TitleParticle&& _Other) noexcept = delete;
	TitleParticle& operator=(const TitleParticle& _Other) = delete;
	TitleParticle& operator=(TitleParticle&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineTextureRenderer* Ren;
	float Time;
};

