#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class ChageExplanation : public GameEngineActor
{
public:
	// constrcuter destructer
	ChageExplanation();
	~ChageExplanation();

	// delete Function
	ChageExplanation(const ChageExplanation& _Other) = delete;
	ChageExplanation(ChageExplanation&& _Other) noexcept = delete;
	ChageExplanation& operator=(const ChageExplanation& _Other) = delete;
	ChageExplanation& operator=(ChageExplanation&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	bool PlusAlpha(GameEngineCollision* _This, GameEngineCollision* _Other);

	GameEngineTextureRenderer* BackGroundRenderer;
	GameEngineTextureRenderer* LightRenderer;
	GameEngineCollision* TriggerCol;

};

