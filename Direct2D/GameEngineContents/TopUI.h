#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineUIRenderer;
class GameEngineFontRenderer;
class GameEngineMyRenderer;
class TopUI : public GameEngineActor
{
public:
	static TopUI* MainTopUI;

public:
	static TopUI* GetMainTopUI()
	{
		return MainTopUI;
	}

public:
	// constrcuter destructer
	TopUI();
	~TopUI();

	// delete Function
	TopUI(const TopUI& _Other) = delete;
	TopUI(TopUI&& _Other) noexcept = delete;
	TopUI& operator=(const TopUI& _Other) = delete;
	TopUI& operator=(TopUI&& _Other) noexcept = delete;


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() {}
private:
	GameEngineUIRenderer* EnergyUI;
	GameEngineUIRenderer* MoneyUI;
	GameEngineUIRenderer* Health[5];
	GameEngineUIRenderer* EmpthyHealth[5];
	GameEngineFontRenderer* MoneyFont;
	GameEngineMyRenderer* Energy;
	int CurHealth;
	float AniTime;
	void HealthUpdate();
	void EnergyUpdate();
};

