#pragma once
#include <string>
#include <list>
#include <map>
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineNameObject.h>

// 설명 :
class GameEngineConstantBuffer :  public GameEngineNameObject
{
private:

public:
	static GameEngineConstantBuffer* Find(const std::string& _Name, int _ByteSize)
	{
		std::string UpperName = GameEngineString::ToUpperReturn(_Name);

		std::map<std::string, std::map<int, GameEngineConstantBuffer*>>::iterator NameIter = NamedRes.find(UpperName);

		if (NamedRes.end() == NameIter)
		{
			return nullptr;
		}

		std::map<int, GameEngineConstantBuffer*>& SizeMap = NameIter->second;

		std::map<int, GameEngineConstantBuffer*>::iterator SizeIter = SizeMap.find(_ByteSize);

		if (SizeIter == SizeMap.end())
		{
			return nullptr;
		}

		return SizeIter->second;
	}

	static GameEngineConstantBuffer* Create(
		const std::string& _Name, 
		D3D11_SHADER_BUFFER_DESC _Desc, 
		ID3D11ShaderReflectionConstantBuffer* _CBufferPtr
	)
	{
		GameEngineConstantBuffer* NewBuffer = CreateResName(_Name, _Desc.Size);

		NewBuffer->Create(_Desc, _CBufferPtr);

		return NewBuffer;
	}


	static GameEngineConstantBuffer* CreateAndFind(
		const std::string& _Name,
		D3D11_SHADER_BUFFER_DESC _Desc,
		ID3D11ShaderReflectionConstantBuffer* _CBufferPtr
	)
	{
		GameEngineConstantBuffer* FindBuffer = Find(_Name, _Desc.Size);

		if (nullptr != FindBuffer)
		{
			return FindBuffer;
		}

		GameEngineConstantBuffer* NewBuffer = CreateResName(_Name, _Desc.Size);

		NewBuffer->Create(_Desc, _CBufferPtr);

		return NewBuffer;
	}


	static void ResourcesDestroy()
	{
		//for (auto& Res : UnNamedRes)
		//{
		//	delete Res;
		//}

		for (auto& NameRes : NamedRes)
		{
			for (auto& SizeRes : NameRes.second)
			{
				delete SizeRes.second;
			}
		}
	}

protected:
	//              이름                바이트 사이즈

	// 상수버퍼에 이름이 없으면 
	static GameEngineConstantBuffer* CreateRes(const std::string& _Name)
	{

		GameEngineConstantBuffer* NewRes = new GameEngineConstantBuffer();
		NewRes->SetName(_Name);

		return NewRes;
	}

	static GameEngineConstantBuffer* CreateResName(const std::string& _Name, int _ByteSize)
	{
		std::string Name = GameEngineString::ToUpperReturn(_Name);

		GameEngineConstantBuffer* FindBuffer = Find(_Name, _ByteSize);

		if (nullptr != FindBuffer)
		{
			return FindBuffer;
		}

		GameEngineConstantBuffer* Res = CreateRes(Name);
		NamedRes[Name][_ByteSize] = Res;

		return Res;
	}

	//static ResType* CreateResUnName()
	//{
	//	ResType* Res = CreateRes();
	//	UnNamedRes.push_back(Res);
	//	return Res;
	//}

private:
	static std::map<std::string, std::map<int, GameEngineConstantBuffer*>> NamedRes;


public:
	// constrcuter destructer
	GameEngineConstantBuffer();
	~GameEngineConstantBuffer();

	// delete Function
	GameEngineConstantBuffer(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer(GameEngineConstantBuffer&& _Other) noexcept = delete;
	GameEngineConstantBuffer& operator=(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer& operator=(GameEngineConstantBuffer&& _Other) noexcept = delete;

	void ChangeData(const void* _Data, size_t _Size) const;

	void VSSetting(int _BindPoint);

	void PSSetting(int _BindPoint);

protected:

private:
	ID3D11Buffer* Buffer;
	D3D11_BUFFER_DESC BufferDesc;
	D3D11_SHADER_BUFFER_DESC ShaderDesc;
	// D3D11_MAPPED_SUBRESOURCE SettingResources;

	void Create(const D3D11_SHADER_BUFFER_DESC& _Desc, ID3D11ShaderReflectionConstantBuffer* _CBufferPtr);
};

