#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

// Ό³Έν :
class GameEngineFontRenderer;
class LorePromptFont : public GameEngineActor
{
private:
	static LorePromptFont* LorePrompt;
public:
	static LorePromptFont*  GetLorePrompt()
	{
		return LorePrompt;
	}
public:
	// constrcuter destructer
	LorePromptFont();
	~LorePromptFont();

	// delete Function
	LorePromptFont(const LorePromptFont& _Other) = delete;
	LorePromptFont(LorePromptFont&& _Other) noexcept = delete;
	LorePromptFont& operator=(const LorePromptFont& _Other) = delete;
	LorePromptFont& operator=(LorePromptFont&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineUIRenderer* BlackAlphaGround;
	GameEngineUIRenderer* LorePromptRenderer;
	GameEngineUIRenderer* TutorialRenderer;
	GameEngineUIRenderer* KeyRenderer;

	GameEngineFontRenderer* Font1;
	GameEngineFontRenderer* Font2;
	GameEngineFontRenderer* Font3;
	GameEngineFontRenderer* Font4;
	GameEngineFontRenderer* Font5;
	GameEngineFontRenderer* Font6;
	GameEngineFontRenderer* Font7;
};

