#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineRes.h"

#include "../GameEngineCore/ThirdParty/inc/FW1FontWrapper.h"

#pragma comment(lib, "FW1FontWrapper.lib")

// ���� :
class GameEngineFont : public GameEngineRes<GameEngineFont>
{
public:
	// �̳༮�� �ѹ��� ����� �˴ϴ�.
	static IFW1Factory* FontFactiory;

	static GameEngineFont* Load(const std::string& _Path)
	{
		return Load(_Path, GameEnginePath::GetFileName(_Path));
	}

	static GameEngineFont* Load(const std::string& _Path, const std::string& _Name);

public:
	// constrcuter destructer
	GameEngineFont();
	~GameEngineFont();

	// delete Function
	GameEngineFont(const GameEngineFont& _Other) = delete;
	GameEngineFont(GameEngineFont&& _Other) noexcept = delete;
	GameEngineFont& operator=(const GameEngineFont& _Other) = delete;
	GameEngineFont& operator=(GameEngineFont&& _Other) noexcept = delete;

	void FontDraw(const std::string& _Text, float _FontSize, float4 _ScreenPos, float4 Color, UINT Flag = FW1_LEFT);

protected:

private:
	IFW1FontWrapper* FontWrapper;
	void LoadFont(const std::string& _Path);
};

