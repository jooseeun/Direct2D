#include "PreCompile.h"
#include "GameEngineTextureRenderer.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"

void FrameAnimation::Reset()
{
	Info.FrameTime = 0.0f;
	Info.CurFrame = 0;
}

void FrameAnimation::Update(float _Delta)
{

	Info.FrameTime += _Delta;

	if (nullptr != Time)
	{
		Time(Info, _Delta);
	}

	if (false == bOnceStart
		&& Info.CurFrame == 0)
	{
		if (nullptr != Start)
		{
			Start(Info);
		}
		bOnceStart = true;
		bOnceEnd = false;
	}

	if (Info.Inter <= Info.FrameTime)
	{
		if (Info.CurFrame == (Info.Frames.size() - 1)
			&& false == bOnceEnd
			&& nullptr != End)
		{
			End(Info);
			bOnceEnd = true;
			bOnceStart = false;
		}

		++Info.CurFrame;
		if (nullptr != Frame)
		{
			Frame(Info);
		}

		if (Info.CurFrame >= Info.Frames.size())
		{

			if (true == Info.Loop)
			{
				Info.CurFrame = 0;
			}
			else
			{
				Info.CurFrame = static_cast<unsigned int>(Info.Frames.size()) - 1;
			}
		}

		if (nullptr != Texture)
		{
			ParentRenderer->CurTex = Texture;
			ParentRenderer->SetTexture(Texture, Info.Frames[Info.CurFrame]);
			ParentRenderer->SetPivot();

			// �߷ȴٴ� ����?
			if (Texture->GetCutCount() != 0)
			{
				if (ParentRenderer->ScaleMode == SCALEMODE::IMAGE)
				{
					ParentRenderer->ScaleToCutTexture(Info.Frames[Info.CurFrame]);
				}
			}
			else
			{
				if (ParentRenderer->ScaleMode == SCALEMODE::IMAGE)
				{
					ParentRenderer->ScaleToTexture();
				}
			}
		}
		else if (nullptr != FolderTexture)
		{
			ParentRenderer->FrameDataReset();
			ParentRenderer->CurTex = FolderTexture->GetTexture(Info.Frames[Info.CurFrame]);
			ParentRenderer->SetTexture(FolderTexture->GetTexture(Info.Frames[Info.CurFrame]));
			ParentRenderer->SetPivot();

			if (ParentRenderer->ScaleMode == SCALEMODE::IMAGE)
			{
				ParentRenderer->ScaleToTexture();
			}
		}
		else
		{
			MsgBoxAssert("�ؽ�ó�� ���õ��� ���� �ִϸ��̼� �Դϴ�.");
		}


		Info.FrameTime -= Info.Inter;
	}
}

GameEngineTextureRenderer::GameEngineTextureRenderer()
	: CurAni(nullptr)
	, CurTex(nullptr)
	, PivotMode(PIVOTMODE::CUSTOM)
	, ScaleMode(SCALEMODE::CUSTOM)
	, ScaleRatio(1.0f)
{
}

GameEngineTextureRenderer::~GameEngineTextureRenderer()
{
}

void GameEngineTextureRenderer::SetTextureRendererSetting()
{

	SetPipeLine("TextureAtlas");

	FrameData.PosX = 0.0f;
	FrameData.PosY = 0.0f;
	FrameData.SizeX = 1.0f;
	FrameData.SizeY = 1.0f;

	ShaderResources.SetConstantBufferLink("AtlasData", FrameData);
	ShaderResources.SetConstantBufferLink("ColorData", ColorData);

}

void GameEngineTextureRenderer::Start()
{
	GameEngineDefaultRenderer::Start();

	PushRendererToMainCamera();

	SetTextureRendererSetting();
}

void GameEngineTextureRenderer::SetSamplingModePoint()
{
	ShaderResources.SetSampler("Smp", "EngineSamplerPoint");
}

void GameEngineTextureRenderer::SetSamplingModeLiner()
{
	ShaderResources.SetSampler("Smp", "EngineSamplerLinear");
}

void GameEngineTextureRenderer::SetPivot()
{
	SetPivot(PivotMode);
}

void GameEngineTextureRenderer::SetPivot(PIVOTMODE _Mode)
{
	switch (_Mode)
	{
	case PIVOTMODE::CENTER:
		SetPivotToVector(float4::ZERO);
		break;
	case PIVOTMODE::LEFTTOP:
		SetPivotToVector(float4(GetTransform().GetWorldScale().hx(), -GetTransform().GetWorldScale().hy()));
		break;
	case PIVOTMODE::BOT:
		SetPivotToVector(float4(0.0f, GetTransform().GetWorldScale().hy()));
		break;
	default:
		break;
	}

	PivotMode = _Mode;
}

void GameEngineTextureRenderer::SetPivotToVector(const float4& _Value)
{
	GetTransform().SetLocalPosition(_Value);
}

