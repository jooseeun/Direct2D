#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

// Ό³Έν :
class GameEngineFontRenderer;
class ElderbugFont : public GameEngineActor
{
	static ElderbugFont* ElderFont;
public:
	static ElderbugFont* GetElderFont()
	{
		return ElderFont;
	}
public:
	// constrcuter destructer
	ElderbugFont();
	~ElderbugFont();

	// delete Function
	ElderbugFont(const ElderbugFont& _Other) = delete;
	ElderbugFont(ElderbugFont&& _Other) noexcept = delete;
	ElderbugFont& operator=(const ElderbugFont& _Other) = delete;
	ElderbugFont& operator=(ElderbugFont&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:

	GameEngineTextureRenderer* DiTop;
	GameEngineTextureRenderer* DiBottom;
	GameEngineTextureRenderer* DiBack;

	std::string MainText;
	std::string MainText2;
	std::string Temp;
	std::string Temp2;
	std::string Text[5];
	float TextTime;
	
	int TextNum1;
	int TextNum2;
	int TextNum3;
	GameEngineFontRenderer* Font1;
	GameEngineFontRenderer* Font2;
};

