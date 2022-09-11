#include "MapSet.h"
#include "PreCompile.h"
#include <GameEngineCore/GameEngineTextureRenderer.h>

MapSet::MapSet() 
{
}

MapSet::~MapSet() 
{
}

void MapSet::Start()
{
	ColMap = CreateComponent<GameEngineTextureRenderer>();
	BackGround = CreateComponent<GameEngineTextureRenderer>();
	BackObject = CreateComponent<GameEngineTextureRenderer>();
	BackObject->SetSamplingModePoint();
	Ground = CreateComponent<GameEngineTextureRenderer>();
	Ground->SetSamplingModePoint();
	FrontObject = CreateComponent<GameEngineTextureRenderer>();
	FrontObject->SetSamplingModePoint();

}
void MapSet::Update(float _DeltaTime)
{

}
void MapSet::End() {}