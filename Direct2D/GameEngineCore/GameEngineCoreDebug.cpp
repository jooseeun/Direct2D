#include "PreCompile.h"
#include "GameEngineCoreDebug.h"
#include <GameEngineBase/GameEngineTransform.h>
#include "GameEngineShaderResourcesHelper.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineCore.h"
#include "GameEngineCamera.h"
#include "GameEngineTexture.h"

namespace GameEngineDebug
{
	enum class DebugRenderType
	{
		Sphere,
		Box,
	};

	struct DebugInfo
	{
		int Type[4];
		float4 Color;

	public:
		DebugInfo(DebugRenderType _Type, const float4& _Color)
			: Type{ static_cast<int>(_Type) }
			, Color(_Color)
		{

		}
	};

	struct DebugRenderData
	{
	public:
		DebugInfo Info;
		TransformData Data;
		GameEngineTexture* Texture;
	};

	std::vector<DebugRenderData> DebugData = std::vector<DebugRenderData>();

	void DrawBox(const GameEngineTransform& _Trans, const float4& _Color)
	{
		DrawBox(_Trans, GameEngineCore::GetCurLevel()->GetMainCamera(), _Color);
	}

	void DrawTexture(const std::string& _Texture, const float4& _Pos, const float4& _Rot, const float4& _Scale /*= float4::ZERO*/)
	{
		DrawTexture(_Texture, GameEngineCore::GetCurLevel()->GetMainCamera(), _Pos, _Rot, _Scale);
	}

	void DrawTexture(const std::string& _Texture, GameEngineCamera* _Camera, const float4& _Pos, const float4& _Rot, const float4& _Scale /*= float4::ZERO*/)
	{
		static GameEngineTransform DebugTrans;

		GameEngineTexture* FindTexture = GameEngineTexture::Find(_Texture);
		if (nullptr == FindTexture)
		{
			MsgBoxAssert("존재하지 않는 텍스처를 디버그 랜더링 하려고 했습니다");
		}

		DebugTrans.SetLocalPosition(_Pos);
		DebugTrans.SetLocalRotate(_Rot);
		DebugTrans.SetLocalScale(_Scale);

		if (_Scale.CompareInt2D(float4::ZERO))
		{
			DebugTrans.SetLocalScale(FindTexture->GetScale());
		}

		DebugTrans.SetView(_Camera->GetView());
		DebugTrans.SetProjection(_Camera->GetProjectionMatrix());
		DebugTrans.CalculateWorldViewProjection();


		DebugData.push_back(DebugRenderData{ DebugInfo(DebugRenderType::Box, float4::WHITE) , DebugTrans.GetTransformData(), FindTexture });
	}

	void DrawBox(const GameEngineTransform& _Trans, GameEngineCamera* _Camera, const float4& _Color)
	{
		static GameEngineTransform DebugTrans;

		DebugTrans.Copy(_Trans);
		DebugTrans.SetView(_Camera->GetView());
		DebugTrans.SetProjection(_Camera->GetProjectionMatrix());
		DebugTrans.CalculateWorldViewProjection();

		DebugData.push_back(DebugRenderData{ DebugInfo(DebugRenderType::Box, _Color) , DebugTrans.GetTransformData(), nullptr });
	}

	void DrawSphere(const GameEngineTransform& _Trans, const float4& _Color)
	{
		DrawSphere(_Trans, GameEngineCore::GetCurLevel()->GetMainCamera(), _Color);
	}
	void DrawSphere(const GameEngineTransform& _Trans, GameEngineCamera* _Camera, const float4& _Color)
	{
		static GameEngineTransform DebugTrans;

		DebugTrans.Copy(_Trans);
		DebugTrans.SetView(_Camera->GetView());
		DebugTrans.SetProjection(_Camera->GetProjectionMatrix());
		DebugTrans.CalculateWorldViewProjection();

		DebugData.push_back(DebugRenderData{ DebugInfo(DebugRenderType::Sphere, _Color) , DebugTrans.GetTransformData() });
	}


	GameEngineShaderResourcesHelper DebugShaderResources;
	GameEngineRenderingPipeLine* DebugRenderingPipeLine;

	GameEngineShaderResourcesHelper TextureShaderResources;
	GameEngineRenderingPipeLine* TextureRenderingPipeLine;

	void Debug3DInitialize()
	{
		static bool IsOnce = false;

		if (true == IsOnce)
		{
			return;
		}


		DebugRenderingPipeLine = GameEngineRenderingPipeLine::Find("3DDebug");
		DebugShaderResources.ResourcesCheck(DebugRenderingPipeLine);

		TextureRenderingPipeLine = GameEngineRenderingPipeLine::Find("DebugTexture");
		TextureShaderResources.ResourcesCheck(TextureRenderingPipeLine);

		IsOnce = true;
	}


	void Debug3DRender()
	{
		for (size_t i = 0; i < DebugData.size(); i++)
		{
			if (nullptr == DebugData[i].Texture)
			{
				DebugShaderResources.SetConstantBufferLink("TransformData", DebugData[i].Data);
				DebugShaderResources.SetConstantBufferLink("DebugInfo", DebugData[i].Info);
				DebugShaderResources.AllResourcesSetting();
				DebugRenderingPipeLine->Rendering();
			}
			else
			{
				TextureShaderResources.SetConstantBufferLink("TransformData", DebugData[i].Data);
				TextureShaderResources.SetConstantBufferLink("DebugInfo", DebugData[i].Info);
				TextureShaderResources.SetTexture("Tex", DebugData[i].Texture);
				TextureShaderResources.AllResourcesSetting();
				TextureRenderingPipeLine->Rendering();
			}
		}
		// DebugData.push_back(DebugRenderData());

		DebugData.clear();
		int a = 0;
	}

};