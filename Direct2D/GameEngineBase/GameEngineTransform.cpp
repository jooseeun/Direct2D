#include "PreCompile.h"
#include "GameEngineTransform.h"

GameEngineTransform::GameEngineTransform() 
	: Parent(nullptr)
	, CollisionDataObject()
{
	CollisionDataSetting();
}

GameEngineTransform::~GameEngineTransform() 
{
}


void GameEngineTransform::CalculateWorld()
{
	//if (IsDebug())
	//{
	//	int a = 0;
	//}

	Data.LocalWorldMatrix = Data.LocalScalingMatrix * Data.LocalRotationMatrix * Data.LocalPositionMatrix;

	if (nullptr != Parent)
	{
		// Parent ������Ʈ�� ��쿡�� ������ Ʈ�������� �θ�� ������ �ȴ�.
		Data.WorldWorldMatrix = Data.LocalWorldMatrix * Parent->GetWorldWorld();
	}
	else 
	{
		Data.WorldWorldMatrix = Data.LocalWorldMatrix;
	}

	for (GameEngineTransform* Child : Childs)
	{
		Child->CalculateWorld();
	}
}

void GameEngineTransform::CalculateWorldViewProjection()
{
	Data.WorldViewMatrix = Data.WorldWorldMatrix * Data.ViewMatrix;
	Data.WorldViewProjectionMatrix = Data.WorldViewMatrix * Data.ProjectionMatrix;

	//float4 Pos = { 0.5f, 0.5f, 0.0f, 1.0f };

	//Pos *= Data.WorldViewProjectionMatrix;

	//Pos /= Pos.w;

}

void GameEngineTransform::DetachTransform()
{
	if (nullptr == Parent)
	{
		return;
	}

	Parent->Childs.remove(this);
	Parent = nullptr;

}

void GameEngineTransform::SetParentTransform(GameEngineTransform& _Parent)
{
	if (nullptr != Parent)
	{
		// ���� �����ǰ�
		Parent->Childs.remove(this);
		Parent = nullptr;
	}

	Parent = &_Parent;
	_Parent.Childs.push_back(this);

	SetLocalScale(Data.LocalScaling);
	SetLocalRotation(Data.LocalRotation);
	SetLocalPosition(Data.LocalPosition);
}


void GameEngineTransform::PixLocalNegativeX()
{
	if (0.0f > Data.LocalScaling.x)
	{
		return;
	}

	Data.LocalScaling.x = -Data.LocalScaling.x;
	
	SetLocalScale(Data.LocalScaling);
}

void GameEngineTransform::PixLocalPositiveX()
{
	if (0.0f < Data.LocalScaling.x)
	{
		return;
	}

	// abs � ���ڸ� ������ ������ ���(���밪)���� �������ִ� �Լ�.
	Data.LocalScaling.x = abs(Data.LocalScaling.x);

	SetLocalScale(Data.LocalScaling);
}

void GameEngineTransform::Copy(const GameEngineTransform& _Trans)
{
	Data = _Trans.Data;
	CollisionDataObject = _Trans.CollisionDataObject;
	Parent = _Trans.Parent;
	Childs = _Trans.Childs;

	CalculateWorldRotation(Data.LocalRotation);
	CalculateWorldScale(Data.LocalScaling);
	CalculateWorldPosition(Data.LocalPosition);
	CalculateWorld();
}