#include "PreCompile.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineDepthStencilTexture.h"

ID3D11RenderTargetView* GameEngineRenderTarget::PrevRenderTargetViews = nullptr;
ID3D11DepthStencilView* GameEngineRenderTarget::PrevDepthStencilView = nullptr;

GameEngineRenderTarget::GameEngineRenderTarget()
	: DepthStencilView(nullptr)
{
}

GameEngineRenderTarget::~GameEngineRenderTarget()
{
}

void GameEngineRenderTarget::GetPrevRenderTarget()
{
	GameEngineDevice::GetContext()->OMGetRenderTargets(1, &PrevRenderTargetViews, &PrevDepthStencilView);
}

void GameEngineRenderTarget::SetPrevRenderTarget()
{
	GameEngineDevice::GetContext()->OMSetRenderTargets(1, &PrevRenderTargetViews, PrevDepthStencilView);
	if (nullptr != PrevRenderTargetViews)
	{
		PrevRenderTargetViews->Release();
	}

	if (nullptr != PrevDepthStencilView)
	{
		PrevDepthStencilView->Release();
	}
}

//GameEngineRenderTarget* GameEngineRenderTarget::Create(const std::string& _Name, ID3D11Texture2D* _Texture) 
//{
//
//}

GameEngineRenderTarget* GameEngineRenderTarget::Create(const std::string& _Name)
{
	return CreateResName(_Name);
}

GameEngineRenderTarget* GameEngineRenderTarget::Create()
{
	return CreateResUnName();
}

void GameEngineRenderTarget::CreateRenderTargetTexture(ID3D11Texture2D* _Texture, float4 _Color)
{
	GameEngineTexture* NewTexture = GameEngineTexture::Create(_Texture);
	CreateRenderTargetTexture(NewTexture, _Color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(float4 _Size, float4 _Color)
{
	CreateRenderTargetTexture(_Size, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, _Color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(float4 _Size, DXGI_FORMAT _Format, float4 _Color)
{
	D3D11_TEXTURE2D_DESC NewData = { 0 };
	NewData.ArraySize = 1; // �ѹ��� 10��¥���� ����� �־��
	NewData.Width = _Size.uix();
	NewData.Height = _Size.uiy();
	NewData.Format = _Format;
	NewData.SampleDesc.Count = 1;
	NewData.SampleDesc.Quality = 0;
	NewData.MipLevels = 1;
	NewData.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	//                  ���⿡�ٰ� �������� �Ҽ��� �ְ�               �̰� ���̴����� ����Ҽ��� �ִٴ� �̴ϴ�.
	NewData.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

	CreateRenderTargetTexture(NewData, _Color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(GameEngineTexture* _Texture, float4 _Color)
{
	RenderTargets.push_back(_Texture);
	RenderTargetViews.push_back(_Texture->CreateRenderTargetView());
	ShaderResourceViews.push_back(_Texture->CreateShaderResourceView());
	ClearColors.push_back(_Color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(D3D11_TEXTURE2D_DESC _Data, float4 _Color)
{
	GameEngineTexture* NewTexture = GameEngineTexture::Create(_Data);
	CreateRenderTargetTexture(NewTexture, _Color);
}

void GameEngineRenderTarget::Clear()
{
	for (size_t i = 0; i < RenderTargetViews.size(); i++)
	{
		GameEngineDevice::GetContext()->ClearRenderTargetView(RenderTargetViews[i], ClearColors[i].Arr1D);
	}

	if (nullptr != DepthStencilView)
	{
		GameEngineDevice::GetContext()->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GameEngineRenderTarget::CreateDepthTexture(int _Index)
{
	DepthTexture = GameEngineDepthStencilTexture::Create(RenderTargets[_Index]->GetScale());

	DepthStencilView = DepthTexture->CreateDepthStencilView();
}

void GameEngineRenderTarget::Setting()
{
	// int* NewInt = new int[10];

	// GameEngineRenderTarget** NewInt = new GameEngineRenderTarget*[10]

	if (0 == RenderTargetViews.size())
	{
		MsgBoxAssert("����Ÿ�ٺ䰡 �������� �ʴ� ����Ÿ���� �����Ϸ��� �߽��ϴ�.");
	}

	GameEngineDevice::GetContext()->OMSetRenderTargets(static_cast<UINT>(RenderTargetViews.size()), &RenderTargetViews[0], DepthStencilView);
}