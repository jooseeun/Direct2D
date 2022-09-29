#pragma once
#include <GameEngineBase/GameEngineTransform.h>
#include "GameEngineTransformComponent.h"
#include <Set>

enum class CollisionMode
{
	Normal,
	Ex
};

enum class CollisionReturn
{
	ContinueCheck,
	Break,
};

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

	void OffEvent() override
	{
		ResetExData();
	}


	template<typename EnumType>
	void ChangeOrder(EnumType _Order)
	{
		ChangeOrder(static_cast<int>(_Order));
	}

	void ChangeOrder(int _Order);

	template<typename EnumType>
	bool IsCollision(CollisionType _ThisType, EnumType _GroupOrder
		, CollisionType _OtherCollision
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Function = nullptr)
	{

		return IsCollision(_ThisType, static_cast<int>(_GroupOrder), _OtherCollision, _Function);
	}

	bool IsCollisionEnterBase(CollisionType _ThisType, int _GroupOrder
		, CollisionType _OtherType
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Enter = nullptr
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Update = nullptr
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Exit = nullptr
	)
	{
		return IsCollision(_ThisType, _GroupOrder, _OtherType, _Update, _Enter, _Exit);
	}

	bool IsCollisionExitBase(CollisionType _ThisType, int _GroupOrder
		, CollisionType _OtherType
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Enter = nullptr
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Update = nullptr
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Exit = nullptr
	)
	{
		return IsCollision(_ThisType, _GroupOrder, _OtherType, _Update, _Enter, _Exit);
	}

	// �浹�� �߳� ���߳�?
	// �־������ �Լ��� �����ϴ� bool���� true��� �� ���� ���̻� �浹üũ�� ���� ����� end�� true�̴�.
	bool IsCollision(CollisionType _ThisType, int _GroupOrder
		, CollisionType _OtherType
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Update = nullptr
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Enter = nullptr
		, std::function<CollisionReturn(GameEngineCollision* _This, GameEngineCollision* _Other)> _Exit = nullptr
	);

	void SetDebugSetting(CollisionType _DebugType, float4 _Color)
	{
		DebugType = _DebugType;
		Color = _Color;
	}

	virtual void DebugRender();

	void SetUIDebugCamera();

	void SetDebugCamera(CAMERAORDER _Order)
	{
		DebugCameraOrder = _Order;
	}

	void SetCollisionMode(CollisionMode _Mode)
	{
		eCollisionMode = _Mode;
	}

	void ResetExData()
	{
		CollisionCheck.clear();
	}

protected:

private:
	CollisionMode eCollisionMode;

	std::set<GameEngineCollision*> CollisionCheck;

	CAMERAORDER DebugCameraOrder;

	CollisionType DebugType;
	float4 Color;

	void Start() override;
};

