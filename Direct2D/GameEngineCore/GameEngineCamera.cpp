#include "PreCompile.h"
#include "GameEngineCamera.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include <GameEngineBase/GameEngineWindow.h>

// 최소 2개이상이 모여야 인스턴싱을 시작하겠다.
int RenderingInstancing::MinInstancingCount = 2;

// 일단 시작했다면 100개는 할수 있다는 가정하에 이걸 하겠다.
int RenderingInstancing::StartInstancingCount = 100;

GameEngineCamera::GameEngineCamera() 
{
	// 윈도우가 여러분들 생각하기 가장 쉬운 비율이라서 여기서 하는거고.
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

	// 순서적으로보면 레스터라이저 단계이지만 변경이 거의 없을거기 때문에.
	GameEngineDevice::GetContext()->RSSetViewports(1, &ViewPortDesc);

	// 랜더하기 전에 
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

	// 인스턴싱정보를 초기화해요.
	/*for (const std::pair<GameEngineRenderingPipeLine*, RenderingInstancing>& _Data : InstancingMap)
	{
		_Data.second.DataInsert = 0;
	}*/


	// 랜더링 하기 전에
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

			// 인스턴싱 정보 수집
			Renderer->Render(ScaleTime);
		}
	}

	// 다끝나면 인스턴싱을 랜더링
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
		MsgBoxAssert("인스턴싱이 불가능한 랜더러 입니다.")
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

// 뷰포트에 있는거죠?
float4 GameEngineCamera::GetMouseWorldPosition()
{
	float4 Pos = GetMouseScreenPosition();

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.x, Size.y, 0, 0, 0, 1);
	ViewPort.Inverse();

	float4x4 ProjectionInvers = Projection.InverseReturn();

	Pos = Pos * ViewPort;
	Pos = Pos * ProjectionInvers;
	// 마우스는 뷰포트의 좌표다?

	return Pos;
}


float4 GameEngineCamera::GetMouseWorldPositionToActor()
{
	return GetTransform().GetWorldPosition() + GetMouseWorldPosition();
}

void GameEngineCamera::ChangeRenderingOrder(GameEngineRenderer* _Renderer, int _ChangeOrder)
{
	// 0번째에서 삭제되고
	AllRenderer_[_Renderer->GetRenderingOrder()].remove(_Renderer);

	_Renderer->RenderingOrder = _ChangeOrder;

	// 10000번째로 이동한다.
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