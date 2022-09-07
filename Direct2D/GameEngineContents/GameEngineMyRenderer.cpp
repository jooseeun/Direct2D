#include "GameEngineMyRenderer.h"

#include "PreCompile.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineFolderTexture.h>


void MyFrameAnimation::PauseSwtich()
{
	Pause = !Pause;
}

void MyFrameAnimation::Reset()
{
	Info.FrameTime = 0.0f;
	Info.CurFrame = 0;
}

void MyFrameAnimation::Update(float _Delta)
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

		// 잘렸다는 거죠?
		if (Texture->GetCutCount() != 0)
		{
			if (ParentRenderer->ScaleMode == MYSCALEMODE::IMAGE)
			{
				ParentRenderer->ScaleToCutTexture(Info.Frames[Info.CurFrame]);
			}
		}
		else
		{
			if (ParentRenderer->ScaleMode == MYSCALEMODE::IMAGE)
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

		if (ParentRenderer->ScaleMode == MYSCALEMODE::IMAGE)
		{
			ParentRenderer->ScaleToTexture();
		}
	}
	else
	{
		MsgBoxAssert("텍스처가 세팅되지 않은 애니메이션 입니다.");
	}

}

GameEngineMyRenderer::GameEngineMyRenderer()
	: CurAni(nullptr)
	, CurTex(nullptr)
	, CurMaskingTex(nullptr)
	, PivotMode(MYPIVOTMODE::CUSTOM)
	, ScaleMode(MYSCALEMODE::CUSTOM)
	, ScaleRatio(1.0f)
{
}

GameEngineMyRenderer::~GameEngineMyRenderer()
{
}

void GameEngineMyRenderer::SetTextureRendererSetting()
{


	SetPipeLine("TextureMask");

	AtlasDataInst.FrameData.PosX = 0.0f;
	AtlasDataInst.FrameData.PosY = 0.0f;
	AtlasDataInst.FrameData.SizeX = 1.0f;
	AtlasDataInst.FrameData.SizeY = 1.0f;
	AtlasDataInst.PivotPos = float4::ZERO;

	ShaderResources.SetConstantBufferLink("MyAtlasData", AtlasDataInst);
	ShaderResources.SetConstantBufferLink("MyPixelData", PixelDataInst);

}

void GameEngineMyRenderer::CurAnimationPauseSwitch()
{
	CurAni->PauseSwtich();
}

void GameEngineMyRenderer::Start()
{
	GameEngineDefaultRenderer::Start();

	PushRendererToMainCamera();

	SetTextureRendererSetting();
}

void GameEngineMyRenderer::SetSamplingModePoint()
{
	ShaderResources.SetSampler("Smp", "EngineSamplerPoint");
}

void GameEngineMyRenderer::SetSamplingModeLiner()
{
	ShaderResources.SetSampler("Smp", "EngineSamplerLinear");
}

void GameEngineMyRenderer::SetPivot()
{
	SetPivot(PivotMode);
}

