#pragma once
#include "GameEngineDefaultRenderer.h"

enum class PIVOTMODE
{
	CENTER,
	LEFTTOP,
	BOT,
	CUSTOM,
};

class FrameAnimation_DESC 
{
public:
	std::string TextureName;

	unsigned int CurFrame;
	unsigned int Start;
	unsigned int End;

	float FrameTime;

	float Inter; // 0.1f

	bool Loop;
	// ��Ʋ�� �ִϸ��̼�

public:
	FrameAnimation_DESC()
		: Loop(false)
		, Inter(0.1f)
		, CurFrame(-1)
		, Start(-1)
		, End(-1)
		, FrameTime(0.0f)
	{

	}

	FrameAnimation_DESC(const std::string _TextureName, unsigned int _Start, unsigned int _End, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, CurFrame(_Start)
		, Start(_Start)
		, End(_End)
		, FrameTime(0.0f)
	{

	}

	FrameAnimation_DESC(const std::string _TextureName, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, CurFrame(0)
		, Start(-1)
		, End(-1)
		, FrameTime(0.0f)
	{

	}
};

class GameEngineFolderTexture;
class GameEngineTextureRenderer;
class FrameAnimation : public GameEngineNameObject
{
	friend GameEngineTextureRenderer;

	FrameAnimation_DESC Info;

	GameEngineTextureRenderer* ParentRenderer;
	GameEngineTexture* Texture;
	GameEngineFolderTexture* FolderTexture;

	bool bOnceStart;
	bool bOnceEnd;
	std::function<void(const FrameAnimation_DESC&)> Frame;
	std::function<void(const FrameAnimation_DESC&)> End;
	std::function<void(const FrameAnimation_DESC&)> Start;
	std::function<void(const FrameAnimation_DESC&, float)> Time;

	void Reset();

	void Update(float _DeltaTime);
};

// ���� :
class GameEngineTextureRenderer : public GameEngineDefaultRenderer
{
	friend FrameAnimation;

public:
	// constrcuter destructer
	GameEngineTextureRenderer();
	~GameEngineTextureRenderer();

	// delete Function
	GameEngineTextureRenderer(const GameEngineTextureRenderer& _Other) = delete;
	GameEngineTextureRenderer(GameEngineTextureRenderer&& _Other) noexcept = delete;
	GameEngineTextureRenderer& operator=(const GameEngineTextureRenderer& _Other) = delete;
	GameEngineTextureRenderer& operator=(GameEngineTextureRenderer&& _Other) noexcept = delete;

	void SetSamplingModePoint();
	void SetSamplingModeLiner();

	void SetTexture(GameEngineTexture* _Texture);

	void SetTexture(const std::string& _Name);

	void SetFrame(UINT _Index);

	GameEngineTexture* GetCurTexture();

	void SetTexture(const std::string& _Name, UINT _Index);

	void SetPivot();

	void SetPivot(PIVOTMODE _Mode);

	void SetPivotToVector(const float4& _Value);

	void SetTexture(GameEngineTexture* _Texture, UINT _Index);

	void CreateFrameAnimationFolder(const std::string& _AnimationName, const FrameAnimation_DESC& _Desc);

	void CreateFrameAnimation(const std::string& _AnimationName, const FrameAnimation_DESC& _Desc);
	void ChangeFrameAnimation(const std::string& _AnimationName);

	void ScaleToTexture();

	void CurAnimationReset();

	void CurAnimationSetStartPivotFrame(int SetFrame);


	// �ִϸ��̼� ���ε�
	// ���� �����ӿ� ���´�.
	template<typename ObjectType>
	void AnimationBindStart(const std::string& _AnimationName, void(ObjectType::* _Ptr)(const FrameAnimation_DESC&), ObjectType* _This)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Start = std::bind(_Ptr, _This, FrameAni[Name].Info);
	}
	// ������ �����ӿ� ���´�
	template<typename ObjectType>
	void AnimationBindEnd(const std::string& _AnimationName, void(ObjectType::* _Ptr)(const FrameAnimation_DESC&), ObjectType* _This)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].End = std::bind(_Ptr, _This, FrameAni[Name].Info);
	}
	// �������� �ٲ𶧸��� ���´�
	template<typename ObjectType>
	void AnimationBindFrame(const std::string& _AnimationName, void(ObjectType::* _Ptr)(const FrameAnimation_DESC&), ObjectType* _This)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Frame = std::bind(_Ptr, _This, FrameAni[Name].Info);
	}
	// Update
	template<typename ObjectType>
	void AnimationBindTime(const std::string& _AnimationName, void(ObjectType::* _Ptr)(const FrameAnimation_DESC&), ObjectType* _This)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Time = std::bind(_Ptr, _This, FrameAni[Name].Info);
	}

	// ����
	void AnimationBindStart(const std::string& _AnimationName, void(*_Ptr)(const FrameAnimation_DESC&))
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Start = std::bind(_Ptr, FrameAni[Name].Info);
	}
	// ������ �����ӿ� ���´�
	void AnimationBindEnd(const std::string& _AnimationName, void(*_Ptr)(const FrameAnimation_DESC&))
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].End = std::bind(_Ptr, FrameAni[Name].Info);
	}
	// �������� �ٲ𶧸��� ���´�
	void AnimationBindFrame(const std::string& _AnimationName, void(*_Ptr)(const FrameAnimation_DESC&))
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Frame = std::bind(_Ptr, FrameAni[Name].Info);
	}
	// Update
	void AnimationBindTime(const std::string& _AnimationName, void(*_Ptr)(const FrameAnimation_DESC&))
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Time = std::bind(_Ptr, FrameAni[Name].Info);
	}

protected:
	void Start() override;

	void SetTextureRendererSetting();

	void Update(float _Delta) override;

private:
	PIVOTMODE PivotMode;

	GameEngineTexture* CurTex;
	float4 FrameData;

	std::map<std::string, FrameAnimation> FrameAni;
	FrameAnimation* CurAni;

	void FrameDataReset();
};

