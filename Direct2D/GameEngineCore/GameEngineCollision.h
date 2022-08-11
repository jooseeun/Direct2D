#pragma once
#include <GameEngineBase/GameEngineTransform.h>
#include "GameEngineTransformComponent.h"

// ���� :
class GameEngineCollisionFunctionInit;
class GameEngineCollision : public GameEngineTransformComponent
{
	friend GameEngineCollisionFunctionInit;

private:
	static bool (*CollisionFunction[static_cast<int>(CollisionType::CT_MAX)][static_cast<int>(CollisionType::CT_MAX)])(const GameEngineTransform& _Left, const GameEngineTransform& _Right);

public:
	// constrcuter destructer
	GameEngineCollision();
	~GameEngineCollision();

	// delete Function
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

	template<typename EnumType>
	void ChangeOrder(EnumType _Order)
	{
		ChangeOrder(static_cast<int>(_Order));
	}

	void ChangeOrder(int _Order);

	template<typename EnumType>
	bool IsCollision(CollisionType _ThisType, EnumType _GroupOrder
		, CollisionType _OtherCollision
		, std::function<bool(GameEngineCollision* _This, GameEngineCollision* _Other)> _Function = nullptr)
	{
		return IsCollision(_ThisType, static_cast<int>(_GroupOrder), _OtherCollision, _Function);
	}

	// �浹�� �߳� ���߳�?
	// �־������ �Լ��� �����ϴ� bool���� true��� �� ���� ���̻� �浹üũ�� ���� ����� end�� true�̴�.
	bool IsCollision(CollisionType _ThisType, int _GroupOrder
		, CollisionType _OtherType
		, std::function<bool(GameEngineCollision* _This, GameEngineCollision* _Other)> _Function = nullptr);

	void SetDebugSetting(CollisionType _DebugType, float4 _Color)
	{
		DebugType = _DebugType;
		Color = _Color;
	}

	void DebugRender();

protected:

private:
	CollisionType DebugType;
	float4 Color;

	void Start() override;
};