void GameEngineMyRenderer::SetPivot(MYPIVOTMODE _Mode)
{
	switch (_Mode)
	{
	case MYPIVOTMODE::CENTER:
		AtlasDataInst.PivotPos = float4::ZERO;
		break;
	case MYPIVOTMODE::BOT:
		AtlasDataInst.PivotPos = float4(0.0f, 0.5f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::TOP:
		AtlasDataInst.PivotPos = float4(0.0f, -0.5f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::LEFT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::RIGHT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::LEFTTOP:
		AtlasDataInst.PivotPos = float4(0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::RIGHTTOP:
		AtlasDataInst.PivotPos = float4(-0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::LEFTBOT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::RIGHTBOT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case MYPIVOTMODE::CUSTOM:
		break;
	default:
		break;
	}

	PivotMode = _Mode;
}

void GameEngineMyRenderer::SetPivotToVector(const float4& _Value)
{
	GetTransform().SetLocalPosition(_Value);
}

void GameEngineMyRenderer::SetTexture(GameEngineTexture* _Texture)
{
	CurTex = _Texture;
	ShaderResources.SetTexture("Tex", _Texture);
}
void GameEngineMyRenderer::SetMaskingTexture(GameEngineTexture* _Texture, const std::string& _MaskingImageName)
{
	CurTex = _Texture;
	ShaderResources.SetTexture("Tex", _Texture);
	CurMaskingTex = ShaderResources.SetTexture("Mask", _MaskingImageName);
}
void GameEngineMyRenderer::SetTexture(const std::string& _Name)
{
	CurTex = ShaderResources.SetTexture("Tex", _Name);
}

void GameEngineMyRenderer::SetMaskingTexture(const std::string& _Name, const std::string& _MaskingImageName)
{
	CurTex = ShaderResources.SetTexture("Tex", _Name);
	CurMaskingTex = ShaderResources.SetTexture("Mask", _MaskingImageName);
}


void GameEngineMyRenderer::SetFrame(UINT _Index)
{
	AtlasDataInst.FrameData = CurTex->GetFrameData(_Index);
}

GameEngineTexture* GameEngineMyRenderer::GetCurTexture()
{
	return CurTex;
}

void GameEngineMyRenderer::SetTexture(const std::string& _Name, UINT _Index)
{
	SetTexture(_Name);
	SetFrame(_Index);
}
void GameEngineMyRenderer::SetMaskingTexture(const std::string& _Name, const std::string& _MaskingImageName, UINT _Index)
{

	SetMaskingTexture(_Name, _MaskingImageName);
	SetFrame(_Index);
}
void GameEngineMyRenderer::SetTexture(GameEngineTexture* _Texture, UINT _Index)
{
	if (nullptr == _Texture)
	{
		MsgBoxAssert("존재하지 않는 텍스처를 사용하려고 했습니다.");
		return;
	}

	SetTexture(_Texture);
	SetFrame(_Index);
}
void GameEngineMyRenderer::SetMaskingTexture(GameEngineTexture* _Texture, UINT _Index, const std::string& _MaskingImageName)
{
	if (nullptr == _Texture)
	{
		MsgBoxAssert("존재하지 않는 텍스처를 사용하려고 했습니다.");
		return;
	}

	SetMaskingTexture(_Texture, _MaskingImageName);
	SetFrame(_Index);
}
void GameEngineMyRenderer::SetFolderTextureToIndex(const std::string& _Text, UINT _Index)
{
	GameEngineFolderTexture* FolderTexture = GameEngineFolderTexture::Find(_Text);

	SetTexture(FolderTexture->GetTexture(_Index));

	AtlasDataInst.FrameData.PosX = 0.0f;
	AtlasDataInst.FrameData.PosY = 0.0f;
	AtlasDataInst.FrameData.SizeX = 1.0f;
	AtlasDataInst.FrameData.SizeY = 1.0f;
}

void GameEngineMyRenderer::CreateFrameAnimationFolder(const std::string& _AnimationName, const MyFrameAnimation_DESC& _Desc)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() != FrameAni.find(Name))
	{
		MsgBoxAssert("이미 존재하는 애니메이션을 또 만들려고 했습니다.");
		return;
	}

	MyFrameAnimation& NewAni = FrameAni[Name];
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

void GameEngineMyRenderer::CreateFrameAnimationCutTexture(const std::string& _AnimationName, const MyFrameAnimation_DESC& _Desc)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() != FrameAni.find(Name))
	{
		MsgBoxAssert("이미 존재하는 애니메이션을 또 만들려고 했습니다.");
		return;
	}

	MyFrameAnimation& NewAni = FrameAni[Name];
	NewAni.Info = _Desc;
	NewAni.Info.Renderer = this;
	NewAni.ParentRenderer = this;
	NewAni.Texture = GameEngineTexture::Find(_Desc.TextureName);
	NewAni.FolderTexture = nullptr;
}

void GameEngineMyRenderer::ChangeFrameAnimation(const std::string& _AnimationName)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() == FrameAni.find(Name))
	{
		MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	if (CurAni != &FrameAni[Name])
	{
		CurAni = &FrameAni[Name];
		CurAni->Reset();
		if (nullptr != CurAni->Texture)
		{
			SetTexture(CurAni->Texture, CurAni->Info.Frames[CurAni->Info.CurFrame]);
			if (ScaleMode == MYSCALEMODE::IMAGE)
			{
				ScaleToCutTexture(CurAni->Info.CurFrame);
			}
		}
		else if (nullptr != CurAni->FolderTexture)
		{
			SetTexture(CurAni->FolderTexture->GetTexture(CurAni->Info.Frames[CurAni->Info.CurFrame]));
			if (ScaleMode == MYSCALEMODE::IMAGE)
			{
				ScaleToTexture();
			}
		}
	}
}
void GameEngineMyRenderer::ChangeMaskFrameAnimation(const std::string& _AnimationName, const std::string& _MaskImage)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() == FrameAni.find(Name))
	{
		MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	if (CurAni != &FrameAni[Name])
	{
		CurAni = &FrameAni[Name];
		CurAni->Reset();
		if (nullptr != CurAni->Texture)
		{
			SetMaskingTexture(CurAni->Texture, CurAni->Info.Frames[CurAni->Info.CurFrame], _MaskImage);
			if (ScaleMode == MYSCALEMODE::IMAGE)
			{
				ScaleToCutTexture(CurAni->Info.CurFrame);
			}
		}
		else if (nullptr != CurAni->FolderTexture)
		{
			SetMaskingTexture(CurAni->FolderTexture->GetTexture(CurAni->Info.Frames[CurAni->Info.CurFrame]), _MaskImage);
			if (ScaleMode == MYSCALEMODE::IMAGE)
			{
				ScaleToTexture();
			}
		}
	}
}

void GameEngineMyRenderer::FrameDataReset()
{
	AtlasDataInst.FrameData = { 0.0f , 0.0f, 1.0f, 1.0f };
}


void GameEngineMyRenderer::Update(float _Delta)
{
	if (nullptr != CurAni)
	{
		CurAni->Update(_Delta);
	}
}



void GameEngineMyRenderer::ScaleToCutTexture(int _Index)
{
	float4 Scale = CurTex->GetCutScale(_Index);

	// 이거는 봐야합니다.
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

void GameEngineMyRenderer::ScaleToTexture()
{
	float4 Scale = CurTex->GetScale();

	// 이거는 봐야합니다.
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

void GameEngineMyRenderer::CurAnimationReset()
{
	CurAni->Reset();
}

void GameEngineMyRenderer::CurAnimationSetStartPivotFrame(int SetFrame)
{
	CurAni->Info.CurFrame = SetFrame;
}

void GameEngineMyRenderer::CurAnimationPauseOn()
{
	CurAni->Pause = true;
}

void GameEngineMyRenderer::CurAnimationPauseOff()
{
	CurAni->Pause = false;
}

bool GameEngineMyRenderer::IsCurAnimationPause()
{
	return CurAni->Pause;
}