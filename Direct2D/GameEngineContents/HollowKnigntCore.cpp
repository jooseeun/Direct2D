#include "HollowKnigntCore.h"
#include "PreCompile.h"

#include "IntroLevel.h"
#include "TownLevel.h"
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

	//{
	//	// 1, 1, 1, 1 * 뭔가 + 0, 0ㅏ, * 뭔가

	//	D3D11_BLEND_DESC Desc = { 0 };

	//	Desc.AlphaToCoverageEnable = FALSE;
	//	Desc.IndependentBlendEnable = FALSE;
	//	Desc.RenderTarget[0].BlendEnable = true;
	//	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
	//	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	//	Desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	//	// Desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
	//	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	//	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	//	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//	//blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//	//blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//	//blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//	//blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//	//blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//	//blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	//	GameEngineBlend::Create("Mask", Desc);
	//}

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

	//UI 리소스
	GameEngineTexture::Cut("EnergyUI.png", 6, 1);
	GameEngineTexture::Cut("FullHealthUI.png", 6, 1);
	GameEngineTexture::Cut("BreakHealthUI.png", 6, 1);
	GameEngineTexture::Cut("TopUICoin.png", 3, 1);
	GameEngineTexture::Cut("HUD_Soulorb_fills_soul_idle0000-Sheet.png", 6, 1);
	GameEngineTexture::Cut("HUD Cln_soul_orb_glow0000.png", 1, 1);
	
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