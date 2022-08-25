#include "Mouse.h"
#include "PreCompile.h"

Mouse* Mouse::MainMouse = nullptr;

Mouse::Mouse()
	: MouseRenderer(nullptr)
	, MouseCol(nullptr)
{
}

Mouse::~Mouse() 
{
}

void Mouse::Start()
{

	{
		MouseRenderer = CreateComponent<GameEngineUIRenderer>();
		MouseRenderer->SetTexture("Cursor.png");
		MouseRenderer->ScaleToTexture();
		MouseRenderer->GetTransform().SetLocalPosition(GetMouseWorldRePosition(float4{ 1920 * 0.7 , 1080 * 0.7 }));
		MouseRenderer->SetOrder(10000000);
	}

	{
		MouseCol = CreateComponent<GameEngineCollision>();
		MouseCol->GetTransform().SetLocalScale({ 15, 15, 1000 });
		MouseCol->GetTransform().SetLocalPosition(GetMouseWorldRePosition(float4{ 1920 * 0.7 , 1080 * 0.7 }) + float4{ 1024.0f, -900.0f, 100.0f });

	}

}

void Mouse::Update(float _DeltaTime)
{

	MouseRenderer->GetTransform().SetLocalPosition(GetMouseWorldRePosition(float4{ 1920*0.7 , 1080*0.7 }));
	MouseCol->GetTransform().SetLocalPosition(GetMouseWorldRePosition(float4{ 1920 * 0.7 , 1080 * 0.7 }) + float4{ 1024.0f, -900.0f, 100.0f });

}

float4 Mouse::GetMouseWorldRePosition(float4 ReSize)
{
	float4 Pos = GetLevel()->GetUICamera()->GetMouseScreenPosition();

	float4x4 ViewPort;
	ViewPort.ViewPort(ReSize.x, ReSize.y, 0, 0, 0, 1);
	ViewPort.Inverse();

	float4x4 ProjectionInvers = GetLevel()->GetUICamera()->GetProjectionMatrix().InverseReturn();

	Pos = Pos * ViewPort;
	Pos = Pos * ProjectionInvers;

	return Pos;
}

bool Mouse::CheckStart(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	GEngine::ChangeLevel("Tutorial1");
	return true;
}
bool Mouse::CheckEnd(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	exit(0);
	return true;
}