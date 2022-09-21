#include "PreCompile.h"
#include "GameEngineCamera.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include <GameEngineBase/GameEngineWindow.h>

// �ּ� 2���̻��� �𿩾� �ν��Ͻ��� �����ϰڴ�.
int RenderingInstancing::MinInstancingCount = 2;

// �ϴ� �����ߴٸ� 100���� �Ҽ� �ִٴ� �����Ͽ� �̰� �ϰڴ�.
int RenderingInstancing::StartInstancingCount = 100;

GameEngineCamera::GameEngineCamera() 
{
	// �����찡 �����е� �����ϱ� ���� ���� �����̶� ���⼭ �ϴ°Ű�.
	Size = GameEngineWindow::GetInst()->GetScale();
	Mode = CAMERAPROJECTIONMODE::PersPective;
	Near = 0.1f;
	Far = 1000.0f;
	Fov = 60.0f;

	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = Size.x;
	ViewPortDesc.Height = Size.y;
	ViewPortDesc.MinDepth = 0.0f;
	ViewPortDesc.MaxDepth = 1.0f;
}

GameEngineCamera::~GameEngineCamera() 
{
}

bool ZSort(GameEngineRenderer* _Left, GameEngineRenderer* _Right)
{
	return _Left->GetTransform().GetWorldPosition().z > _Right->GetTransform().GetWorldPosition().z;
}

void GameEngineCamera::Render(float _DeltaTime)
{
	CameraRenderTarget->Clear();
	CameraRenderTarget->Setting();

	// ���������κ��� �����Ͷ����� �ܰ������� ������ ���� �����ű� ������.
	GameEngineDevice::GetContext()->RSSetViewports(1, &ViewPortDesc);

	// �����ϱ� ���� 
	View.LookAtLH(
		GetActor()->GetTransform().GetLocalPosition(), 
		GetActor()->GetTransform().GetForwardVector(), 
		GetActor()->GetTransform().GetUpVector());

	switch (Mode)
	{
	case CAMERAPROJECTIONMODE::PersPective:
		Projection.PerspectiveFovLH(Fov, Size.x, Size.y, Near, Far);
		break;
	case CAMERAPROJECTIONMODE::Orthographic:
		Projection.OrthographicLH(Size.x, Size.y, Near, Far);
		break;
	default:
		break;
	}

	float4 WindowSize = GameEngineWindow::GetInst()->GetScale();

	// �ν��Ͻ������� �ʱ�ȭ�ؿ�.
	/*for (const std::pair<GameEngineRenderingPipeLine*, RenderingInstancing>& _Data : InstancingMap)
	{
		_Data.second.DataInsert = 0;
	}*/


	// ������ �ϱ� ����
	for (std::pair<const int, std::list<GameEngineRenderer*>>& Group : AllRenderer_)
	{
		float ScaleTime = GameEngineTime::GetInst()->GetDeltaTime(Group.first);

		std::list<GameEngineRenderer*>& RenderList = Group.second;
		RenderList.sort(ZSort);

		for (GameEngineRenderer* const Renderer : Group.second)
		{
			if (false == Renderer->IsUpdate())
			{
				continue;
			}

			Renderer->renderOption.DeltaTime = _DeltaTime;
			Renderer->renderOption.SumDeltaTime = _DeltaTime;
			Renderer->GetTransform().SetView(View);
			Renderer->GetTransform().SetProjection(Projection);
			Renderer->GetTransform().CalculateWorldViewProjection();

			// �ν��Ͻ� ���� ����
			Renderer->Render(ScaleTime);
		}
	}

	// �ٳ����� �ν��Ͻ��� ������
}

void GameEngineCamera::SetCameraOrder(CAMERAORDER _Order)
{
	GetActor()->GetLevel()->PushCamera(this, _Order);
}

void GameEngineCamera::Start()
{
	CameraRenderTarget = GameEngineRenderTarget::Create();

	CameraRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	CameraRenderTarget->SettingDepthTexture(GameEngineDevice::GetBackBuffer()->GetDepthTexture());
	// CameraRenderTarget->CreateDepthTexture()
}

void GameEngineCamera::PushRenderer(GameEngineRenderer* _Renderer)
{
	AllRenderer_[_Renderer->RenderingOrder].push_back(_Renderer);
}

