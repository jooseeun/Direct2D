#include "HollowKnigntCore.h"
#include "PreCompile.h"

#include "IntroLevel.h"
#include "Town1Level.h"
#include "Town2Level.h"
#include "Town3Level.h"
#include "Tutorial1Level.h"
#include "Tutorial2Level.h"
#include "Tutorial3Level.h"
#include "Tutorial4Level.h"
#include <GameEngineCore/GameEngineBlend.h>

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
	//Shader
	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("TextureMask");
		NewPipe->SetVertexShader("TextureMask.hlsl");
		NewPipe->SetPixelShader("TextureMask.hlsl");
	}
	//PostEffect
	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("BlueLight");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
		NewPipe->SetVertexShader("BlueLight.hlsl");
		NewPipe->SetPixelShader("BlueLight.hlsl");
	}

	///Resource
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
		Dir.Move("Object");

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
		Dir.Move("Effect");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}

	GameEngineFont::Load("Noto Serif KR");

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
	GameEngineTexture::Cut("Player_stun.png", 5, 1);
	
	///Monster 리소스
	GameEngineTexture::Cut("Crawler_walk.png", 4, 1);
	GameEngineTexture::Cut("Crawler_Death.png", 5, 1);
	GameEngineTexture::Cut("Buzzer_idle_01-Sheet.png", 5, 1);
	GameEngineTexture::Cut("Buzzer_fly0000-Sheet.png", 4, 1);
	GameEngineTexture::Cut("Buzzer_death0000-Sheet.png", 3, 1);
	GameEngineTexture::Cut("Buzzer_startle_01-Sheet.png", 4, 1);
	//UI 리소스
	GameEngineTexture::Cut("EnergyUI.png", 6, 1);
	GameEngineTexture::Cut("FullHealthUI.png", 6, 1);
	GameEngineTexture::Cut("BreakHealthUI.png", 6, 1);
	GameEngineTexture::Cut("TopUICoin.png", 3, 1);
	GameEngineTexture::Cut("HUD_Soulorb_fills_soul_idle0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("HUD Cln_soul_orb_glow0000.png", 1, 1);
	GameEngineTexture::Cut("Area Title Cln_fleur_bottom0002-Sheet.png", 20, 1);
	GameEngineTexture::Cut("Area Title Cln_fleur_top0000-Sheet.png", 22, 1);

	//Object 리소스
	GameEngineTexture::Cut("Geo_coin0000-Sheet.png", 8, 1);
	GameEngineTexture::Cut("Geo_coin_air000-Sheet.png", 9, 1);
	GameEngineTexture::Cut("Ambient Life_cave_glow_bug0000-Sheet.png", 4, 1);
	GameEngineTexture::Cut("Ambient Life_bug_glow0000-Sheet.png", 6, 1);
	
	
	//Effect 리소스
	GameEngineTexture::Cut("Hit_crack_simple.png", 3, 1);
	GameEngineTexture::Cut("Stun_impact_effect.png", 7, 1);
	GameEngineTexture::Cut("EnemyHitEffects_1.png", 3, 1);
	GameEngineTexture::Cut("EnemyHitEffects_2.png", 5, 1);
	GameEngineTexture::Cut("EnemyHitEffects_3.png", 5, 1);
	GameEngineTexture::Cut("Geo_coinget_01-Sheet.png", 4, 1);

	
	if (false == GameEngineInput::GetInst()->IsKey("LevelChangeKey"))
	{
		GameEngineInput::GetInst()->CreateKey("LevelChangeKey", 'P');
	}


	// 리소스를 로드하는데.

	// RTTI 런 타임 타입 인포메이션
	CreateLevel<IntroLevel>("Intro");
	CreateLevel<Town1Level>("Town1");
	CreateLevel<Town2Level>("Town2");
	CreateLevel<Town3Level>("Town3");
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