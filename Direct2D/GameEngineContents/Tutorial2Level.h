#pragma once

#include <GameEngineCore/GameEngineLevel.h>
// Ό³Έν :
class Tutorial2Level : public GameEngineLevel
{
public:
	// constrcuter destructer
	Tutorial2Level();
	~Tutorial2Level();

	// delete Function
	Tutorial2Level(const Tutorial2Level& _Other) = delete;
	Tutorial2Level(Tutorial2Level&& _Other) noexcept = delete;
	Tutorial2Level& operator=(const Tutorial2Level& _Other) = delete;
	Tutorial2Level& operator=(Tutorial2Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void OnEvent() override;
private:

};

