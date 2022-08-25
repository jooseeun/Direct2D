#pragma once
#include <GameEngineCore/CoreMinimal.h>
// Ό³Έν :
class GameEngineCollision;
class MapMoveCollision : public GameEngineActor
{
public:
	// constrcuter destructer
	MapMoveCollision();
	~MapMoveCollision();

	// delete Function
	MapMoveCollision(const MapMoveCollision& _Other) = delete;
	MapMoveCollision(MapMoveCollision&& _Other) noexcept = delete;
	MapMoveCollision& operator=(const MapMoveCollision& _Other) = delete;
	MapMoveCollision& operator=(MapMoveCollision&& _Other) noexcept = delete;

	GameEngineCollision* MoveCol1;
	GameEngineCollision* MoveCol2;
	GameEngineCollision* MoveCol3;
	GameEngineCollision* MoveCol4;

	std::string MoveLevel1;
	std::string MoveLevel2;
	std::string MoveLevel3;
	std::string MoveLevel4;
	
	bool Move1Level(GameEngineCollision* _This, GameEngineCollision* _Other);
	bool Move2Level(GameEngineCollision* _This, GameEngineCollision* _Other);
	bool Move3Level(GameEngineCollision* _This, GameEngineCollision* _Other);
	bool Move4Level(GameEngineCollision* _This, GameEngineCollision* _Other);
protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:

};

