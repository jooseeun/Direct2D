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
#include "CrossLoad1.h"
#include "CrossLoad2.h"
#include "CrossLoad3.h"
#include "CrossLoad4.h"
#include "CrossLoad5.h"
#include "CrossLoad6.h"
#include "CrossLoad7.h"
#include "CrossLoad8.h"
#include "CrossLoad9.h"
#include "CrossLoad10.h"
#include "CrossLoadBossLevel.h"
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
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("ColorOverlay");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
		NewPipe->SetVertexShader("ColorOverlay.hlsl");
		NewPipe->SetPixelShader("ColorOverlay.hlsl");
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
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("ContentsResources");
		Dir.Move("ContentsResources");
		Dir.Move("Texture");
		Dir.Move("NPC");

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
		Dir.Move("Boss");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}
	GameEngineFont::Load("Noto Serif KR");
	GameEngineFont::Load("Noto Serif KR Black");
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
	GameEngineTexture::Cut("Player_Charge.png", 12, 1);

	
	///Monster 리소스
	GameEngineTexture::Cut("Crawler_walk.png", 4, 1);
	GameEngineTexture::Cut("Crawler_Death.png", 5, 1);
	GameEngineTexture::Cut("Buzzer_idle_01-Sheet.png", 5, 1);
	GameEngineTexture::Cut("Buzzer_fly0000-Sheet.png", 4, 1);
	GameEngineTexture::Cut("Buzzer_death0000-Sheet.png", 3, 1);
	GameEngineTexture::Cut("Buzzer_startle_01-Sheet.png", 4, 1);

	GameEngineTexture::Cut("Zombie Basic 1_attack0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Zombie Basic 1_death0000-Sheet.png", 9, 1);
	GameEngineTexture::Cut("Zombie Basic 1_idle0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Zombie Basic 1_walk0000-Sheet.png", 7, 1);
	GameEngineTexture::Cut("Zombie Basic 1_turn0000-Sheet.png", 2, 1);

	GameEngineTexture::Cut("Zombie Basic 5_idle0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Zombie Basic 5_death0000-Sheet.png", 9, 1);
	GameEngineTexture::Cut("Zombie Basic 5_attack_v020000-Sheet.png", 10, 1);
	GameEngineTexture::Cut("Zombie Basic 5_walk0000-Sheet.png", 7, 1);
	GameEngineTexture::Cut("Zombie Basic 5_turn0000-Sheet.png", 2, 1);

	///////////Boss
	GameEngineTexture::Cut("False Knight_attack_charge0000-Sheet.png", 7, 1);
	GameEngineTexture::Cut("False Knight_attack0000-Sheet.png", 4, 1);
	GameEngineTexture::Cut("False Knight_death_fall0000-Sheet.png", 3, 1);
	GameEngineTexture::Cut("False Knight_death_head0000-Sheet.png", 14, 1);
	GameEngineTexture::Cut("False Knight_idle0000-Sheet.png", 5, 1);
	GameEngineTexture::Cut("False Knight_jump_antic0000-Sheet.png", 3, 1);
	GameEngineTexture::Cut("False Knight_jump_attack0001-Sheet.png", 11, 1);
	GameEngineTexture::Cut("False Knight_jump0002-Sheet.png", 3, 1);
	GameEngineTexture::Cut("False Knight_pop_out0000-Sheet.png", 5, 1);
	GameEngineTexture::Cut("False Knight_roll0026-Sheet.png", 7, 1);
	GameEngineTexture::Cut("False Knight_staff_leave0025-Sheet.png", 7, 1);
	GameEngineTexture::Cut("False Knight_staff_leave0046-Sheet.png", 4, 1);
	GameEngineTexture::Cut("False Knight_stun_crop0000-Sheet.png", 3, 1);
	GameEngineTexture::Cut("False Knight_stun_damage0000-Sheet.png", 3, 1);
	GameEngineTexture::Cut("False Knight_stun_head_idle0000-Sheet.png", 5, 1);
	GameEngineTexture::Cut("False Knight_stun0000-Sheet.png", 9, 1);
	GameEngineTexture::Cut("False Knight_attack0005-Sheet.png", 8, 1);
	GameEngineTexture::Cut("Boss_WaveSkill_Sheet.png", 16, 1);
	GameEngineTexture::Cut("barrel_fall0000-Sheet.png", 8, 1);
	
	//UI 리소스
	GameEngineTexture::Cut("EnergyUI.png", 6, 1);
	GameEngineTexture::Cut("FullHealthUI.png", 6, 1);
	GameEngineTexture::Cut("BreakHealthUI.png", 6, 1);
	GameEngineTexture::Cut("TopUICoin.png", 3, 1);
	GameEngineTexture::Cut("HUD_Soulorb_fills_soul_idle0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("HUD Cln_appear_v020000-Sheet.png", 5, 1);
	GameEngineTexture::Cut("HUD Cln_soul_orb_glow0000.png", 1, 1);
	GameEngineTexture::Cut("Area Title Cln_fleur_bottom0002-Sheet.png", 20, 1);
	GameEngineTexture::Cut("Area Title Cln_fleur_top0000-Sheet.png", 22, 1);
	GameEngineTexture::Cut("Prompts Cln_prompt_appear0000-Sheet.png", 10, 1);
	GameEngineTexture::Cut("Prompts Cln_prompt_idle0000-Sheet.png", 5, 1);
	GameEngineTexture::Cut("Dialogue Cln_dialogue_fleur_top0000-Sheet.png", 8, 1);
	GameEngineTexture::Cut("Dialogue Cln_dialogue_fleur_bottom0000-Sheet.png", 8, 1);

	//Object 리소스
	GameEngineTexture::Cut("Geo_coin0000-Sheet.png", 8, 1);
	GameEngineTexture::Cut("Geo_coin_air000-Sheet.png", 9, 1);
	GameEngineTexture::Cut("Ambient Life_cave_glow_bug0000-Sheet.png", 4, 1);
	GameEngineTexture::Cut("Ambient Life_bug_glow0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Health Bugs_health_cocoon_top0000-Sheet.png", 23, 1);
	GameEngineTexture::Cut("Health Bugs_health_plant_010013-Sheet.png", 5, 1);
	GameEngineTexture::Cut("Health Bugs_health_plant_020007-Sheet.png", 16, 1);
	GameEngineTexture::Cut("Tute Great Door_door_v01-Sheet.png", 3, 1);
	GameEngineTexture::Cut("Ruins Lift Cln_ornate_door0000-Sheet.png", 7, 1);
	GameEngineTexture::Cut("Ruins Lift Cln_ornate_door0001-Sheet.png", 7, 1);

	
	//Effect 리소스
	GameEngineTexture::Cut("Hit_crack_simple.png", 3, 1);
	GameEngineTexture::Cut("Stun_impact_effect.png", 7, 1);
	GameEngineTexture::Cut("EnemyHitEffects_1.png", 3, 1);
	GameEngineTexture::Cut("EnemyHitEffects_2.png", 5, 1);
	GameEngineTexture::Cut("EnemyHitEffects_3.png", 5, 1);
	GameEngineTexture::Cut("Geo_coinget_01-Sheet.png", 4, 1);
	GameEngineTexture::Cut("Parasite Balloon Cln_Parasite_Blob_Flyer0012-Sheet.png", 4, 1);
	GameEngineTexture::Cut("orange_puff_animated.png", 9, 1);
	GameEngineTexture::Cut("nail_terrain_hit_effect.png", 3, 1);
	GameEngineTexture::Cut("Spell Effects_focus_appear0000-Sheet.png", 7, 1);
	GameEngineTexture::Cut("Spell Effects_burst_effect0000-Sheet.png", 8, 1);

	//NPC 리소스
	GameEngineTexture::Cut("Elderbug_idle_01-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Elderbug_look_left000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Elderbug_reach0000-Sheet.png", 11, 1);
	GameEngineTexture::Cut("Elderbug_talk_left0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Elderbug_talk_right0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("Elderbug_turn000-Sheet.png", 4, 1);


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
	CreateLevel<CrossLoad1>("Cross1");
	CreateLevel<CrossLoad2>("Cross2");
	CreateLevel<CrossLoad3>("Cross3");
	CreateLevel<CrossLoad4>("Cross4");
	CreateLevel<CrossLoad5>("Cross5");
	CreateLevel<CrossLoad6>("Cross6");
	CreateLevel<CrossLoad7>("Cross7");
	CreateLevel<CrossLoad8>("Cross8");
	CreateLevel<CrossLoad9>("Cross9");
	CreateLevel<CrossLoad10>("Cross10");
	CreateLevel<CrossLoadBossLevel>("CrossLoadBoss");
	ChangeLevel("Cross6");

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