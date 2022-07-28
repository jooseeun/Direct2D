#include "PreCompile.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include "GameEngineCamera.h"
#include "GameEngineCameraActor.h"
#include "GameEngineGUI.h"

GameEngineLevel::GameEngineLevel() 
{
	Cameras.resize(static_cast<unsigned int>(CAMERAORDER::UICAMERA));

	{
		GameEngineCameraActor* CameraActor = CreateActor<GameEngineCameraActor>();
		CameraActor->GetTransform().SetLocalPosition({ 0.0f, 0.0f, -100.0f });
		CameraActor->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
		CameraActor->GetCameraComponent()->SetCameraOrder(CAMERAORDER::MAINCAMERA);
	}

	{
		GameEngineCameraActor* CameraActor = CreateActor<GameEngineCameraActor>();
		CameraActor->GetTransform().SetLocalPosition({ 0.0f, 0.0f, -100.0f });
		CameraActor->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
		CameraActor->GetCameraComponent()->SetCameraOrder(CAMERAORDER::UICAMERA);
	}
}

GameEngineLevel::~GameEngineLevel() 
{
	for (const std::pair<int, std::list<GameEngineActor*>>& Group : AllActors)
	{
		for (GameEngineActor* Actor : Group.second)
		{
			if (nullptr == Actor)
			{
				continue;
			}

			Actor->ReleaseHierarchy();
		}
	}
}


void GameEngineLevel::ActorUpdate(float _DeltaTime)
{
	for (const std::pair<int, std::list<GameEngineActor*>>& Group : AllActors)
	{
		float ScaleTime = GameEngineTime::GetInst()->GetDeltaTime(Group.first);
		for (GameEngineActor* const Actor : Group.second)
		{
			if (false == Actor->IsUpdate())
			{
				continue;
			}

			Actor->AllUpdate(ScaleTime, _DeltaTime);
		}
	}

}

void GameEngineLevel::PushRenderer(GameEngineRenderer* _Renderer, int _CameraOrder)
{
	Cameras[_CameraOrder]->PushRenderer(_Renderer);
}

void GameEngineLevel::PushCamera(GameEngineCamera* _Camera, int _CameraOrder)
{
	Cameras.resize(_CameraOrder + 1);

	Cameras[_CameraOrder] = _Camera;
}

GameEngineTransform& GameEngineLevel::GetMainCameraActorTransform()
{
	return Cameras[static_cast<int>(CAMERAORDER::MAINCAMERA)]->GetActor()->GetTransform();
}


GameEngineCameraActor* GameEngineLevel::GetMainCameraActor()
{
	return Cameras[static_cast<int>(CAMERAORDER::MAINCAMERA)]->GetActor<GameEngineCameraActor>();
}

GameEngineTransform& GameEngineLevel::GetUICameraActorTransform()
{
	return Cameras[static_cast<int>(CAMERAORDER::UICAMERA)]->GetActor()->GetTransform();
}


GameEngineCameraActor* GameEngineLevel::GetUICameraActor()
{
	return Cameras[static_cast<int>(CAMERAORDER::UICAMERA)]->GetActor<GameEngineCameraActor>();
}



void GameEngineLevel::Render(float _DelataTime)
{
	GameEngineDevice::RenderStart();

	// �� ���̿��� ���𰡸� �ؾ� �մϴ�.
	for (size_t i = 0; i < Cameras.size(); i++)
	{
		if (nullptr == Cameras[i])
		{
			continue;
		}

		Cameras[i]->Render(_DelataTime);
	}

	GameEngineGUI::GUIRender(this, _DelataTime);

	GameEngineDevice::RenderEnd();
}

void GameEngineLevel::Release(float _DelataTime)
{
	for (GameEngineUpdateObject* Object : DeleteObject)
	{
		Object->ReleaseHierarchy();
	}

	DeleteObject.clear();

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		if (nullptr == Cameras[i])
		{
			continue;
		}

		Cameras[i]->Release(_DelataTime);
	}

	// std::list<GameEngineActor*> ��Ʈ ���� �θ� ���� ���͵鸸 ���⿡ ���ü� �ִ�.
	// a c
	// b
	// 
	// b->setParent(a);

	std::map<int, std::list<GameEngineActor*>>::iterator StartGroupIter = AllActors.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator EndGroupIter = AllActors.end();

	for (; StartGroupIter != EndGroupIter; ++StartGroupIter)
	{
		std::list<GameEngineActor*>& Group = StartGroupIter->second;

		std::list<GameEngineActor*>::iterator GroupStart = Group.begin();
		std::list<GameEngineActor*>::iterator GroupEnd = Group.end();

		for (; GroupStart != GroupEnd; )
		{
			(*GroupStart)->ReleaseObject(DeleteObject);

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

void GameEngineLevel::LevelUpdate(float _DeltaTime)
{
	AddAccTime(_DeltaTime);
	Update(_DeltaTime);
	ActorUpdate(_DeltaTime);
	Render(_DeltaTime);
	Release(_DeltaTime);
}

// ������ �̵��ϴ� ����
// ��Ʈ�ξְ� ������� ����� �´ٰ� ������ �̴ϴ�.
void GameEngineLevel::RemoveActor(GameEngineActor* _Actor)
{
	if (AllActors.end() == AllActors.find(_Actor->GetOrder()))
	{
		MsgBoxAssert("�̾��͸� ��Ʈ�� �ƴմϴ� �����Ҽ� �����ϴ�.");
	}

	AllActors[_Actor->GetOrder()].remove(_Actor);
}

void GameEngineLevel::OverChildMove(GameEngineLevel* _NextLevel)
{
	// �÷��� ����
	
	// �α��� ����
	// _NextLevel

	std::map<int, std::list<GameEngineActor*>>::iterator StartGroupIter = AllActors.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator EndGroupIter = AllActors.end();

	std::list<GameEngineActor*> OverList;

	for (; StartGroupIter != EndGroupIter; ++StartGroupIter)
	{
		std::list<GameEngineActor*>& Group = StartGroupIter->second;

		std::list<GameEngineActor*>::iterator GroupStart = Group.begin();
		std::list<GameEngineActor*>::iterator GroupEnd = Group.end();
		for (; GroupStart != GroupEnd; )
		{
			if (true == (*GroupStart)->IsLevelOver)
			{
				// ���ʿ����� �����ǰ�
				OverList.push_back((*GroupStart));
				GroupStart = Group.erase(GroupStart);
			}
			else
			{
				++GroupStart;
			}

		}
	}

	// ������Ʈ�� �ѱ��
	for (GameEngineActor* OverActor : OverList)
	{
		_NextLevel->AllActors[OverActor->GetOrder()].push_back(OverActor);
	}

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		if (nullptr == Cameras[i])
		{
			continue;
		}

		Cameras[i]->OverRenderer(_NextLevel->Cameras[i]);
	}

	// this->Childs

}