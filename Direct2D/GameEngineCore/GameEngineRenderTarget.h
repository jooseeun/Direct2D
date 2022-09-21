#pragma once
#include "GameEngineTexture.h"
#include <vector>
#include "GameEngineShaderResourcesHelper.h"
#include "GameEngineRenderer.h"

// Setting �������� ����׷���.

// Copy(������) �� �Լ��� ȣ���� ����Ÿ���� ������ ��� ����� �������� ������ �����ض�.

// Merge(������, ����) �� �Լ��� ȣ���� ����Ÿ�� ���� ���������� ������ ���� ������.

// Effect() �� ����Ÿ�ٿ� ���� ȿ���� ��.

// Cameras;
// �÷��� ī�޶�
// UI ī�޶�

// ������ ����
// �÷��� ī�޶� => �÷��� ����Ÿ�� ����
//    ���ΰ� ����

// ����Ʈ ����Ʈ��� �մϴ�.

// UI ī�޶� => UI ����Ÿ�� ����
//    �κ��丮 ��ư ���ñ� 

// �������. ����Ʈ ����Ʈ��� �մϴ�.

// ����� ����Ÿ��->Merge(�÷��� ����Ÿ��)
// ����� ����Ÿ��->Merge(UI ����Ÿ��)

// ����̽��� �ִ� ����� ����Ÿ��

class GameEnginePostEffect
{
private:
	bool IsUpdate_ = true;

public:
	bool IsUpdate() 
	{
		return IsUpdate_;
	}

	virtual void On()
	{
		IsUpdate_ = true;
	}

	virtual void Off()
	{
		IsUpdate_ = false;
	}

public:
	virtual void EffectInit() = 0;
	virtual void Effect(class GameEngineRenderTarget* _Render) = 0;

	virtual ~GameEnginePostEffect() 
	{

	}
};

// ���� :
class GameEngineStatusWindow;
class GameEngineDepthStencilTexture;
class GameEngineRenderTarget : public GameEngineRes <GameEngineRenderTarget>
{
	friend GameEngineStatusWindow;

public:
	static ID3D11RenderTargetView* PrevRenderTargetViews;
	static ID3D11DepthStencilView* PrevDepthStencilView;

	static void GetPrevRenderTarget();
	static void SetPrevRenderTarget();

public:
	// constrcuter destructer
	GameEngineRenderTarget();
	~GameEngineRenderTarget();

	// delete Function
	GameEngineRenderTarget(const GameEngineRenderTarget& _Other) = delete;
	GameEngineRenderTarget(GameEngineRenderTarget&& _Other) noexcept = delete;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _Other) = delete;
	GameEngineRenderTarget& operator=(GameEngineRenderTarget&& _Other) noexcept = delete;

	static GameEngineRenderTarget* Create(const std::string& _Name);

	static GameEngineRenderTarget* Create();

	void CreateRenderTargetTexture(ID3D11Texture2D* _Texture, float4 _Color);

	void CreateRenderTargetTexture(float4 _Size, float4 _Color);

	void CreateRenderTargetTexture(float4 _Size, DXGI_FORMAT _Format, float4 _Color);

	// ���� �ؽ�ó�� ������ ���
	void CreateRenderTargetTexture(D3D11_TEXTURE2D_DESC _Data, float4 _Color);

	void CreateRenderTargetTexture(GameEngineTexture* _Texture, float4 _Color);

	void SettingDepthTexture(GameEngineTexture* _Texture);

	void Clear();

	void Setting();

	void CreateDepthTexture(int _Index = 0);

	inline GameEngineTexture* GetDepthTexture() 
	{
		return DepthTexture;
	}

	GameEngineTexture* GetRenderTargetTexture(size_t _Index);

	void Copy(GameEngineRenderTarget* _Other, int _Index = 0);

	void Merge(GameEngineRenderTarget* _Other, int _Index = 0);
	
	void Effect(GameEngineRenderingPipeLine* _Other, GameEngineShaderResourcesHelper* _ShaderResourcesHelper);

	void Effect(class GameEngineRenderSet& _RenderSet);

	void EffectProcess();


protected:
	std::vector<GameEngineTexture*> RenderTargets;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews;
	std::vector<ID3D11ShaderResourceView*> ShaderResourceViews;
	std::vector<float4> ClearColors;

	// std::vector<ID3D11RenderTargetView*> RenderTargetViews;

	ID3D11DepthStencilView* DepthStencilView;

	GameEngineShaderResourcesHelper MergeShaderResourcesHelper;
	GameEngineRenderingPipeLine* MergePipeLine;

	GameEngineTexture* DepthTexture;

	// Post����Ʈ �κ�
private:
	std::list<GameEnginePostEffect*> Effects;

public:
	template<typename EffectType>
	EffectType* AddEffect()
	{
		GameEnginePostEffect* NewEffect = new EffectType();
		NewEffect->EffectInit();
		Effects.push_back(NewEffect);

		return reinterpret_cast<EffectType*>(NewEffect);
	}
};

