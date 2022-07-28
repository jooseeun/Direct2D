#pragma once
#include "GameEngineRes.h"


// ���� :
class GameEngineInputLayOut;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEngineIndexBuffer;
class GameEngineRasterizer;
class GameEnginePixelShader;
class GameEngineDepthStencil;
class GameEngineBlend;
class GameEngineRenderingPipeLine : public GameEngineRes<GameEngineRenderingPipeLine>
{
public:
	static GameEngineRenderingPipeLine* Create(const std::string& _Name);

public:
	// constrcuter destructer
	GameEngineRenderingPipeLine();
	~GameEngineRenderingPipeLine();

	// delete Function
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _Other) = delete;
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _Other) noexcept = delete;
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _Other) = delete;
	GameEngineRenderingPipeLine& operator=(GameEngineRenderingPipeLine&& _Other) noexcept = delete;

	// void SetInputAssembler1InputLayOutSetting(const std::string& _Name);

	// ������ �����Ϸ��� ���� �ʿ�����?
	// ���� �̷���� ���� �ʿ�����?
	// ���ҽ��� ���� n���� ���۶�� �������� �׷���ī������ ���ִ°Ŵ�.

	// �� ���� ����� �������� ���� ������
	// WinApi�� �� ��ǥ��� ���� �Ѵ�.

	// ���� ������ �˷��ִ� ��ǲ���̾ƿ��� �� �ܰ迡�� ���õȴ�.
	void SetInputAssembler1VertexBuffer(const std::string& _Name);

	// ���ؽ� ���̴�
	// �׷��� �־��� ���� �̵����Ѿ� ���� ���ϴ� ���� ������ ������ �ִ�.
	// �� �ϳ��ϳ��� ���ϴ� ��ġ�� �ű��.
	// �� ���� ��������� ��ġ�� �����Ҽ� �ִ� �ܰ�
	void SetVertexShader(const std::string& _Name);

	// �׷��� �ؼ� ��ȯ�� ���� ��� �����ؼ� ������ ������̳ĸ� ���⼭ ó���մϴ�.
	// �ε��� ���۰� ���õȴ�.
	void SetInputAssembler2IndexBuffer(const std::string& _Name);

	// �׸��� ���� �� ������ ��¥ ����� ��ġ�� �Ű��ִ� ������ 
	void SetRasterizer(const std::string& _Name);

	void SetPixelShader(const std::string& _Name);

	void SetOutputMergerBlend(const std::string& _Name);

	void SetOutputMergerDepthStencil(const std::string& _Name);

	void Rendering();

	inline GameEngineVertexShader* GetVertexShader() 
	{
		return VertexShader;
	}

	inline GameEnginePixelShader* GetPixelShader()
	{
		return PixelShader;
	}

protected:

	
private:
	GameEngineInputLayOut* InputLayOut; // ��ǲ�����1 ����
	GameEngineVertexBuffer* VertexBuffer; // ��ǲ�����1 ����

	// HLSL�̶�� �ٸ� �� ����ϴ� �ܰ迡��.
	// HLSL�� �׷���ī�忡 Ư�� �����Ϳ� ���� ó�� �ܰ踦 �츮�� ���� �ۼ��ؼ�
	// �׷���ī�忡 �÷��ټ� �ִ� �̴ϴ�.

	// ������ �̷��� ó����.
	GameEngineVertexShader* VertexShader; // ���ؽ����̴� ����

	GameEngineIndexBuffer* IndexBuffer; // ��ǲ�����2 ����

	D3D11_PRIMITIVE_TOPOLOGY Topology;// ��ǲ�����2 ����

	GameEngineRasterizer* Rasterizer; // �����Ͷ����� ����

	// �ȼ��� �̷��� ó����.
	GameEnginePixelShader* PixelShader; // �ȼ����̴� ����

	GameEngineDepthStencil* DepthStencil; // �ƿ�ǲ���� ���� ������ ���� 

	GameEngineBlend* Blend; // �ƿ�ǲ���� �������� ���� 



	// �ƿ�ǲ ������ ����Ÿ�ټ����� �̹� �س���.
	// ����̽��� ����۸� ����ؼ� ���� ����Ÿ���� �Ź� ���õǰ� �ִ�.
	// �ű⿡ �׷���.

	// ���� �������ش�.

	// �Ʒ��� �Լ����� ����
	void InputAssembler1VertexBufferSetting();

	void VertexShaderSetting();

	void InputAssembler2IndexBufferSetting();

	void RasterizerSetting();

	void PixelShaderSetting();

	void OutputMergerBlendSetting();

	void OutputMergerDepthStencilSetting();

	void Draw();

};


