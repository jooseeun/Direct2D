#pragma once
#include "GameEngineRenderTarget.h"

// Ό³Έν :
class GameEngineTexture;
class GameEngineDepthStencilTexture : public GameEngineRes<GameEngineDepthStencilTexture>
{
public:
	static GameEngineDepthStencilTexture* Create(const float4 _Scale);


public:
	// constrcuter destructer
	GameEngineDepthStencilTexture();
	~GameEngineDepthStencilTexture();

	// delete Function
	GameEngineDepthStencilTexture(const GameEngineDepthStencilTexture& _Other) = delete;
	GameEngineDepthStencilTexture(GameEngineDepthStencilTexture&& _Other) noexcept = delete;
	GameEngineDepthStencilTexture& operator=(const GameEngineDepthStencilTexture& _Other) = delete;
	GameEngineDepthStencilTexture& operator=(GameEngineDepthStencilTexture&& _Other) noexcept = delete;

	ID3D11DepthStencilView* CreateDepthStencilView();

protected:

private:
	GameEngineTexture* Buffer;

	void CreateDepthTexture(const float4 _Scale);

};

