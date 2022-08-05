#pragma once
#include <GameEngineCore/CoreMinimal.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class MapSet : public GameEngineActor
{
public:
	// constrcuter destructer
	MapSet();
	~MapSet();

	// delete Function
	MapSet(const MapSet& _Other) = delete;
	MapSet(MapSet&& _Other) noexcept = delete;
	MapSet& operator=(const MapSet& _Other) = delete;
	MapSet& operator=(MapSet&& _Other) noexcept = delete;

	GameEngineTextureRenderer* ColMap;
	GameEngineTextureRenderer* BackGround;
	GameEngineTextureRenderer* FrontObject;
	GameEngineTextureRenderer* BackObject;
	GameEngineTextureRenderer* Ground;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End();
private:

};

