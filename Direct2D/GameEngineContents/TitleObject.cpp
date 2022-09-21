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
	LeftArrow = CreateComponent<GameEngineTextureRenderer>();
	RightArrow = CreateComponent<GameEngineTextureRenderer>();

	GameStartFont = CreateComponent<GameEngineFontRenderer>();
	GameSet = CreateComponent<GameEngineFontRenderer>();
	GameExit = CreateComponent<GameEngineFontRenderer>();
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
		TitleLogo->GetTransform().SetLocalPosition({ 960.0f, -650.0f, 0 });
	}
	{
		LeftArrow->SetTexture("MenuButton.png"); 
		LeftArrow->GetTransform().SetLocalScale({
	LeftArrow->GetCurTexture()->GetScale().x * 0.5f,
	LeftArrow->GetCurTexture()->GetScale().y * 0.5f,
	100.0f });
		LeftArrow->SetPivot(PIVOTMODE::CENTER);
		LeftArrow->GetTransform().SetLocalPosition({ 830.0f, -995.0f, 0 });
	}
	{
		RightArrow->SetTexture("MenuButton.png");
		RightArrow->GetTransform().SetLocalScale({
	RightArrow->GetCurTexture()->GetScale().x * 0.5f,
	RightArrow->GetCurTexture()->GetScale().y * 0.5f,
	100.0f });
		RightArrow->SetPivot(PIVOTMODE::CENTER);
		RightArrow->GetTransform().SetLocalPosition({ 1060.0f, -995.0f, 0 });
		RightArrow->GetTransform().PixLocalNegativeX();
	}
	{
		GameStartFont->SetText("게임시작", "Noto Serif KR");
		GameStartFont->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		GameStartFont->SetSize(24.0f);
		GameStartFont->SetScreenPostion({ 1920*0.7/2-60, 1080*0.7/2 + 50, 1 });
		GameStartFont->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		GameSet->SetText("게임설정", "Noto Serif KR");
		GameSet->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		GameSet->SetSize(24.0f);
		GameSet->SetScreenPostion({ 1920 * 0.7 / 2 - 60, 1080 * 0.7 / 2 + 120, 1 });
		GameSet->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		GameExit->SetText("게임종료", "Noto Serif KR");
		GameExit->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		GameExit->SetSize(24.0f);
		GameExit->SetScreenPostion({ 1920 * 0.7 / 2 - 60, 1080 * 0.7 / 2 + 190, 1 });
		GameExit->ChangeCamera(CAMERAORDER::UICAMERA);
	}

	{
		GameStartCol = CreateComponent<GameEngineCollision>();
		GameStartCol->GetTransform().SetLocalScale({ 140, 50, 100.0f });
		GameStartCol->GetTransform().SetLocalPosition({ 1010.0f, -1000.0f, 0 });
		GameStartCol->ChangeOrder(OBJECTORDER::GameStartUI);

	}
	{
		GameSetCol = CreateComponent<GameEngineCollision>();
		GameSetCol->GetTransform().SetLocalScale({ 140, 50, 100.0f });
		GameSetCol->GetTransform().SetLocalPosition({ 1010.0f, -1100.0f, 0 });
		GameSetCol->ChangeOrder(OBJECTORDER::GameSetUI);

	}
	{
		GameEndCol = CreateComponent<GameEngineCollision>();
		GameEndCol->GetTransform().SetLocalScale({ 140, 50, 100.0f });
		GameEndCol->GetTransform().SetLocalPosition({ 1010.0f, -1200.0f, 0 });
		GameEndCol->ChangeOrder(OBJECTORDER::GameEndUI);
	}


}
void TitleObject::Update(float _DeltaTime)
{
	GameStartCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Mouse, CollisionType::CT_OBB2D,
		std::bind(&TitleObject::CheckStart, this, std::placeholders::_1, std::placeholders::_2)
	);
	GameSetCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Mouse, CollisionType::CT_OBB2D,
		std::bind(&TitleObject::CheckSet, this, std::placeholders::_1, std::placeholders::_2)
	);
	GameEndCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Mouse, CollisionType::CT_OBB2D,
		std::bind(&TitleObject::CheckEnd, this, std::placeholders::_1, std::placeholders::_2)
	);

}
CollisionReturn TitleObject::CheckStart(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	LeftArrow->GetTransform().SetLocalPosition({ 830.0f, -995.0f, 0 });
	RightArrow->GetTransform().SetLocalPosition({ 1060.0f, -995.0f, 0 });
	return CollisionReturn::ContinueCheck;
}
CollisionReturn TitleObject::CheckSet(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	LeftArrow->GetTransform().SetLocalPosition({ 830.0f, -1095.0f, 0 });
	RightArrow->GetTransform().SetLocalPosition({ 1060.0f, -1095.0f, 0 });
	return CollisionReturn::ContinueCheck;
}
CollisionReturn TitleObject::CheckEnd(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	LeftArrow->GetTransform().SetLocalPosition({ 830.0f, -1195.0f, 0 });
	RightArrow->GetTransform().SetLocalPosition({ 1060.0f, -1195.0f, 0 });
	return CollisionReturn::ContinueCheck;
}
void TitleObject::End() {}