void GameEngineCamera::PushInstancing(GameEngineRenderingPipeLine* _Pipe, int Count)
{
	if (false == _Pipe->GetVertexShader()->IsInstancing())
	{
		MsgBoxAssert("�ν��Ͻ��� �Ұ����� ������ �Դϴ�.")
	}

	// Camera->gameenginepipeline
	InstancingMap[_Pipe].Count += Count;

	if (RenderingInstancing::MinInstancingCount <= InstancingMap[_Pipe].Count
		&& nullptr == InstancingMap[_Pipe].Buffer)
	{
		GameEngineVertexBuffer* Buffer = _Pipe->GetVertexBuffer();
		InstancingMap[_Pipe].Size = Buffer->GetLayOutDesc()->InstancingSize;
		InstancingMap[_Pipe].Buffer = GameEngineInstancingBuffer::Create(RenderingInstancing::StartInstancingCount, Buffer->GetLayOutDesc()->InstancingSize);
		InstancingMap[_Pipe].DataBuffer.resize(RenderingInstancing::StartInstancingCount * InstancingMap[_Pipe].Size);
	}
	else if(nullptr != InstancingMap[_Pipe].Buffer
		&& InstancingMap[_Pipe].Count > InstancingMap[_Pipe].Buffer->GetBufferCount())
	{
		//           105                           100
		GameEngineVertexBuffer* Buffer = _Pipe->GetVertexBuffer();
		int NextBufferSize = static_cast<int>(InstancingMap[_Pipe].Count * 1.5);
		InstancingMap[_Pipe].Buffer->BufferCreate(NextBufferSize, Buffer->GetLayOutDesc()->InstancingSize);
		InstancingMap[_Pipe].DataBuffer.resize(NextBufferSize * InstancingMap[_Pipe].Size);
	}
}

void GameEngineCamera::PushInstancingData(GameEngineRenderingPipeLine* _Pipe, void* _DataPtr, int _Size)
{
	// int DataOffset = InstancingMap[_Pipe].DataInsert * _Size;
	// InstancingMap[_Pipe].DataBuffer[DataOffset];
}

void GameEngineCamera::Release(float _DelataTime)
{
	std::map<int, std::list<GameEngineRenderer*>>::iterator StartGroupIter = AllRenderer_.begin();
	std::map<int, std::list<GameEngineRenderer*>>::iterator EndGroupIter = AllRenderer_.end();

	for (; StartGroupIter != EndGroupIter; ++StartGroupIter)
	{
		std::list<GameEngineRenderer*>& Group = StartGroupIter->second;
		std::list<GameEngineRenderer*>::iterator GroupStart = Group.begin();
		std::list<GameEngineRenderer*>::iterator GroupEnd = Group.end();

		for (; GroupStart != GroupEnd; )
		{
			(*GroupStart)->ReleaseUpdate(_DelataTime);
			if (true == (*GroupStart)->IsDeath())
			{
				GroupStart = Group.erase(GroupStart);
			}
			else
			{
				++GroupStart;
			}

		}
	}
}

float4 GameEngineCamera::GetMouseScreenPosition() 
{
	POINT P;

	GetCursorPos(&P);

	ScreenToClient(GameEngineWindow::GetHWND(), &P);

	return { static_cast<float>(P.x), static_cast<float>(P.y) };
}

void GameEngineCamera::Update(float _DeltaTime) 
{
	float4 MousePos = GetMouseWorldPosition();
	MousePos.w = 0.0f;
	MouseDir = MousePos - PrevMouse;
	PrevMouse = MousePos;
}

// ����Ʈ�� �ִ°���?
float4 GameEngineCamera::GetMouseWorldPosition()
{
	float4 Pos = GetMouseScreenPosition();

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.x, Size.y, 0, 0, 0, 1);
	ViewPort.Inverse();

	float4x4 ProjectionInvers = Projection.InverseReturn();

	Pos = Pos * ViewPort;
	Pos = Pos * ProjectionInvers;
	// ���콺�� ����Ʈ�� ��ǥ��?

	return Pos;
}


float4 GameEngineCamera::GetMouseWorldPositionToActor()
{
	return GetTransform().GetWorldPosition() + GetMouseWorldPosition();
}

void GameEngineCamera::ChangeRenderingOrder(GameEngineRenderer* _Renderer, int _ChangeOrder)
{
	// 0��°���� �����ǰ�
	AllRenderer_[_Renderer->GetRenderingOrder()].remove(_Renderer);

	_Renderer->RenderingOrder = _ChangeOrder;

	// 10000��°�� �̵��Ѵ�.
	AllRenderer_[_Renderer->GetRenderingOrder()].push_back(_Renderer);
}

void GameEngineCamera::OverRenderer(GameEngineCamera* _NextCamera) 
{
	if (nullptr == _NextCamera)
	{
		MsgBoxAssert("next camera is nullptr! fuck you");
		return;
	}

	std::map<int, std::list<GameEngineRenderer*>>::iterator StartGroupIter = AllRenderer_.begin();
	std::map<int, std::list<GameEngineRenderer*>>::iterator EndGroupIter = AllRenderer_.end();

	for (; StartGroupIter != EndGroupIter; ++StartGroupIter)
	{
		std::list<GameEngineRenderer*>& Group = StartGroupIter->second;
		std::list<GameEngineRenderer*>::iterator GroupStart = Group.begin();
		std::list<GameEngineRenderer*>::iterator GroupEnd = Group.end();

		for (; GroupStart != GroupEnd; )
		{
			GameEngineActor* Root = (*GroupStart)->GetRoot<GameEngineActor>();

			if (true == Root->IsLevelOver)
			{
				_NextCamera->AllRenderer_[StartGroupIter->first].push_back(*GroupStart);
				GroupStart = Group.erase(GroupStart);
			}
			else
			{
				++GroupStart;
			}

		}
	}
}