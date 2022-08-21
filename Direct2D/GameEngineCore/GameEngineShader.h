#pragma once
#include <string>
#include <map>
#include <GameEngineBase/GameEngineNameObject.h>
#include <functional>


enum class ShaderType
{
	Vertex,
	Pixel,
	MAX,
};

class GameEngineShader;
class ShaderResSetter : public GameEngineNameObject
{
public:
	GameEngineShader* ParentShader;
	ShaderType ShaderType;
	int BindPoint;
	std::function<void()> SettingFunction;

public:
	ShaderResSetter() 
		: ShaderType(ShaderType::MAX)
		, BindPoint(-1)
		, ParentShader(nullptr)
	{

	}
};

class GameEngineConstantBuffer;
class GameEngineConstantBufferSetter : public ShaderResSetter
{
	friend GameEngineShader;

public:
	GameEngineConstantBuffer* Res;
	// ���ڰ� ���� ������ ���� �ּ�
	const void* SetData;
	UINT Size;


	// �ڱ�޸𸮷� �Ҵ��� ���̴�.
	std::vector<char> OriginalData;

	void Setting() const;

public:
	GameEngineConstantBufferSetter() 
		: Res(nullptr)
		, SetData(nullptr)
		, Size(-1)
	{

	}
};

class GameEngineTexture;
class GameEngineTextureSetter : public ShaderResSetter
{
	friend GameEngineShader;

public:
	void Setting() const;

public:
	GameEngineTexture* Res;
};

class GameEngineSampler;
class GameEngineSamplerSetter : public ShaderResSetter
{
public:
	void Setting() const;

public:
	GameEngineSampler* Res;
};


// ���� :
class GameEngineShaderResourcesHelper;
class GameEngineShader 
{
	friend GameEngineShaderResourcesHelper;

public:
	static void AutoCompile(const std::string& _Path);

public:
	// constrcuter destructer
	GameEngineShader();
	virtual ~GameEngineShader();

	// delete Function
	GameEngineShader(const GameEngineShader& _Other) = delete;
	GameEngineShader(GameEngineShader&& _Other) noexcept = delete;
	GameEngineShader& operator=(const GameEngineShader& _Other) = delete;
	GameEngineShader& operator=(GameEngineShader&& _Other) noexcept = delete;

	GameEngineConstantBufferSetter& GetConstantBufferSetter(std::string _Name);

protected:
	void CreateVersion(const std::string& _ShaderType, UINT _VersionHigh, UINT _VersionLow);
	void SetEntryPoint(const std::string& _EntryPoint)
	{
		EntryPoint = _EntryPoint;
	}

	ID3DBlob* BinaryPtr;

	std::string Version;

	void ShaderResCheck();

	ShaderType ShaderSettingType;

private:
	std::map<std::string, GameEngineConstantBufferSetter> ConstantBufferMap;
	std::map<std::string, GameEngineTextureSetter> TextureMap;
	std::map<std::string, GameEngineSamplerSetter> SamplerMap;

	std::string EntryPoint;

	// std::map<unsigned int, ConstantBuffer> 

	// void SetConstantBuffer() override;

};

