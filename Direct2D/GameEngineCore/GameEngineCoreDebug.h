#pragma once
#include <GameEngineBase/GameEngineTransform.h>

namespace GameEngineDebug
{
	void Debug3DInitialize();

	void DrawBox(const GameEngineTransform& _Trans, const float4& _Color);
	void DrawBox(const GameEngineTransform& _Trans, GameEngineCamera* _Camera, const float4& _Color);

	void DrawTexture(const std::string& _Texture, const float4& _Pos, const float4& _Rot, const float4& _Scale = float4::ZERO);
	void DrawTexture(const std::string& _Texture, GameEngineCamera* _Camera, const float4& _Pos, const float4& _Rot, const float4& _Scale = float4::ZERO);

	void DrawSphere(const GameEngineTransform& _Trans, const float4& _Color);
	void DrawSphere(const GameEngineTransform& _Trans, GameEngineCamera* _Camera, const float4& _Color);

	void Debug3DRender();
}