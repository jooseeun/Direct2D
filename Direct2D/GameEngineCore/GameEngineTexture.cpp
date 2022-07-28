#include "PreCompile.h"
#include "GameEngineTexture.h"

GameEngineTexture::GameEngineTexture()
	: Texture2D(nullptr)
	, RenderTargetView(nullptr)
	, ShaderResourceView(nullptr)
	, Metadata()
{
}

GameEngineTexture::~GameEngineTexture()
{
	if (nullptr != DepthStencilView)
	{
		DepthStencilView->Release();
	}

	if (nullptr != ShaderResourceView)
	{
		ShaderResourceView->Release();
	}

	if (nullptr != RenderTargetView)
	{
		RenderTargetView->Release();
	}

	if (nullptr != Texture2D)
	{
		Texture2D->Release();
	}
}

ID3D11RenderTargetView* GameEngineTexture::CreateRenderTargetView()
{
	if (nullptr != RenderTargetView)
	{
		return RenderTargetView;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D, nullptr, &RenderTargetView))
	{
		MsgBoxAssert("랜더타겟 생성에 실패했습니다.");
	}

	return RenderTargetView;
}

ID3D11DepthStencilView* GameEngineTexture::CreateDepthStencilView()
{
	if (nullptr != DepthStencilView)
	{
		return DepthStencilView;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateDepthStencilView(Texture2D, nullptr, &DepthStencilView))
	{
		MsgBoxAssert("깊이 버퍼 세팅 실패했습니다.");
	}

	return DepthStencilView;
}

GameEngineTexture* GameEngineTexture::Create(ID3D11Texture2D* _Texture)
{
	GameEngineTexture* NewRes = CreateResUnName();
	NewRes->Texture2D = _Texture;
	_Texture->GetDesc(&NewRes->Desc);
	return NewRes;
}

GameEngineTexture* GameEngineTexture::Create(const D3D11_TEXTURE2D_DESC& _Desc)
{
	GameEngineTexture* NewRes = CreateResUnName();
	NewRes->TextureCreate(_Desc);
	return NewRes;
}

GameEngineTexture* GameEngineTexture::Create(const std::string& _Name, ID3D11Texture2D* _Texture)
{
	GameEngineTexture* NewRes = CreateResName(_Name);
	NewRes->Texture2D = _Texture;
	return NewRes;
}

GameEngineTexture* GameEngineTexture::Load(const std::string& _Path, const std::string& _Name)
{
	GameEngineTexture* NewRes = CreateResName(_Name);
	NewRes->TextureLoad(_Path);
	return NewRes;
}

void GameEngineTexture::TextureLoad(const std::string& _Path)
{
	std::string Ex = GameEngineString::ToUpperReturn(GameEnginePath::GetExtension(_Path));

	std::wstring LoadPath = GameEngineString::AnsiToUnicodeReturn(_Path);

	if (Ex == "TGA")
		// && S_OK != DirectX::LoadFromTGAFile(LoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, Image))
	{
		MsgBoxAssertString(_Path + "아직 처리하지 않은 이미지 포멧입니다.");
	}
	else if (Ex == "DDS")
		// && S_OK != DirectX::LoadFromDDSFile(LoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, Image))
	{
		MsgBoxAssertString(_Path + "아직 처리하지 않은 이미지 포멧입니다.");
	}
	else if (S_OK != DirectX::LoadFromWICFile(LoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, Image))
	{
		MsgBoxAssertString(_Path + "로드에 실패했습니다.");
	}

	// GameEngineDevice::GetDevice()->쉬운텍스처함수

	if (S_OK != DirectX::CreateShaderResourceView(
		GameEngineDevice::GetDevice(),
		Image.GetImages(),
		Image.GetImageCount(),
		Image.GetMetadata(),
		&ShaderResourceView
	))
	{
		MsgBoxAssertString(_Path + "쉐이더 리소스 생성에 실패했습니다.");
	}

	Desc.Width = static_cast<UINT>(Metadata.width);
	Desc.Height = static_cast<UINT>(Metadata.height);
}

void GameEngineTexture::VSSetting(int _BindPoint)
{
	if (nullptr == ShaderResourceView)
	{
		MsgBoxAssert("존재하지 않는 텍스처를 사용할 수 없습니다.");
	}

	GameEngineDevice::GetContext()->VSSetShaderResources(_BindPoint, 1, &ShaderResourceView);
}

void GameEngineTexture::PSSetting(int _BindPoint)
{
	if (nullptr == ShaderResourceView)
	{
		MsgBoxAssert("존재하지 않는 텍스처를 사용할 수 없습니다.");
	}

	GameEngineDevice::GetContext()->PSSetShaderResources(_BindPoint, 1, &ShaderResourceView);
}


void GameEngineTexture::Cut(const std::string& _Name, UINT _X, UINT _Y)
{
	GameEngineTexture* Texture = Find(_Name);

	if (nullptr == Texture)
	{
		MsgBoxAssert("존재하지 않는 텍스처를 자르려고 했습니다.");
		return;
	}

	Texture->Cut(_X, _Y);
}

void GameEngineTexture::Cut(UINT _X, UINT _Y)
{
	float SizeX = 1.0f / _X;
	float SizeY = 1.0f / _Y;

	float4 Start = float4::ZERO;

	for (size_t y = 0; y < _Y; y++)
	{
		for (size_t x = 0; x < _X; x++)
		{

			float4 FrameData;

			FrameData.PosX = Start.x;
			FrameData.PosY = Start.y;
			FrameData.SizeX = SizeX;
			FrameData.SizeY = SizeY;
			CutData.push_back(FrameData);
			Start.x += SizeX;
		}

		Start.x = 0.0f;
		Start.y += SizeY;
	}
}

void GameEngineTexture::TextureCreate(const D3D11_TEXTURE2D_DESC& _Desc)
{
	Desc = _Desc;

	GameEngineDevice::GetDevice()->CreateTexture2D(&Desc, nullptr, &Texture2D);

	if (nullptr == Texture2D)
	{
		MsgBoxAssert("텍스처 생성에 실패했습니다.");
		return;
	}
}

float4 GameEngineTexture::GetPixel(int _x, int _y)
{
	if (0 > _x)
	{
		return float4::ZERO;
	}

	if (0 > _y)
	{
		return float4::ZERO;
	}

	if (Image.GetMetadata().width <= _x)
	{
		return float4::ZERO;
	}

	if (Image.GetMetadata().height <= _y)
	{
		return float4::ZERO;
	}

	// 여러분들이 생각하기에 색깔이 이상하다고 생각하면
	DXGI_FORMAT Fmt = Image.GetMetadata().format;

	uint8_t* Color = Image.GetImages()->pixels;

	int Index = _y * static_cast<int>(Image.GetMetadata().width) + _x;
	Color = Color + (Index * 4);

	unsigned char R = Color[0];
	unsigned char G = Color[1];
	unsigned char B = Color[2];
	unsigned char A = Color[3];

	return float4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}