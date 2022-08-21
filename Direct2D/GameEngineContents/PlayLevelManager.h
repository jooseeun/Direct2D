#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include "MapSet.h"
// Ό³Έν :
class MapSet;
class PlayLevelManager : public GameEngineLevel
{

public:
	// constrcuter destructer
	PlayLevelManager();
	~PlayLevelManager();

	// delete Function
	PlayLevelManager(const PlayLevelManager& _Other) = delete;
	PlayLevelManager(PlayLevelManager&& _Other) noexcept = delete;
	PlayLevelManager& operator=(const PlayLevelManager& _Other) = delete;
	PlayLevelManager& operator=(PlayLevelManager&& _Other) noexcept = delete;

	inline GameEngineTextureRenderer* GetBackGround() const
	{
		return Map->BackGround;
	}
	inline GameEngineTextureRenderer* GetFrontObject() const
	{
		return Map->FrontObject;
	}
	inline GameEngineTextureRenderer* GetBackObject() const
	{
		return Map->BackObject;
	}
	inline GameEngineTextureRenderer* GetGround() const
	{
		return Map->Ground;
	}
	inline GameEngineTextureRenderer* GetColMap() const
	{
		return Map->ColMap;
	}

	inline void SetMapONOFF() const
	{
		Map->Ground->OnOffSwitch();
		Map->BackGround->OnOffSwitch();
		Map->BackObject->OnOffSwitch();
		Map->FrontObject->OnOffSwitch();
	}

protected:
	void CreateMap(const std::string _BackGround
		, const std::string _BackObject
		, const std::string _Ground
		, const std::string _FrontObject
		, const std::string _ColMap);
private:

	MapSet* Map;
};