void GameEngineTextureRenderer::SetTexture(GameEngineTexture* _Texture)
{
	CurTex = _Texture;
	ShaderResources.SetTexture("Tex", _Texture);
}

void GameEngineTextureRenderer::SetTexture(const std::string& _Name)
{
	CurTex = ShaderResources.SetTexture("Tex", _Name);
}

void GameEngineTextureRenderer::SetFrame(UINT _Index)
{
	FrameData = CurTex->GetFrameData(_Index);
}

GameEngineTexture* GameEngineTextureRenderer::GetCurTexture()
{
	return CurTex;
}

void GameEngineTextureRenderer::SetTexture(const std::string& _Name, UINT _Index)
{
	SetTexture(_Name);
	SetFrame(_Index);
}

void GameEngineTextureRenderer::SetTexture(GameEngineTexture* _Texture, UINT _Index)
{
	if (nullptr == _Texture)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	SetTexture(_Texture);
	SetFrame(_Index);
}

void GameEngineTextureRenderer::CreateFrameAnimationFolder(const std::string& _AnimationName, const FrameAnimation_DESC& _Desc)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() != FrameAni.find(Name))
	{
		MsgBoxAssert("�̹� �����ϴ� �ִϸ��̼��� �� ������� �߽��ϴ�.");
		return;
	}

	FrameAnimation& NewAni = FrameAni[Name];
	NewAni.Info = _Desc;
	NewAni.ParentRenderer = this;
	NewAni.Texture = nullptr;
	NewAni.FolderTexture = GameEngineFolderTexture::Find(_Desc.TextureName);

	if (NewAni.Info.Frames.size() == 0)
	{
		for (unsigned int i = 0; i < NewAni.FolderTexture->GetTextureCount(); i++)
		{
			NewAni.Info.Frames.push_back(i);
		}
	}
}

void GameEngineTextureRenderer::CreateFrameAnimationCutTexture(const std::string& _AnimationName, const FrameAnimation_DESC& _Desc)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() != FrameAni.find(Name))
	{
		MsgBoxAssert("�̹� �����ϴ� �ִϸ��̼��� �� ������� �߽��ϴ�.");
		return;
	}

	FrameAnimation& NewAni = FrameAni[Name];
	NewAni.Info = _Desc;
	NewAni.ParentRenderer = this;
	NewAni.Texture = GameEngineTexture::Find(_Desc.TextureName);
	NewAni.FolderTexture = nullptr;
}

void GameEngineTextureRenderer::ChangeFrameAnimation(const std::string& _AnimationName)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() == FrameAni.find(Name))
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	if (CurAni != &FrameAni[Name])
	{
		CurAni = &FrameAni[Name];
		CurAni->Reset();
		if (nullptr != CurAni->Texture)
		{
			SetTexture(CurAni->Texture, CurAni->Info.Frames[CurAni->Info.CurFrame]);
			ScaleToCutTexture(CurAni->Info.CurFrame);
		}
		else if (nullptr != CurAni->FolderTexture)
		{
			SetTexture(CurAni->FolderTexture->GetTexture(CurAni->Info.Frames[CurAni->Info.CurFrame]));
			ScaleToTexture();

		}
	}
}

void GameEngineTextureRenderer::FrameDataReset()
{
	FrameData = { 0.0f , 0.0f, 1.0f, 1.0f };
}


void GameEngineTextureRenderer::Update(float _Delta)
{
	if (nullptr != CurAni)
	{
		CurAni->Update(_Delta);
	}
}



void GameEngineTextureRenderer::ScaleToCutTexture(int _Index)
{
	float4 Scale = CurTex->GetCutScale(_Index);

	// �̰Ŵ� �����մϴ�.
	if (0 > GetTransform().GetLocalScale().x)
	{
		Scale.x = -Scale.x;
	}

	if (0 > GetTransform().GetLocalScale().y)
	{
		Scale.y = -Scale.y;
	}

	GetTransform().SetLocalScale(Scale * ScaleRatio);
}

void GameEngineTextureRenderer::ScaleToTexture()
{
	float4 Scale = CurTex->GetScale();

	// �̰Ŵ� �����մϴ�.
	if (0 > GetTransform().GetLocalScale().x)
	{
		Scale.x = -Scale.x;
	}

	if (0 > GetTransform().GetLocalScale().y)
	{
		Scale.y = -Scale.y;
	}

	GetTransform().SetLocalScale(Scale * ScaleRatio);
}

void GameEngineTextureRenderer::CurAnimationReset()
{
	CurAnimationSetStartPivotFrame(0);
	// CurAni->Info.CurFrame = CurAni->Info.Start;
}

void GameEngineTextureRenderer::CurAnimationSetStartPivotFrame(int SetFrame)
{
	CurAni->Info.CurFrame = SetFrame;
}