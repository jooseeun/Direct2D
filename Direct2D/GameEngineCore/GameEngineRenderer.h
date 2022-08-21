#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineLevel.h"

struct RenderOption 
{
     float DeltaTime;
     int IsMask;
     int IsJump;
     int Option00;
     int Option01;
     int Option02;
     int Option03;
     int Option04;
     int Option05;
     int Option06;
     int Option07;
     int Option08;
     int Option09;
     int Option10;
     int Option11;
     int Option12;
     int Option13;
     int Option14;
     int Option15;
     int Option16;
     int Option17;
     int Option18;
     int Option19;
     int Option20;
};

// Ό³Έν :
class GameEngineRenderingPipeLine;
class GameEngineRenderer : public GameEngineTransformComponent
{
	friend class GameEngineLevel;
	friend class GameEngineCamera;

public:
	RenderOption Option;

	// constrcuter destructer
	GameEngineRenderer();
	~GameEngineRenderer();

	// delete Function
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

	// float4x4 ViewPort;
	void ChangeCamera(CAMERAORDER _Order);

    GameEngineRenderingPipeLine* GetClonePipeLine(GameEngineRenderingPipeLine* _Rendering);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime) {}
	virtual void End() {}

	void PushRendererToMainCamera();
	void PushRendererToUICamera();


	CAMERAORDER CameraOrder;


private:
	virtual void Render(float _DeltaTime) = 0;

};

