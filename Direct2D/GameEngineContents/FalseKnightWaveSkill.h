#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class FalseKnightWaveSkill : public GameEngineActor
{
public:
	// constrcuter destructer
	FalseKnightWaveSkill();
	~FalseKnightWaveSkill();

	// delete Function
	FalseKnightWaveSkill(const FalseKnightWaveSkill& _Other) = delete;
	FalseKnightWaveSkill(FalseKnightWaveSkill&& _Other) noexcept = delete;
	FalseKnightWaveSkill& operator=(const FalseKnightWaveSkill& _Other) = delete;
	FalseKnightWaveSkill& operator=(FalseKnightWaveSkill&& _Other) noexcept = delete;

	MonsterDIR CurDir;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	GameEngineTextureRenderer* SkillRenderer;
	GameEngineCollision* SkillCol;
	float4 WaveScale[16];
};

