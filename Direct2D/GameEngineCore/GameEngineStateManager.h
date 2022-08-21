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

		Info.StateTime += _DeltaTime;
		Update(_DeltaTime, Info);
	}

private:
	StateInfo Info;
	std::function<void(const StateInfo&)> Start;
	std::function<void(const StateInfo&)> End;
	std::function<void(float, const StateInfo&)> Update;


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
	void CreateStateMember(const std::string& _StateName
		, std::function<void(float, const StateInfo&)> _Update
		, std::function<void(const StateInfo&)> _Start = nullptr
		, std::function<void(const StateInfo&)> _End = nullptr
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
			NewState.Update = _Update;
		}
		if (nullptr != _Start)
		{
			NewState.Start = _Start;
		}
		if (nullptr != _End)
		{
			NewState.End = _End;
		}
	}

	void Update(float _DeltaTime);

	std::string GetCurStateStateName()
	{
		if (nullptr == CurState)
		{
			return "";
		}

		return CurState->GetNameCopy();
	}

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

	float GetCurStateTime() 
	{
		if (nullptr == CurState)
		{
			return 0.0f;
		}

		return CurState->Info.StateTime;
	}

protected:

private:
	std::map<std::string, GameEngineState> AllState;
	GameEngineState* CurState;
};

