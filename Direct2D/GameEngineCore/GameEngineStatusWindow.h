#pragma once
#include "GameEngineGUI.h"

// Ό³Έν :
class GameEngineStatusWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	GameEngineStatusWindow();
	~GameEngineStatusWindow();

	// delete Function
	GameEngineStatusWindow(const GameEngineStatusWindow& _Other) = delete;
	GameEngineStatusWindow(GameEngineStatusWindow&& _Other) noexcept = delete;
	GameEngineStatusWindow& operator=(const GameEngineStatusWindow& _Other) = delete;
	GameEngineStatusWindow& operator=(GameEngineStatusWindow&& _Other) noexcept = delete;

protected:

private:
	void Initialize(class GameEngineLevel* _Level) override;
	void OnGUI(GameEngineLevel* _Level, float _DeltaTime) override;
};

