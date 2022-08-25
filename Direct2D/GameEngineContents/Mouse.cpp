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
	MouseRenderer = CreateComponent<GameEngineUIRenderer>();
	MouseRenderer->SetTexture("Cursor.png");
	MouseRenderer->ScaleToTexture();
	MouseRenderer->GetTransform().SetLocalPosition(float4(GetLevel()->GetMainCamera()->GetMouseWorldPosition()));
	MouseRenderer->ChangeCamera(CAMERAORDER::UICAMERA);
}

void Mouse::Update(float _DeltaTime)
{
	MouseRenderer->GetTransform().SetLocalPosition(float4(GetLevel()->GetMainCamera()->GetMouseWorldPosition())+float4{300,-180});
}