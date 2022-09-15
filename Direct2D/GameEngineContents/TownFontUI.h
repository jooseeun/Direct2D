#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

// Ό³Έν :
class GameEngineFontRenderer;
class TownFontUI : public GameEngineActor
{
public:
	// constrcuter destructer
	TownFontUI();
	~TownFontUI();

	// delete Function
	TownFontUI(const TownFontUI& _Other) = delete;
	TownFontUI(TownFontUI&& _Other) noexcept = delete;
	TownFontUI& operator=(const TownFontUI& _Other) = delete;
	TownFontUI& operator=(TownFontUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineUIRenderer* Top;
	GameEngineUIRenderer* Bottom;
	GameEngineFontRenderer* Big;
	GameEngineFontRenderer* Small;
	float Time;
};

