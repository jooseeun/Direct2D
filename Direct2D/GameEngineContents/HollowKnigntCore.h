#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCore.h>
// ���� :
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
protected:
	void Start() override;

	// �� ���α׷��� ��� ��Ȳ������ ��� Update�ϸ鼭 üũ������ �ִٸ� ���⼭ �Ѵ�.
	// ������ ���� ������ �׷����� ����. ������ ���� �����̰� ���� ����Ű�� ������ �����Ѵ�.
	// Ÿ��Ʋ�� �ѱ�ų� �̷�
	void Update(float _DeltaTime) override;

	void End() override;

private:

};

