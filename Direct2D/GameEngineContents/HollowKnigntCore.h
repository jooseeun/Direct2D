#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCore.h>
// 설명 :
class HollowKnigntCore : public GameEngineCore
{
public:
	// constrcuter destructer
	HollowKnigntCore();
	~HollowKnigntCore();

	// delete Function
	HollowKnigntCore(const HollowKnigntCore& _Other) = delete;
	HollowKnigntCore(HollowKnigntCore&& _Other) noexcept = delete;
	HollowKnigntCore& operator=(const HollowKnigntCore& _Other) = delete;
	HollowKnigntCore& operator=(HollowKnigntCore&& _Other) noexcept = delete;

	std::string GetWindowTitle() override
	{
		return "HollowKnight";
	}
	float4 StartWindowSize()  override 
	{ 
		return { 1920*0.7, 1080*0.7}; 
	}
	float4 StartWindowPosition() override
	{
		return { 10,10 };
	}
protected:

	void Start() override;

	// 이 프로그램이 어느 상황에서건 계속 Update하면서 체크할일이 있다면 여기서 한다.
	// 서버가 붙지 않으면 그럴일은 없다. 게임이 무슨 레벨이건 뭐건 무슨키를 누르면 종료한다.
	// 타이틀을 넘기거나 이런
	void Update(float _DeltaTime) override;

	void End() override;

private:

};

