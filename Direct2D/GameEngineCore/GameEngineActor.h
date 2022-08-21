#pragma once
#include <GameEngineBase/GameEngineNameObject.h>
#include "GameEngineUpdateObject.h"
#include <list>
#include "GameEngineTransformBase.h"


// ���� : ȭ�鿡 �����ϴ� ������ ǥ���ϱ� ���� Ŭ����
class GameEngineComponent;
class GameEngineTransformComponent;
class GameEngineActor :
	public GameEngineNameObject,
	public GameEngineUpdateObject,
	public GameEngineTransformBase
{
	friend class GameEngineLevel;
	friend class GameEngineCamera;

public:
	// constrcuter destructer
	GameEngineActor();
	virtual ~GameEngineActor();

	// delete Function
	GameEngineActor(const GameEngineActor& _Other) = delete;
	GameEngineActor(GameEngineActor&& _Other) noexcept = delete;
	GameEngineActor& operator=(const GameEngineActor& _Other) = delete;
	GameEngineActor& operator=(GameEngineActor&& _Other) noexcept = delete;

	inline GameEngineLevel* GetLevel()
	{
		return ParentLevel;
	}

	template<typename LevelType>
	inline LevelType* GetLevel()
	{
		return dynamic_cast<LevelType*>(ParentLevel);
	}

	template<typename ComponentType>
	ComponentType* CreateComponent(const std::string& _Name = "")
	{
		GameEngineComponent* NewComponent = new ComponentType();
		NewComponent->SetName(_Name);
		NewComponent->SetParent(this);
		NewComponent->Start();

		return dynamic_cast<ComponentType*>(NewComponent);
	}

	void DetachObject() override;

	void SetParent(GameEngineUpdateObject*) override;


	void SetLevelOverOn() 
	{
		IsLevelOver = true;
	}

	void ActorUpdate(float _DeltaTime);

	bool IsRoot() 
	{
		return nullptr == GetParent();
	}

protected:
	virtual void Start() override;
	virtual void Update(float _DeltaTime) override;
	virtual void End() override;



private:
	void SetLevel(GameEngineLevel* _ParentLevel)
	{
		ParentLevel = _ParentLevel;
	}

	bool IsLevelOver;

	class GameEngineLevel* ParentLevel;
};

