#include "PreCompile.h"
#include "GameEngineRenderingPipeLine.h"

#include "GameEngineInputLayOut.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineRasterizer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineDepthStencil.h"
#include "GameEngineBlend.h"


GameEngineRenderingPipeLine::GameEngineRenderingPipeLine() 
	: InputLayOut(nullptr)
	, VertexBuffer(nullptr)
	, VertexShader(nullptr)
	, IndexBuffer(nullptr)
	, Rasterizer(nullptr)
	, PixelShader(nullptr)
	, DepthStencil(nullptr)
	, Blend(nullptr)
	, Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	VertexBuffer = GameEngineVertexBuffer::Find("rect");
	IndexBuffer = GameEngineIndexBuffer::Find("rect");
	Rasterizer = GameEngineRasterizer::Find("EngineRasterizer");
	Blend = GameEngineBlend::Find("AlphaBlend");
	DepthStencil = GameEngineDepthStencil::Find("EngineBaseDepth");
}

GameEngineRenderingPipeLine::~GameEngineRenderingPipeLine() 
{
	// �ٸ��ֵ��� �����͸� ���� ���� ������ �����ϸ� �ȵ�����
	// InputLayOut�� �ڽŽ����� new�� �ϰ� �ڱ� ������ ������ �մϴ�.
	if (nullptr != InputLayOut)
	{
		delete InputLayOut;
		InputLayOut = nullptr;
	}
}

//
//void GameEngineRenderingPipeLine::SetInputAssembler1InputLayOutSetting(const std::string& _Name)
//{
//	InputLayOut = GameEngineInputLayOut::Find(_Name);
//
//	if (nullptr == InputLayOut)
//	{
//		MsgBoxAssert("�������� �ʴ� ��ǲ���̾ƿ��� �����Ϸ��� �߽��ϴ�.");
//		return;
//	}
//}

GameEngineRenderingPipeLine* GameEngineRenderingPipeLine::Create(const std::string& _Name)
{
	return CreateResName(_Name);
}

void GameEngineRenderingPipeLine::SetInputAssembler1VertexBuffer(const std::string& _Name) 
{
	VertexBuffer = GameEngineVertexBuffer::Find(_Name);

	if (nullptr == VertexBuffer)
	{
		MsgBoxAssert("�������� �ʴ� ���ؽ� ���۸� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	if (nullptr != InputLayOut)
	{
		delete InputLayOut;
		InputLayOut = nullptr;
	}

	if (nullptr == InputLayOut && nullptr != VertexShader)
	{
		InputLayOut = new GameEngineInputLayOut();
		InputLayOut->Create(*VertexBuffer->GetLayOutDesc(), VertexShader);
	}
}

void GameEngineRenderingPipeLine::SetVertexShader(const std::string& _Name)
{
	VertexShader = GameEngineVertexShader::Find(_Name);

	if (nullptr == VertexShader)
	{
		MsgBoxAssert("�������� �ʴ� ���ؽ� ���̴��� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	if (nullptr != InputLayOut)
	{
		delete InputLayOut;
		InputLayOut = nullptr;
	}

	// ��ǲ���̾ƿ��� ��������� �ʾҴµ�.
	if (nullptr == InputLayOut && nullptr != VertexBuffer)
	{
		InputLayOut = new GameEngineInputLayOut();
		InputLayOut->Create(*VertexBuffer->GetLayOutDesc(), VertexShader);
	}
}

void GameEngineRenderingPipeLine::SetInputAssembler2IndexBuffer(const std::string& _Name) 
{
	IndexBuffer = GameEngineIndexBuffer::Find(_Name);

	if (nullptr == IndexBuffer)
	{
		MsgBoxAssert("�������� �ʴ� �ε������۸� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetRasterizer(const std::string& _Name)
{
	Rasterizer = GameEngineRasterizer::Find(_Name);

	if (nullptr == Rasterizer)
	{
		MsgBoxAssert("�������� �ʴ� �����Ͷ����� ���ø� �����Ϸ��� �߽��ϴ�.");
		return;
	}

}


void GameEngineRenderingPipeLine::SetPixelShader(const std::string& _Name)
{
	PixelShader = GameEnginePixelShader::Find(_Name);

	if (nullptr == PixelShader)
	{
		MsgBoxAssert("�������� �ʴ� �ȼ����̴��� �����Ϸ��� �߽��ϴ�.");
		return;
	}

}



void GameEngineRenderingPipeLine::SetOutputMergerDepthStencil(const std::string& _Name)
{
	DepthStencil = GameEngineDepthStencil::Find(_Name);

	if (nullptr == DepthStencil)
	{
		MsgBoxAssert("�������� �ʴ� ���̽��ٽǸ� �����Ϸ��� �߽��ϴ�.");
		return;
	}

}


void GameEngineRenderingPipeLine::SetOutputMergerBlend(const std::string& _Name)
{
	Blend = GameEngineBlend::Find(_Name);

	if (nullptr == Blend)
	{
		MsgBoxAssert("�������� �ʴ� ������ �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeLine::Rendering()
{
	InputAssembler1VertexBufferSetting();

	VertexShaderSetting();

	InputAssembler2IndexBufferSetting();

	RasterizerSetting();

	PixelShaderSetting();

	OutputMergerBlendSetting();

	OutputMergerDepthStencilSetting();

	Draw();

}

// ���������� ������ ������ �״��� �߿����� �ʴ�.

void GameEngineRenderingPipeLine::InputAssembler1VertexBufferSetting()
{
	// �׷��ȸ��ҽ��� Setting�̶�� �Լ��� �����Ѵٸ�
	// �װ� �������� �� �������� ������ ������������ ���ư��� �ȴٴ� ���� �˴ϴ�.
	InputLayOut->Setting();
	// ���ؽ� ���۴� �����Ұ� ����.
	VertexBuffer->Setting();
}

void GameEngineRenderingPipeLine::VertexShaderSetting() 
{
	VertexShader->Setting();
	// ��ġ 
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void GameEngineRenderingPipeLine::InputAssembler2IndexBufferSetting() 
{
	GameEngineDevice::GetContext()->IASetPrimitiveTopology(Topology);

	IndexBuffer->Setting();
}

void GameEngineRenderingPipeLine::RasterizerSetting() 
{
	Rasterizer->Setting();
}

void GameEngineRenderingPipeLine::PixelShaderSetting() 
{
	PixelShader->Setting();
}

void GameEngineRenderingPipeLine::OutputMergerBlendSetting() 
{
	Blend->Setting();
}

void GameEngineRenderingPipeLine::OutputMergerDepthStencilSetting() 
{
	DepthStencil->Setting();
}


void GameEngineRenderingPipeLine::Draw()
{

	GameEngineDevice::GetContext()->DrawIndexed(IndexBuffer->GetIndexCount(), 0, 0);
}