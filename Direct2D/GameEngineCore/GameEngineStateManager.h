#pragma once
#include <GameEngineBase/GameEngineNameObject.h>

class StateInfo
{
public:
	float StateTime; // ���� �� ������Ʈ�� �󸶳� ���ӵƳ�?
	std::string PrevState; // ���� ������Ʈ�� ������
};

class GameEngineStateManager;
class GameEngineState : public GameEngineNameObject
{
	friend GameEngineStateManager;

public:
	void StateUpdate(float _DeltaTime) 
	{
		if (nullptr == Update)
		{
			return;
		}

		Update(_DeltaTime, Info);
	}

private:
	StateInfo Info;
	std::function<void(const StateInfo&)> Start;
	std::function<void(const StateInfo&)> End;
	std::function<void(float _DeltaTime, const StateInfo&)> Update;


public:
	GameEngineState()
		: Start(nullptr)
		, End(nullptr)
		, Update(nullptr)
	{

	}
};

// ���� :
class GameEngineStateManager final // ���� ��� ������
{
public:
	// constrcuter destructer
	GameEngineStateManager();
	~GameEngineStateManager();

	// delete Function
	GameEngineStateManager(const GameEngineStateManager& _Other) = delete;
	GameEngineStateManager(GameEngineStateManager&& _Other) noexcept = delete;
	GameEngineStateManager& operator=(const GameEngineStateManager& _Other) = delete;
	GameEngineStateManager& operator=(GameEngineStateManager&& _Other) noexcept = delete;

	// �ɹ��Լ��� �˴ϴ�.
	template<typename ObjectType>
	void CreateStateMember(const std::string& _StateName
		, ObjectType* _Object
		, void(ObjectType::* _Update)(float, const StateInfo&)
		, void(ObjectType::* _Start)(const StateInfo&) = nullptr
		, void(ObjectType::* _End)(const StateInfo&) = nullptr
	) 
	{
		if (AllState.end() != AllState.find(_StateName))
		{
			MsgBoxAssert("�����ϴ� �̸��� ������Ʈ�� �� ������� �߽��ϴ�.");
		}

		// ������ ����� find�� �������� ã�Ƴ���
		GameEngineState& NewState = AllState[_StateName];
		NewState.SetName(_StateName);
		if (nullptr != _Update)
		{
			NewState.Update = std::bind(_Update, _Object, std::placeholders::_1, std::placeholders::_2);
		}
		if (nullptr != _Start)
		{
			NewState.Start = std::bind(_Start, _Object, std::placeholders::_1);
		}
		if (nullptr != _End)
		{
			NewState.End = std::bind(_End, _Object, std::placeholders::_1);
		}
	}

	void Update(float _DeltaTime);

	void ChangeState(const std::string& _StateName)
	{
		if (AllState.end() == AllState.find(_StateName))
		{
			MsgBoxAssert("�������� �ʴ� ������Ʈ�� ���� �Ϸ��� �߽��ϴ�.");
		}

		std::string PrevState = "";

		if (nullptr != CurState)
		{
			PrevState = CurState->GetNameCopy();
			if (nullptr != CurState->End)
			{
				CurState->End(CurState->Info);
			}
		}

		CurState = &AllState[_StateName];
		CurState->Info.StateTime = 0.0f;
		CurState->Info.PrevState = PrevState;

		if (nullptr != CurState->Start)
		{
			CurState->Start(CurState->Info);
		}
	}

protected:

private:
	std::map<std::string, GameEngineState> AllState;
	GameEngineState* CurState;

};

