#include "PreCompile.h"
#include "GameEngineCore.h"
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineCore/GameEngineDevice.h>
#include "GameEngineLevel.h"
#include "GameEngineDevice.h"
#include "GameEngineGUI.h"
#include "GameEngineCamera.h"
#include "GameEngineCameraActor.h"
#include "GameEngineCoreDebug.h"

#pragma comment(lib, "GameEngineBase.lib")

GameEngineLevel* GameEngineCore::CurrentLevel = nullptr;
GameEngineLevel* GameEngineCore::NextLevel = nullptr;

std::map<std::string, class GameEngineLevel*> GameEngineCore::AllLevels;


GameEngineCore::GameEngineCore()
{
}

GameEngineCore::~GameEngineCore()
{
}

class GameEngineLevel* GameEngineCore::FindLevel(const std::string& _Name)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name);

	std::map<std::string, GameEngineLevel*>::iterator FindIter = AllLevels.find(UpperName);
	if (FindIter == AllLevels.end())
	{
		return nullptr;
	}
	return FindIter->second;
}

bool GameEngineCore::ChangeLevel(const std::string& _Name)
{
	NextLevel = FindLevel(_Name);

	if (nullptr == NextLevel)
	{
		MsgBoxAssert("�������� �ʴ� ������ �ٲٷ��� �߽��ϴ�.");
		return false;
	}

	return true;
}

void GameEngineCore::CoreStart(GameEngineCore* _UserCore)
{
	GameEngineGUI::Initialize();

	// ���� ���ҽ��� �ϼ��Ǿ�� �մϴ�.
	EngineResourcesInitialize();

	GameEngineDebug::Debug3DInitialize();

	// ������ ������ �Ұ̴ϴ�.
		// �غ� �����ϰ�.
	_UserCore->Start();
}

void GameEngineCore::CoreUpdate(GameEngineCore* _UserCore)
{
	if (nullptr != NextLevel)
	{
		Rectangle(GameEngineWindow::GetInst()->GetHDC()
			, 0
			, 0
			, GameEngineWindow::GetInst()->GetScale().ix(), GameEngineWindow::GetInst()->GetScale().iy());

		if (nullptr != CurrentLevel)
		{
			CurrentLevel->ActorOffEvent();
			CurrentLevel->OffEvent();
			// �Ѿ���� ���Ͱ� �̶� �����ؾ� ����?

			CurrentLevel->OverChildMove(NextLevel);
		}

		CurrentLevel = NextLevel;
		NextLevel = nullptr;
		CurrentLevel->OnEvent();
		CurrentLevel->ActorOnEvent();

		// ex) Ÿ��Ʋ���� 5���� => �÷��� ������ �̵�
		//     �÷��� �������� => �ٽ� Ÿ��Ʋ������
		CurrentLevel->ReSetAccTime();

		GameEngineTime::GetInst()->Reset();
	}


	if (nullptr == CurrentLevel)
	{
		MsgBoxAssert("������ ���������� ������ ������ �����Ҽ��� �����ϴ�.");
	}

	GameEngineTime::GetInst()->Update();

	float DeltaTime = GameEngineTime::GetDeltaTime();

	GameEngineInput::GetInst()->Update(DeltaTime);
	// �������ؿ��� ������ �ϰ� ������.
	_UserCore->Update(DeltaTime);

	CurrentLevel->LevelUpdate(DeltaTime);

}

void GameEngineCore::CoreEnd(GameEngineCore* _UserCore)
{
	_UserCore->End();

	for (auto& Level : AllLevels)
	{
		if (nullptr == Level.second)
		{
			continue;
		}
		delete Level.second;
		Level.second = nullptr;
	}

	// ������ ���ҽ��� ����Ұ� ���Ƽ�
	// gui���� �ؽ�ó����� �̷��� ���ٰ�
	GameEngineGUI::GUIDestroy();

	EngineResourcesDestroy();

	GameEngineWindow::Destroy();
	GameEngineInput::Destroy();
	GameEngineDebug::Destroy();
	GameEngineTime::Destroy();
}


void GameEngineCore::WindowCreate(const std::string& _Name, GameEngineCore* _UserCore)
{
	GameEngineWindow::GetInst()->CreateGameWindow(nullptr, _Name.c_str());
	GameEngineWindow::GetInst()->SetWindowScaleAndPosition(_UserCore->StartWindowPosition(), _UserCore->StartWindowSize());
	GameEngineWindow::GetInst()->ShowGameWindow();
	GameEngineDevice::Initialize();




	// ������ �����ε�.
	GameEngineWindow::GetInst()->MessageLoop(
		std::bind(&GameEngineCore::CoreStart, _UserCore),
		std::bind(&GameEngineCore::CoreUpdate, _UserCore),
		std::bind(&GameEngineCore::CoreEnd, _UserCore)
	);


}

void GameEngineCore::InitializeLevel(GameEngineLevel* _Level, const std::string _Name)
{
	{
		GameEngineCameraActor* actor = _Level->CreateActor<GameEngineCameraActor>();
		actor->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
		actor->GetTransform().SetLocalPosition({ 0.0f, 0.0f, -100.0f });
	}

	{
		GameEngineCameraActor* actor = _Level->CreateActor<GameEngineCameraActor>();
		actor->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
		actor->GetTransform().SetLocalPosition({ 0.0f, 0.0f, -100.0f });
	}

	_Level->Start();
	_Level->SetName(_Name);

	// AllLevels.insert(std::map<std::string, GameEngineLevel*>::value_type(_Name, NewLevel));
	AllLevels.insert(std::make_pair(_Name, _Level));
}