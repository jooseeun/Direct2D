#pragma once

// ���� :
class GameEngineStateManager
{
public:
	// constrcuter destructer
	GameEngineStateManager();
	~GameEngineStateManager();

	// delete Function
	GameEngineStateManager(const GameEngineStateManager& _Other) = delete;
	GameEngineStateManager(GameEngineStateManager&& _Other) noexcept = delete;
	GameEngineStateManager& operator=(const GameEngineStateManager& _Other) = delete;
	GameEngineStateManager& operator=(GameEngineStateManager&& _Other) noexcept = delete;

protected:

private:

};

