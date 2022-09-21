#include "PreCompile.h"
#include "GameEngineStructuredBuffer.h"

// 데이터 1개의 사이즈.
std::map<std::string, std::map<int, GameEngineStructuredBuffer*>> GameEngineStructuredBuffer::StructuredBufferRes;


GameEngineStructuredBuffer::GameEngineStructuredBuffer() 
	: Buffer(nullptr)
	,BufferDesc()
	,ShaderDesc()
	,SettingResources()
	,SRV(nullptr)
	, DataCount(0)
{
}

GameEngineStructuredBuffer::~GameEngineStructuredBuffer() 
{
	Release();
}

void GameEngineStructuredBuffer::Release() 
{
	if (nullptr != SRV)
	{
		SRV->Release();
		SRV = nullptr;
	}

	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}
}


void GameEngineStructuredBuffer::CreateResize(const D3D11_SHADER_BUFFER_DESC& _Desc, int Count, void* _StartData)
{
	ShaderDesc = _Desc;

	if (0 == Count)
	{
		return;
	}

	if (DataCount >= Count)
	{
		return;
	}

	Release();

	BufferDesc.ByteWidth = ShaderDesc.Size * DataCount; // GPU 에 생성할 구조화 버퍼 메모리 크기(최소단위 ??)
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferDesc.StructureByteStride = ShaderDesc.Size; // 1개 크기도 넣어줘야 한다.

	D3D11_SUBRESOURCE_DATA StartData;
	if (nullptr != _StartData)
	{
		StartData.SysMemPitch = 0;
		StartData.SysMemSlicePitch = 0;
		StartData.pSysMem = _StartData;
	}


	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferDesc, &StartData, &Buffer))
	{
		MsgBoxAssert("스트럭처드 버퍼 생성에 실패했습니다.");
	}

	// Shader Resource View
	// 쉐이더 리소스를 만드는데 버퍼기반 
	// D3D11_SRV_DIMENSION_BUFFEREX 버퍼기반 쉐이더 리소스 뷰라는것을 알려주기 위한 용도
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = DataCount; // 10개로 우리어진 m_pSRV를 넣어달라.

	if (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(Buffer, &tSRVDesc, &SRV))
	{
		MsgBoxAssert("FAIL (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &m_pSRV))");
	}
}