#pragma once
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShader.h"

// 설명 :
class GameEngineShader;
class GameEngineShaderResourcesHelper
{
	friend GameEngineShader;

// 리소스 세팅함수들 정리

	// 이 상수버퍼가 이녀석 안에 있어?


public:
	// constrcuter destructer
	GameEngineShaderResourcesHelper();
	~GameEngineShaderResourcesHelper();

	// delete Function
	GameEngineShaderResourcesHelper(const GameEngineShaderResourcesHelper& _Other) = delete;
	GameEngineShaderResourcesHelper(GameEngineShaderResourcesHelper&& _Other) noexcept = delete;
	GameEngineShaderResourcesHelper& operator=(const GameEngineShaderResourcesHelper& _Other) = delete;
	GameEngineShaderResourcesHelper& operator=(GameEngineShaderResourcesHelper&& _Other) noexcept = delete;

	void AllResourcesSetting();

	void AllResourcesReset();

	void ResourcesCheck(GameEngineRenderingPipeLine* _Line);

	// 상수버퍼 계열
	bool IsConstantBuffer(const std::string& _Name);

	template<typename Res>
	void SetConstantBufferLink(const std::string& _Name, const Res& Data) 
	{
		SetConstantBufferLink(_Name , &Data, sizeof(Res));
	}

	void SetConstantBufferLink(const std::string& _Name, const void* Data, UINT _Size);

	template<typename Res>
	void SetConstantBufferNew(const std::string& _Name, const Res& Data)
	{
		SetConstantBufferNew(_Name, &Data, sizeof(Res));
	}

	void SetConstantBufferNew(const std::string& _Name, const void* Data, UINT _Size);

	// 텍스처 계열
	bool IsTexture(const std::string& _Name);

	GameEngineTexture* SetTexture(const std::string& _Name, const std::string& _TextureName);

	GameEngineTexture* SetTexture(const std::string& _Name, GameEngineTexture* _TextureName);

	bool IsSampler(const std::string& _Name);

	GameEngineSampler* SetSampler(const std::string& _Name, const std::string& _TextureName);

	GameEngineSampler* SetSampler(const std::string& _Name, GameEngineSampler* _SamplerName);


protected:
	void ShaderCheck(GameEngineShader* _Shader);

private:
	// 컬러가 
	// 픽셀 쉐이더에서는 같은 Color(바이트 덩어리)인데 레드를 세팅해주고 싶고
	// 버텍스 쉐이더에서는 같은 Color(바이트 덩어리)인데 블루를 세팅해주고 싶고
	// 그건 아직 구분하지 않겠습니다.
	std::multimap<std::string, GameEngineConstantBufferSetter> ConstantBufferSettingMap;
	std::multimap<std::string, GameEngineTextureSetter> TextureSettingMap;
	std::multimap<std::string, GameEngineSamplerSetter> SamplerSettingMap;

	void BindConstantBuffer(GameEngineConstantBufferSetter& _Setter, GameEngineConstantBuffer* _Res);
	void BindSampler(GameEngineSamplerSetter& _Setter, GameEngineSampler* _Res);
	void BindTexture(GameEngineTextureSetter& _Setter, GameEngineTexture* _Res);
};

