#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class GameEngineFontRenderer;
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
	CollisionReturn PlusAlpha(GameEngineCollision* _This, GameEngineCollision* _Other);
	float4 GetWorldRePosition(float4 ReSize);
	GameEngineTextureRenderer* BackGroundRenderer;
	GameEngineTextureRenderer* LightRenderer;
	GameEngineTextureRenderer* PromptRenderer;
	GameEngineFontRenderer* Font;
	GameEngineCollision* TriggerCol;
	bool Trigger;
};

