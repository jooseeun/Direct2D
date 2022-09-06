#include "PreCompile.h"
#include "GameEngineTextureRenderer.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"

void FrameAnimation::PauseSwtich() 
{
	Pause = !Pause;
}

void FrameAnimation::Reset()
{
	Info.FrameTime = 0.0f;
	Info.CurFrame = 0;
}

void FrameAnimation::Update(float _Delta) 
{
	if (false == Pause)
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
				&& false == bOnceEnd)
			{
				if (nullptr != End)
				{
					End(Info);
				}
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
			Info.FrameTime -= Info.Inter;
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

	AtlasDataInst.FrameData.PosX = 0.0f;
	AtlasDataInst.FrameData.PosY = 0.0f;
	AtlasDataInst.FrameData.SizeX = 1.0f;
	AtlasDataInst.FrameData.SizeY = 1.0f;
	AtlasDataInst.PivotPos = float4::ZERO;

	ShaderResources.SetConstantBufferLink("AtlasData", AtlasDataInst);
	ShaderResources.SetConstantBufferLink("PixelData", PixelDataInst);
	
}

void GameEngineTextureRenderer::CurAnimationPauseSwitch()
{
	CurAni->PauseSwtich();
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
		AtlasDataInst.PivotPos = float4::ZERO;
		break;
	case PIVOTMODE::BOT:
		AtlasDataInst.PivotPos = float4(0.0f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::TOP:
		AtlasDataInst.PivotPos = float4(0.0f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFTTOP:
		AtlasDataInst.PivotPos = float4(0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHTTOP:
		AtlasDataInst.PivotPos = float4(-0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFTBOT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHTBOT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::CUSTOM:
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
	AtlasDataInst.FrameData = CurTex->GetFrameData(_Index);
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

void GameEngineTextureRenderer::SetFolderTextureToIndex(const std::string& _Text, UINT _Index)
{
	GameEngineFolderTexture* FolderTexture = GameEngineFolderTexture::Find(_Text);

	SetTexture(FolderTexture->GetTexture(_Index));

	AtlasDataInst.FrameData.PosX = 0.0f;
	AtlasDataInst.FrameData.PosY = 0.0f;
	AtlasDataInst.FrameData.SizeX = 1.0f;
	AtlasDataInst.FrameData.SizeY = 1.0f;
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
	NewAni.Info.Renderer = this;
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
	NewAni.Info.Renderer = this;
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
			if (ScaleMode == SCALEMODE::IMAGE)
			{
				ScaleToCutTexture(CurAni->Info.CurFrame);
			}
		}
		else if(nullptr != CurAni->FolderTexture)
		{
			SetTexture(CurAni->FolderTexture->GetTexture(CurAni->Info.Frames[CurAni->Info.CurFrame]));
			if (ScaleMode == SCALEMODE::IMAGE)
			{
				ScaleToTexture();
			}
		}
	}
}

void GameEngineTextureRenderer::FrameDataReset()
{
	AtlasDataInst.FrameData = { 0.0f , 0.0f, 1.0f, 1.0f};
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
	CurAni->Reset();
}

void GameEngineTextureRenderer::CurAnimationSetStartPivotFrame(int SetFrame)
{
	CurAni->Info.CurFrame = SetFrame;
}

void GameEngineTextureRenderer::CurAnimationPauseOn() 
{
	CurAni->Pause = true;
}

void GameEngineTextureRenderer::CurAnimationPauseOff() 
{
	CurAni->Pause = false;
}

bool GameEngineTextureRenderer::IsCurAnimationPause() 
{
	return CurAni->Pause;
}