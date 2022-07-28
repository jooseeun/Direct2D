#pragma once
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <map>

// ���� :
class GameEngineCore
{
	friend class GEngine;

public:
	template<typename CoreType>
	static void Start()
	{
		GameEngineDebug::LeakCheckOn();

		// ���⿡�� ������� �� �ھ �˾ƾ� �ϴ°���.
		CoreType Core;
		WindowCreate(Core.GetWindowTitle(), &Core);
	}



protected:
	virtual std::string GetWindowTitle() { return "MainWindow"; }

	// ������� �����Ҽ� �ִ� ����.
	virtual void Start() = 0;
	// �������� �����ϴ� �ð��� �ְ� �ʴ� �̰� ���.
	virtual void Update(float _DeltaTime) = 0;
	virtual void End() = 0;

	GameEngineCore();
	~GameEngineCore();

	static class GameEngineLevel* FindLevel(const std::string& _Name);

	template<typename LevelType>
	static GameEngineLevel* CreateLevel(const std::string& _Name)
	{
		std::string UpperName = GameEngineString::ToUpperReturn(_Name);
		GameEngineLevel* NewLevel = new LevelType();
		InitializeLevel(NewLevel, UpperName);
		return NewLevel;
	}

	static bool ChangeLevel(const std::string& _Name);

private:
	static std::map<std::string, class GameEngineLevel*> AllLevels;
	static GameEngineLevel* CurrentLevel;
	static GameEngineLevel* NextLevel;

	static void EngineResourcesInitialize();
	static void EngineResourcesDestroy();

	static void WindowCreate(const std::string& _Name, GameEngineCore* _UserCore);
	static void CoreStart(GameEngineCore* _UserCore); // ���α׷� ����
	static void CoreUpdate(GameEngineCore* _UserCore); // ���α׷� ������Ʈ
	static void CoreEnd(GameEngineCore* _UserCore); // ���α׷� ������Ʈ

	// ��� �߰��ϱ� �Ⱦ �ʱ�ȭ�� CPP���� �ϱ� ���� �Լ�.
	static void InitializeLevel(GameEngineLevel* _Level, const std::string _Name);

	// delete Function
	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;


};

