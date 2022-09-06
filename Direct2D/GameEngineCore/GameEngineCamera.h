#pragma once
#include "GameEngineTransformComponent.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineWindow.h>

enum class CAMERAPROJECTIONMODE
{
	PersPective,
	Orthographic,
};

class RenderingInstancing
{

};

// ���� :
class GameEngineLevel;
class GameEngineCamera;
class GameEngineRenderTarget;
class GameEngineRenderingPipeLine;
class GameEngineCamera : public GameEngineTransformComponent
{
	friend GameEngineLevel;
	friend GameEngineRenderer;

public:
	// constrcuter destructer
	GameEngineCamera();
	~GameEngineCamera();

	// delete Function
	GameEngineCamera(const GameEngineCamera& _Other) = delete;
	GameEngineCamera(GameEngineCamera&& _Other) noexcept = delete;
	GameEngineCamera& operator=(const GameEngineCamera& _Other) = delete;
	GameEngineCamera& operator=(GameEngineCamera&& _Other) noexcept = delete;

	inline CAMERAPROJECTIONMODE GetProjectionMode()
	{
		return Mode;
	}

	inline GameEngineRenderTarget* GetCameraRenderTarget()
	{
		return CameraRenderTarget;
	}

	void SetCameraOrder(CAMERAORDER _Order);

	inline void SetProjectionMode(CAMERAPROJECTIONMODE _Mode)
	{
		Mode = _Mode;
	}

	// ���� ���� 0,0
	float4 GetMouseScreenPosition();

	float4 GetMouseWorldPosition();

	float4 GetMouseWorldPositionToActor();

	inline float4x4 GetView() 
	{
		return View;
	}
	inline float4x4 GetProjectionMatrix()
	{
		return Projection;
	}

	inline float4 GetMouseWorldDir()
	{
		return MouseDir;
	}

	inline void SetProjectionSize(const float4& _Value) 
	{
		Size = _Value;
		// return float4();
	}

	inline float4 GetProjectionSize()
	{
		return Size;
	}

protected:
	void Start();

	void ChangeRenderingOrder(GameEngineRenderer* _Renderer, int _ChangeOrder);

private:
	void Render(float _DeltaTime);

	void PushRenderer(GameEngineRenderer* _Renderer);

	void Release(float _DelataTime);

	void Update(float _DeltaTime) override;

	void OverRenderer(GameEngineCamera* _NextOver);

	class GameEngineRenderTarget* CameraRenderTarget;

	std::map<int, std::list<class GameEngineRenderer*>> AllRenderer_;

	std::map<GameEngineRenderingPipeLine*, RenderingInstancing*> InstancingMap;

	float4x4 View; // �ٶ󺸴°�
	float4x4 Projection;
	float4x4 ViewPort;
	CAMERAPROJECTIONMODE Mode;

	float4 PrevMouse;
	float4 MouseDir;

	D3D11_VIEWPORT ViewPortDesc;

	// Projection Data
	float4 Size;
	float Near;
	float Far;

	// Perspective
	float Fov;
};

