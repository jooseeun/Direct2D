#pragma once
#include <GameEngineBase/GameEngineTransform.h>

namespace GameEngineDebug
{
	void Debug3DInitialize();

	void DrawBox(const GameEngineTransform& _Trans, const float4& _Color);
	void DrawBox(const GameEngineTransform& _Trans, GameEngineCamera* _Camera, const float4& _Color);
	// void DrawBox(float4 _Postion, float4 _Rot, float4 _Scale);
	void DrawSphere();
	void Debug3DRender();
}