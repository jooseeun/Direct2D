#pragma once

#include <GameEngineCore/GameEngineLevel.h>
// Ό³Έν :
class Tutorial4Level : public GameEngineLevel
{
public:
	// constrcuter destructer
	Tutorial4Level();
	~Tutorial4Level();

	// delete Function
	Tutorial4Level(const Tutorial4Level& _Other) = delete;
	Tutorial4Level(Tutorial4Level&& _Other) noexcept = delete;
	Tutorial4Level& operator=(const Tutorial4Level& _Other) = delete;
	Tutorial4Level& operator=(Tutorial4Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;
private:

};

