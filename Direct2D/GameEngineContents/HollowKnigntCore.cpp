#include "HollowKnigntCore.h"
#include "PreCompile.h"

#include "IntroLevel.h"
#include "TownLevel.h"
#include "Tutorial1Level.h"
#include "Tutorial2Level.h"
#include "Tutorial3Level.h"
#include "Tutorial4Level.h"

#pragma comment(lib, "GameEngineBase.lib")

HollowKnigntCore::HollowKnigntCore() 
	: GameEngineCore()
{
}

HollowKnigntCore::~HollowKnigntCore() 
{
}

void HollowKnigntCore::Start()
{
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("ContentsResources");
		Dir.Move("ContentsResources");
		Dir.Move("Texture");
		Dir.Move("Player");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("ContentsResources");
		Dir.Move("ContentsResources");
		Dir.Move("Texture");
		Dir.Move("PlayMap");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}
	
	
	GameEngineTexture::Cut("PlayerWalk.png", 1, 8);


	if (false == GameEngineInput::GetInst()->IsKey("LevelChangeKey"))
	{
		GameEngineInput::GetInst()->CreateKey("LevelChangeKey", 'P');
	}

	// ���ҽ��� �ε��ϴµ�.

	// RTTI �� Ÿ�� Ÿ�� �������̼�
	CreateLevel<IntroLevel>("Intro");
	CreateLevel<TownLevel>("Town");
	CreateLevel<Tutorial1Level>("Tutorial1");
	CreateLevel<Tutorial2Level>("Tutorial2");
	CreateLevel<Tutorial3Level>("Tutorial3");
	CreateLevel<Tutorial4Level>("Tutorial4");
	ChangeLevel("Tutorial1");

	GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);

}

void HollowKnigntCore::Update(float _DeltaTime)
{
	// ������ ����Ǿ���.
	// ������ ��� �޼���â�� �������Ѵ�.


}

void HollowKnigntCore::End()
{
	int a = 0;
}