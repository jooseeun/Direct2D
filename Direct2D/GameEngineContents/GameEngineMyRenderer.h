#pragma once
#include <GameEngineCore/GameEngineDefaultRenderer.h>
#include <GameEngineBase/GameEngineDebug.h>

enum class MYPIVOTMODE
{
	CENTER,
	BOT,
	TOP,
	LEFT,
	RIGHT,
	LEFTTOP,
	RIGHTTOP,
	LEFTBOT,
	RIGHTBOT,
	CUSTOM,
};

enum class MYSCALEMODE
{
	IMAGE,
	CUSTOM,
};

struct MyPixelData
{
	float4 MulColor;
	float4 PlusColor;
	float4 Slice;

	MyPixelData()
		: MulColor(float4::WHITE)
		, PlusColor(float4::ZERO)
		, Slice(float4::ZERO)
	{

	}
};

struct MyAtlasData
{
public:
	float4 FrameData;
	float4 PivotPos;
};


class MyFrameAnimation_DESC
{
public:
	std::string TextureName;

	unsigned int CurFrame;

	std::vector<unsigned int> Frames;

	float FrameTime;

	float Inter; // 0.1f

	bool Loop;
	// ��Ʋ�� �ִϸ��̼�

	class GameEngineMyRenderer* Renderer;

public:
	MyFrameAnimation_DESC()
		: Loop(false)
		, Inter(0.1f)
		, CurFrame(-1)
		, FrameTime(0.0f)
	{

	}

	MyFrameAnimation_DESC(const std::string _TextureName, unsigned int _Start, unsigned int _End, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, CurFrame(0)
		, FrameTime(0.0f)
	{
		for (unsigned int i = _Start; i <= _End; i++)
		{
			Frames.push_back(i);
		}
	}

	MyFrameAnimation_DESC(const std::string _TextureName, const std::vector<unsigned int>& _Frames, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, Frames(_Frames)
		, FrameTime(0.0f)
	{

	}


	MyFrameAnimation_DESC(const std::string _TextureName, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, CurFrame(0)
		, FrameTime(0.0f)
	{

	}
};

class GameEngineFolderTexture;
class GameEngineMyRenderer;
class MyFrameAnimation : public GameEngineNameObject
{
	friend GameEngineMyRenderer;

	MyFrameAnimation_DESC Info;

	GameEngineMyRenderer* ParentRenderer;
	GameEngineTexture* Texture;
	GameEngineFolderTexture* FolderTexture;

	bool Pause;
	bool bOnceStart;
	bool bOnceEnd;
	std::function<void(const MyFrameAnimation_DESC&)> Frame;
	std::function<void(const MyFrameAnimation_DESC&)> End;
	std::function<void(const MyFrameAnimation_DESC&)> Start;
	std::function<void(const MyFrameAnimation_DESC&, float)> Time;

	void PauseSwtich();

	void Reset();

	void Update(float _DeltaTime);

public:
	MyFrameAnimation()
		: bOnceStart(true)
		, bOnceEnd(false)
		, Pause(false)
	{

	}
};

// ���� :
class GameEngineMyRenderer : public GameEngineDefaultRenderer
{
	friend MyFrameAnimation;
public:
	// constrcuter destructer
	GameEngineMyRenderer();
	~GameEngineMyRenderer();

	// delete Function
	GameEngineMyRenderer(const GameEngineMyRenderer& _Other) = delete;
	GameEngineMyRenderer(GameEngineMyRenderer&& _Other) noexcept = delete;
	GameEngineMyRenderer& operator=(const GameEngineMyRenderer& _Other) = delete;
	GameEngineMyRenderer& operator=(GameEngineMyRenderer&& _Other) noexcept = delete;

	void SetSamplingModePoint();
	void SetSamplingModeLiner();

	void SetScaleModeImage()
	{
		ScaleMode = MYSCALEMODE::IMAGE;
	}

	void SetScaleRatio(float _Scale)
	{
		ScaleRatio = _Scale;
	}

	float GetScaleRatio()
	{
		return ScaleRatio;
	}

	bool IsCurAnimation()
	{
		if (nullptr == CurAni)
		{
			return false;
		}
		return true;
	}

	void SetTexture(GameEngineTexture* _Texture);

	void SetTexture(const std::string& _Name);

	void SetFrame(UINT _Index);

	GameEngineTexture* GetCurTexture();

	void SetTexture(const std::string& _Name, UINT _Index);
	void SetMaskingTexture(GameEngineTexture* _Texture, const std::string& _MaskingImageName);
	void SetMaskingTexture(const std::string& _Name, const std::string& _MaskingImageName);
	void SetMaskingTexture(const std::string& _Name, const std::string& _MaskingImageName, UINT _Index);
	void SetMaskingTexture(GameEngineTexture* _Texture, UINT _Index, const std::string& _MaskingImageName);
	void SetPivot();

	void SetPivot(MYPIVOTMODE _Mode);

	void SetPivotToVector(const float4& _Value);

	void SetTexture(GameEngineTexture* _Texture, UINT _Index);

	void SetFolderTextureToIndex(const std::string& _Text, UINT _Index);

	void CreateFrameAnimationFolder(const std::string& _AnimationName, const MyFrameAnimation_DESC& _Desc);

	void CreateFrameAnimationCutTexture(const std::string& _AnimationName, const MyFrameAnimation_DESC& _Desc);

	void ChangeFrameAnimation(const std::string& _AnimationName);
	void ChangeMaskFrameAnimation(const std::string& _AnimationName, const std::string& _MaskImage);

	void ScaleToTexture();

	void ScaleToCutTexture(int _Index);

	void CurAnimationPauseSwitch();

	void CurAnimationPauseOn();

	void CurAnimationPauseOff();

	bool IsCurAnimationPause();

	void CurAnimationReset();

	void CurAnimationSetStartPivotFrame(int SetFrame);

	MyPixelData& GetPixelData()
	{
		return PixelDataInst;
	}


	// �ִϸ��̼� ���ε�
	// ���� �����ӿ� ���´�.
	void AnimationBindStart(const std::string& _AnimationName, std::function<void(const MyFrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Start = _Function;
	}
	// ������ �����ӿ� ���´�
	void AnimationBindEnd(const std::string& _AnimationName, std::function<void(const MyFrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].End = _Function;
	}
	// �������� �ٲ𶧸��� ���´�
	void AnimationBindFrame(const std::string& _AnimationName, std::function<void(const MyFrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Frame = _Function;
	}
	// Update
	void AnimationBindTime(const std::string& _AnimationName, std::function<void(const MyFrameAnimation_DESC&, float)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Time = _Function;
	}

protected:
	void Start() override;

	void SetTextureRendererSetting();

	void Update(float _Delta) override;

private:
	MYPIVOTMODE PivotMode;
	MYSCALEMODE ScaleMode;
	float ScaleRatio;

	GameEngineTexture* CurTex;
	GameEngineTexture* CurMaskingTex;

	MyPixelData PixelDataInst;
	MyAtlasData AtlasDataInst;

	std::map<std::string, MyFrameAnimation> FrameAni;
	MyFrameAnimation* CurAni;

	void FrameDataReset();
};

