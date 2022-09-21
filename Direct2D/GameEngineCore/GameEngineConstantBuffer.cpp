#include "PreCompile.h"
#include "GameEngineConstantBuffer.h"

// ������۴� �̸��̶� ������� int
std::map<std::string, std::map<int, GameEngineConstantBuffer*>> GameEngineConstantBuffer::ConstantBufferRes;


GameEngineConstantBuffer::GameEngineConstantBuffer() 
		: Buffer(nullptr)
		, BufferDesc()
		, ShaderDesc()
{
}

GameEngineConstantBuffer::~GameEngineConstantBuffer() 
{
	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}
}



void GameEngineConstantBuffer::Create(const D3D11_SHADER_BUFFER_DESC& _Desc)
{
	ShaderDesc = _Desc;

	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.ByteWidth = ShaderDesc.Size;

	BufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferDesc, nullptr, &Buffer))
	{
		MsgBoxAssert("������� ������ �����߽��ϴ�.");
	}
}

void GameEngineConstantBuffer::ChangeData(const void* _Data, size_t _Size) const
{
	if (_Data == nullptr)
	{
		MsgBoxAssertString(GetNameCopy() + "  �����͸� ���������� �ʾҽ��ϴ�.");
	}

	if (BufferDesc.ByteWidth != _Size)
	{
		MsgBoxAssertString(GetNameCopy() + "  ��������� ����Ʈ ũ�Ⱑ ���� ���� �ʽ��ϴ�.");
	}

	static D3D11_MAPPED_SUBRESOURCE SettingResources = {};
	memset(&SettingResources, 0, sizeof(SettingResources));

	// � �׷��� ���ҽ��� �������� �ƹ��� �ǵ��� ���ϰ� ��.
	// �׷���ī�带 ������ ����ϴ�.
	GameEngineDevice::GetContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SettingResources);

	if (nullptr == SettingResources.pData)
	{
		MsgBoxAssert("�׷���ī�� ���ۿ� �������� ���߽��ϴ�..");
	}

	memcpy_s(SettingResources.pData, BufferDesc.ByteWidth, _Data, BufferDesc.ByteWidth);

	
	// ������ �ٽ� �ݾ���� �մϴ�.
	GameEngineDevice::GetContext()->Unmap(Buffer, 0);
}

void GameEngineConstantBuffer::VSSetting(int _BindPoint)
{
	GameEngineDevice::GetContext()->VSSetConstantBuffers(_BindPoint, 1, &Buffer);
}

void GameEngineConstantBuffer::PSSetting(int _BindPoint)
{
	GameEngineDevice::GetContext()->PSSetConstantBuffers(_BindPoint, 1, &Buffer);
}