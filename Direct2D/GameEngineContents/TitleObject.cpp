#include "TitleObject.h"
#include "PreCompile.h"
#include <GameEngineCore/GameEngineTextureRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>

TitleObject::TitleObject() 
	: BackGround(nullptr)
	, TitleLogo(nullptr)
	, GameStartFont(nullptr)
	, GameSet(nullptr)
	, GameExit(nullptr)
	, GameStartCol(nullptr)
{
}

TitleObject::~TitleObject() 
{
}

void TitleObject::Start()
{

	GetTransform().SetLocalScale({ 1, 1, 1 });

	BackGround = CreateComponent<GameEngineTextureRenderer>();
	TitleLogo = CreateComponent< GameEngineTextureRenderer>();

	GameStartFont = CreateComponent<GameEngineTextureRenderer>();
	GameSet = CreateComponent<GameEngineTextureRenderer>();
	GameExit = CreateComponent<GameEngineTextureRenderer>();

	{
		BackGround->SetTexture("MenuBackGround.png");
		BackGround->GetTransform().SetLocalScale({
			BackGround->GetCurTexture()->GetScale().x,
			BackGround->GetCurTexture()->GetScale().y,
			100.0f });
		BackGround->SetPivot(PIVOTMODE::LEFTTOP);
	}

	{
		TitleLogo->SetTexture("Title.png");
		TitleLogo->GetTransform().SetLocalScale({
			TitleLogo->GetCurTexture()->GetScale().x,
			TitleLogo->GetCurTexture()->GetScale().y,
			100.0f });
		TitleLogo->SetPivot(PIVOTMODE::CENTER);
		TitleLogo->GetTransform().SetLocalPosition({ 1024.0f, -650.0f, 0 });
	}

	{
		GameStartFont->SetTexture("GameStart.png");
		GameStartFont->GetTransform().SetLocalScale({
			GameStartFont->GetCurTexture()->GetScale().x,
			GameStartFont->GetCurTexture()->GetScale().y,
			100.0f });
		GameStartFont->SetPivot(PIVOTMODE::CENTER);
		GameStartFont->GetTransform().SetLocalPosition({ 1010.0f, -1000.0f, 0 });

	}

	{
		GameStartCol = CreateComponent<GameEngineCollision>();
		GameStartCol->GetTransform().SetLocalScale({ 134, 42, 100.0f });
		GameStartCol->ChangeOrder(OBJECTORDER::FrontObject);

	}

	//GameEngineUIRenderer* Mouse = CreateComponent<GameEngineUIRenderer>();
	//Mouse->SetTexture();

}
void TitleObject::Update(float _DeltaTime)
{
	
}
void TitleObject::End() {}