#pragma once
#include <string>
#include <list>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEngineDebugObject.h>


class GameEngineUpdateObject : public GameEngineDebugObject
{
public:
	// constrcuter destructer
	GameEngineUpdateObject();
	virtual ~GameEngineUpdateObject();

	// delete Function
	GameEngineUpdateObject(const GameEngineUpdateObject& _Other) = delete;
	GameEngineUpdateObject(GameEngineUpdateObject&& _Other) noexcept = delete;
	GameEngineUpdateObject& operator=(const GameEngineUpdateObject& _Other) = delete;
	GameEngineUpdateObject& operator=(GameEngineUpdateObject&& _Other) noexcept = delete;


	inline bool& IsUpdateRef()
	{
		return IsUpdate_;
	}

	inline void On()
	{
		IsUpdate_ = true;
		AllOnEvent();
	}

	inline void Off()
	{
		IsUpdate_ = false;
		AllOffEvent();
	}

	inline void OnOffSwitch()
	{
		IsUpdate_ = !IsUpdate_;
		IsUpdate_ == true ? OnEvent() : OffEvent();
	}

	inline bool IsUpdate()
	{
		if (nullptr != Parent)
		{
			return IsUpdate_ && false == IsDeath_ && true == Parent->IsUpdate();
		}
		else
		{
			return IsUpdate_ && false == IsDeath_;
		}

	}

	inline bool IsDeath()
	{
		if (nullptr != Parent)
		{
			return IsDeath_ || true == Parent->IsDeath();
		}
		else {
			return IsDeath_;
		}
	}

	void AddAccTime(float _DeltaTime)
	{
		AccTime_ += _DeltaTime;
	}

	float GetAccTime()
	{
		return AccTime_;
	}

	void ReSetAccTime()
	{
		AccTime_ = 0.0f;
	}

	inline 	void Death()
	{
		IsDeath_ = true;
	}

	void ReleaseUpdate(float _DeltaTime)
	{
		if (false == IsReleaseUpdate_)
		{
			return;
		}

		DeathTime_ -= _DeltaTime;

		if (0.0f >= DeathTime_)
		{
			IsDeath_ = true;
		}
	}

	inline void Death(float _Time)
	{
		IsReleaseUpdate_ = true;
		DeathTime_ = _Time;
	}

	inline int GetOrder()
	{
		return Order_;
	}

	virtual inline void SetOrder(int _Order)
	{
		Order_ = _Order;
	}

	template<typename ParentType>
	ParentType* GetParent()
	{
		return dynamic_cast<ParentType*>(Parent);
	}

	GameEngineUpdateObject* GetParent()
	{
		return Parent;
	}

	template<typename ParentType>
	ParentType* GetRoot()
	{
		return dynamic_cast<ParentType*>(GetRoot());
	}

	GameEngineUpdateObject* GetRoot()
	{
		GameEngineUpdateObject* CurObject = this;

		while (nullptr != CurObject->GetParent())
		{
			CurObject = CurObject->GetParent();
		}

		return CurObject;
	}


	virtual void SetParent(GameEngineUpdateObject* _Parent);
	virtual void DetachObject();

	virtual void ReleaseHierarchy();

	// �� ������Ʈ�� �����ӱ����ȿ��� ���� �ִ�.
	virtual void Update(float _DeltaTime) = 0;

	void AllUpdate(float _DeltaTime);


	virtual void LevelStartEvent() {}
	virtual void LevelEndEvent() { }

protected:
	void AllLevelStartEvent();
	void AllLevelEndEvent();

	void AllOnEvent();
	void AllOffEvent();

	// �� ������Ʈ�� ������ �ϱ� �����ߴ�.
	virtual void OnEvent() {}//����ü���� ��ŸƮ
												//�������� �̷����� ���ͳ� ������Ʈ�� �����ִ�.
	// �� ������Ʈ�� ������.
	virtual void OffEvent() {}//����ü���� ����

	// �� ������Ʈ�� ���������.
	virtual void Start() = 0;

	// �� ������Ʈ�� �޸𸮰� �����ȴ�.
	virtual void End() = 0;

	virtual void ReleaseObject(std::list<GameEngineUpdateObject*>& _RelaseList);

	template<typename ConvertType>
	std::list<ConvertType*> GetConvertChilds() 
	{
		std::list<ConvertType*> NewList;

		for (GameEngineUpdateObject* Child : Childs)
		{
			ConvertType* ConvertPtr = dynamic_cast<ConvertType*>(Child);
			if (nullptr != ConvertPtr)
			{
				NewList.push_back(ConvertPtr);
			}
		}

		return NewList;
	}

	std::list<GameEngineUpdateObject*> Childs;




private:
	int Order_;
	bool IsReleaseUpdate_;
	float DeathTime_;
	float AccTime_;

	bool IsUpdate_;
	bool IsDeath_;

	GameEngineUpdateObject* Parent;
};

