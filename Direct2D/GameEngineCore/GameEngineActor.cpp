#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineComponent.h"
#include "GameEngineTransformComponent.h"

GameEngineActor::GameEngineActor()
	: ParentLevel(nullptr)
	, IsLevelOver(false)
{

}

GameEngineActor::~GameEngineActor()
{
}

void GameEngineActor::Start() {}
void GameEngineActor::Update(float _DeltaTime) {}
void GameEngineActor::End() {}

void GameEngineActor::DetachObject()
{
	if (nullptr == GetParent())
	{
		MsgBoxAssert("�θ� �������� �ʴ� ������Ʈ�� ��� �� �����ϴ�");
	}

	GameEngineUpdateObject::DetachObject();

	GetTransform().DetachTransform();

	GetLevel()->PushActor(this, this->GetOrder());
}

void GameEngineActor::SetParent(GameEngineUpdateObject* _Object)
{
	if (nullptr == GetParent())
	{
		GetLevel()->RemoveActor(this);
	}

	GameEngineUpdateObject::SetParent(_Object);

	{
		GameEngineTransformBase* Actor = nullptr;
		if (Actor = dynamic_cast<GameEngineTransformBase*>(_Object))
		{
			GetTransform().SetParentTransform(Actor->GetTransform());
			return;
		}
	}

	MsgBoxAssert("Ʈ�������� ���� ������Ʈ�� Ʈ�������� �ִ� �θ� ���̷��� �߽��ϴ�.");
}
