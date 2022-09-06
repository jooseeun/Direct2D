#include "PreCompile.h"
#include "GameEngineDevice.h"
#include <GameEngineBase/GameEngineWindow.h>
#include "GameEngineRenderTarget.h"

ID3D11Device* GameEngineDevice::Device_ = nullptr;
ID3D11DeviceContext* GameEngineDevice::Context_ = nullptr;
IDXGISwapChain* GameEngineDevice::SwapChain_ = nullptr;
GameEngineRenderTarget* GameEngineDevice::BackBufferTarget = nullptr;

void GameEngineDevice::Destroy()
{
	if (nullptr != SwapChain_)
	{
		SwapChain_->Release();
		SwapChain_ = nullptr;
	}

	if (nullptr != Context_)
	{
		Context_->Release();
		Context_ = nullptr;
	}

	if (nullptr != Device_)
	{
		Device_->Release();
		Device_ = nullptr;
	}
}

IDXGIAdapter* GameEngineDevice::GetHighPerformanceAdapter() 
{
	/*
* typedef struct DXGI_ADAPTER_DESC
  {
	WCHAR Description[ 128 ];			//	����� ����
	UINT VendorId;						//	�ϵ���� ���޾�ü�� PCI ID
	UINT DeviceId;						//	�ϵ���� ��ġ�� PCI ID
	UINT SubSysId;						//	����ý����� PCI ID
	UINT Revision;						//	����� ����(revision) ��ȣ�� PCI ID
	SIZE_T DedicatedVideoMemory;		//	CPU�� �������� �ʴ� ���� ���� �޸��� ����Ʈ ��
	SIZE_T DedicatedSystemMemory;		//	CPU�� �������� �ʴ� ���� �ý��� �޸� ����Ʈ ��
	SIZE_T SharedSystemMemory;			//	���� �ý��� �޸��� ����Ʈ ��
	LUID AdapterLuid;					//	����͸� �ĺ��ϴ� ������(Locally Unique Identifier)
   } 	DXGI_ADAPTER_DESC;
*/
	IDXGIFactory* pF = nullptr;
	IDXGIAdapter* pA = nullptr;
	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pF);

	size_t prevAdapterVideoMemory = 0;

	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		IDXGIAdapter* pA1 = nullptr;
		pF->EnumAdapters(adapterIndex, &pA1);
		if (nullptr == pA1)
		{
			break;
		}

		DXGI_ADAPTER_DESC adapterDesc;
		pA1->GetDesc(&adapterDesc);
		if (prevAdapterVideoMemory <= adapterDesc.DedicatedVideoMemory)
		{
			prevAdapterVideoMemory = adapterDesc.DedicatedVideoMemory;
			if (nullptr != pA)
			{
				pA->Release();
			}
			pA = pA1;
			continue;
		}

		pA1->Release();
	}

	pF->Release();
	return pA;
}

void GameEngineDevice::DeviceCreate()
{
	if (nullptr == GameEngineWindow::GetHWND())
	{
		MsgBoxAssert("�����찡 ��������� �ʾҴµ� ����̽��� �ʱ�ȭ �Ϸ��� �߽��ϴ�.");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 
	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	IDXGIAdapter* pA = GetHighPerformanceAdapter();

	// soft
	if (
		S_OK != D3D11CreateDevice(
			// ���� �ڵ��� �׷���ī�� ����̹� ����� �ֳ���?
			pA,
			// �Ϲ����� ǥ���Ͽ��� ��������� �׷���ī�带 ����Ұ̴ϴ�.
			D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
			// �Ϲ����� ǥ���Ͽ��� ��������� �׷���ī�带 ����Ұ̴ϴ�.
			// D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			iFlag,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&Device_,
			&Level,
			&Context_
		)
		)
	{
		MsgBoxAssert("����̽� ������ �����߽��ϴ�.");
	}

	if (nullptr != pA)
	{
		pA->Release();
		pA = nullptr;
	}

	if (Level != D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0)
	{
		MsgBoxAssert("11�� �������� �ʴ� ����̽� �Դϴ�.");
	}

	// ��Ƽ������ �ε��� �ɼ�.
	HRESULT Hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
}

void GameEngineDevice::CreateSwapChain()
{
	float4 ScreenSize = GameEngineWindow::GetScale();

	// ���̷�Ʈ�� �׷���ī�忡 ���� ����� ���� �����ϱ� ���� ������ �ѱ�°�
	// �̰� �׷���ī�忡 �� �޸𸮸� ����ų�
	// ���� ����� ����.
	DXGI_SWAP_CHAIN_DESC ScInfo = {0,};

	ScInfo.BufferCount = 2;
	ScInfo.BufferDesc.Width = ScreenSize.uix();
	ScInfo.BufferDesc.Height = ScreenSize.uiy();
	ScInfo.OutputWindow = GameEngineWindow::GetHWND();

	ScInfo.BufferDesc.RefreshRate.Denominator = 1;
	ScInfo.BufferDesc.RefreshRate.Numerator = 60;

	ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	ScInfo.SampleDesc.Quality = 0;
	ScInfo.SampleDesc.Count = 1;

	ScInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;

	ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ScInfo.Windowed = true;

	// ����ü���� �������
	// �⺻������ ���丮��°� ����ų� ���;� �ϴµ�.
	// ����� ����� �ƴϰ�. 

	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	// �� ���α׷� ������ ����� ��ġ�� ���� �ϳ��� ���� ������ �̴ϴ�.
	// "54ec77fa-1377-44e6-8c32-88fd5f44c84c"
	// ����� ��ġ��
	// ���̷�Ʈ�� �����ϴ� �������̽��� �����͵��� ���۷��� ī��Ʈ ����� ����ϴµ�.
	// Comptr�̳� �̷� ����Ŭ������ ������ ���� ���� Release�� ȣ������� �Ѵ�.
	Device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pD));
	if (nullptr == pD)
	{
		MsgBoxAssert("����̽� ���⿡ �����߽��ϴ�.");
	}

	pD->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pA));
	if (nullptr == pA)
	{
		MsgBoxAssert("��� ���⿡ �����߽��ϴ�.");
	}

	pA->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));
	if (nullptr == pF)
	{
		MsgBoxAssert("���丮 ���⿡ �����߽��ϴ�.");
	}

	if (S_OK != pF->CreateSwapChain(Device_, &ScInfo, &SwapChain_))
	{
		MsgBoxAssert("����ü�� ������ �����߽��ϴ�.");
	}

	pF->Release();
	pA->Release();
	pD->Release();

	ID3D11Texture2D* BackBufferTexture = nullptr;
	if (S_OK != SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBufferTexture)))
	{
		MsgBoxAssert("����� �ؽ�ó�� ������ ���߽��ϴ�.");
	}

	BackBufferTarget = GameEngineRenderTarget::Create("BackBuffer");
	BackBufferTarget->CreateRenderTargetTexture(BackBufferTexture, float4::BLUE);
	BackBufferTarget->CreateDepthTexture(0);
}

void GameEngineDevice::Initialize() 
{
	DeviceCreate();
	// CreateSwapChain();
}

void GameEngineDevice::RenderStart()
{
	// �����
	BackBufferTarget->Clear();

	// �����ϰ�
	BackBufferTarget->Setting();
}

void GameEngineDevice::RenderEnd()
{
	// ����Ϳ� ������ �ض�.
	HRESULT Result = SwapChain_->Present(0, 0);
	if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
	{
		MsgBoxAssert("����̽� ������Ʈ�� �̻��� ������ϴ�.");
	}
}
