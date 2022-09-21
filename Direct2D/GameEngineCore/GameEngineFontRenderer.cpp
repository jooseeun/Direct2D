#include "PreCompile.h"
#include "GameEngineFontRenderer.h"
#include "GameEngineBlend.h"
#include "GameEngineCamera.h"

GameEngineRenderTarget* GameEngineFontRenderer::FontTarget = nullptr;

GameEngineFontRenderer::GameEngineFontRenderer()
	: Font(nullptr)
	, FontSize(20.0f)
	, Color(float4::WHITE)
	, ScreenPostion(float4::ZERO)
{
}

GameEngineFontRenderer::~GameEngineFontRenderer()
{
}

void GameEngineFontRenderer::SetText(const std::string& _Text, const std::string& _Font)
{
	Text = _Text;
	Font = GameEngineFont::Find(_Font);
}

void GameEngineFontRenderer::Start()
{
	if (nullptr == FontTarget)
	{
		FontTarget = GameEngineRenderTarget::Create("FontTarget");
		FontTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), float4::ZERO);
	}

	PushRendererToMainCamera();
}

// ������ ������������ �ʿ䰡 ������ �����
void GameEngineFontRenderer::Render(float _DeltaTime)
{
	// �̰Ŵ� ����Ÿ���� �ؾ߰ڳ׿�.
	if (nullptr == Font)
	{
		return;
	}

	float4 Pos = ScreenPostion;

	if (Mode == FontPositionMode::WORLD)
	{
		Pos = GetTransform().GetWorldPosition();

		Pos *= Camera->GetView();
		Pos *= Camera->GetProjectionMatrix();

		float4x4 ViewPort;

		float4 Size = GameEngineWindow::GetInst()->GetScale();
		ViewPort.ViewPort(Size.x, Size.y, 0.0f, 0.0f, 0.0f, 1.0f);

		Pos *= ViewPort;
	}

	// �̳༮�� ���ο��� �������� �ϴ����� ������.
	// ���� ȭ�鿡�ٰ� �׸��� �ȵ���?
	// ����Ÿ��
	// ���ڴ� �Ǵٸ� ����Ÿ�ٿ� �׸��̴ϴ�.

	// ������ �����ߴ� ����Ÿ���� ���
	//GameEngineRenderTarget::GetPrevRenderTarget();
	//FontTarget->Setting();

	FontTarget->Clear();
	FontTarget->Setting();
	Font->FontDraw(Text, FontSize, Pos, Color, static_cast<int>(LR) | static_cast<int>(TB));
	GameEngineRenderingPipeLine::AllShaderReset();
	Camera->GetCameraRenderTarget()->Merge(FontTarget);

	//GameEngineRenderTarget::SetPrevRenderTarget();
	// FontTarget->R();
}