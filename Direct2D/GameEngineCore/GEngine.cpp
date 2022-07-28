#include "PreCompile.h"
#include "GEngine.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"


GEngine::GEngine() 
{
}

GEngine::~GEngine() 
{
}

void GEngine::ChangeLevel(std::string _Name)
{
	GameEngineCore::ChangeLevel(_Name);
}
