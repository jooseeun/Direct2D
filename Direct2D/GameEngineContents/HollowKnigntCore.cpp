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
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("ContentsResources");
		Dir.Move("ContentsResources");
		Dir.Move("Texture");
		Dir.Move("UI");

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
		Dir.Move("Intro");

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
		Dir.Move("Monster");

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
		Dir.Move("Font");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineFont::Load(Shaders[i].GetFullPath());
		}
	}

	//player 리소스
	GameEngineTexture::Cut("PlayerWalk.png", 8, 1);
	GameEngineTexture::Cut("PlayerIdleHighHealth.png", 9, 1);
	GameEngineTexture::Cut("Player_run.png", 8, 1);
	GameEngineTexture::Cut("Player_idle_to_run.png", 5, 1);
	GameEngineTexture::Cut("Player_run_to_idle.png", 6, 1);
	GameEngineTexture::Cut("Player_roar.png", 8, 1);
	GameEngineTexture::Cut("Player_fall.png", 6, 1);
	GameEngineTexture::Cut("Player_jump.png", 6, 1);

	GameEngineTexture::Cut("Player_slash_longer.png", 11, 1);
	GameEngineTexture::Cut("Player_slash_up.png", 5, 1);
	GameEngineTexture::Cut("Player_slash_down.png", 5, 1);
	GameEngineTexture::Cut("Player_slash_effect.png", 8, 1);
	GameEngineTexture::Cut("Player_Upslash_effect.png", 3, 1);
	GameEngineTexture::Cut("Player_Downslash_effect.png", 4, 1);

	GameEngineTexture::Cut("Player_land.png", 3, 1);
	GameEngineTexture::Cut("Player_land_hard.png", 5, 1);

	///Monster 리소스
	GameEngineTexture::Cut("Crawler_walk.png", 4, 1);
	GameEngineTexture::Cut("Crawler_Death.png", 5, 1);

	//UI 리소스
	GameEngineTexture::Cut("EnergyUI.png", 6, 1);
	GameEngineTexture::Cut("FullHealthUI.png", 6, 1);
	GameEngineTexture::Cut("BreakHealthUI.png", 6, 1);

	if (false == GameEngineInput::GetInst()->IsKey("LevelChangeKey"))
	{
		GameEngineInput::GetInst()->CreateKey("LevelChangeKey", 'P');
	}


	// 리소스를 로드하는데.

	// RTTI 런 타임 타입 인포메이션
	CreateLevel<IntroLevel>("Intro");
	CreateLevel<TownLevel>("Town");
	CreateLevel<Tutorial1Level>("Tutorial1");
	CreateLevel<Tutorial2Level>("Tutorial2");
	CreateLevel<Tutorial3Level>("Tutorial3");
	CreateLevel<Tutorial4Level>("Tutorial4");
	ChangeLevel("Intro");

	GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);

}

void HollowKnigntCore::Update(float _DeltaTime)
{
	// 서버가 종료되었어.
	// 무조건 경고 메세지창을 띄워줘야한다.


}

void HollowKnigntCore::End()
{
	int a = 0;
}