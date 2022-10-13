#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"
// Ό³Έν :
class TitleEffect : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleEffect();
	~TitleEffect();

	// delete Function
	TitleEffect(const TitleEffect& _Other) = delete;
	TitleEffect(TitleEffect&& _Other) noexcept = delete;
	TitleEffect& operator=(const TitleEffect& _Other) = delete;
	TitleEffect& operator=(TitleEffect&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineTextureRenderer* Ren;
	float Time;
};

