#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class GameEngineUIRenderer;
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
	GameEngineUIRenderer* Health[5];
	GameEngineUIRenderer* EmpthyHealth[5];
	int CurHealth;
	void HealthUpdate();
};
