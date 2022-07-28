#include "PreCompile.h"
#include "GameEngineDepthStencilTexture.h"

GameEngineDepthStencilTexture::GameEngineDepthStencilTexture() 
{
}

GameEngineDepthStencilTexture::~GameEngineDepthStencilTexture() 
{
}


GameEngineDepthStencilTexture* GameEngineDepthStencilTexture::Create(const float4 _Scale)
{
	GameEngineDepthStencilTexture* Res = CreateResUnName();
	Res->CreateDepthTexture(_Scale);
	return Res;
}

void GameEngineDepthStencilTexture::CreateDepthTexture(const float4 _Scale)
{
	D3D11_TEXTURE2D_DESC Desc = {0};
	Desc.ArraySize = 1;
	Desc.Width = _Scale.uix();
	Desc.Height = _Scale.uiy();
	//             24비트 float + 8비트 unsigned int
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	// 할수 있다면 쓰겠다는것
	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	// cpu에서는 접근 못함
	Desc.CPUAccessFlags = 0;
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	Buffer = GameEngineTexture::Create(Desc);
}

ID3D11DepthStencilView* GameEngineDepthStencilTexture::CreateDepthStencilView()
{
	return Buffer->CreateDepthStencilView();
}