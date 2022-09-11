#pragma once
#include <GameEngineCore/CoreMinimal.h>

// Ό³Έν :
class GameEngineCollision;
class GameEngineFontRenderer;
class GameEngineTextureRenderer;
class TitleObject : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleObject();
	~TitleObject();

	// delete Function
	TitleObject(const TitleObject& _Other) = delete;
	TitleObject(TitleObject&& _Other) noexcept = delete;
	TitleObject& operator=(const TitleObject& _Other) = delete;
	TitleObject& operator=(TitleObject&& _Other) noexcept = delete;



protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End();
private:
	bool CheckStart(GameEngineCollision* _This, GameEngineCollision* _Other);
	bool CheckSet(GameEngineCollision* _This, GameEngineCollision* _Other);
	bool CheckEnd(GameEngineCollision* _This, GameEngineCollision* _Other);

	GameEngineCollision* GameStartCol;
	GameEngineCollision* GameSetCol;
	GameEngineCollision* GameEndCol;

	GameEngineTextureRenderer* BackGround;
	GameEngineTextureRenderer* TitleLogo;
	GameEngineFontRenderer* GameStartFont;
	GameEngineFontRenderer* GameSet;
	GameEngineFontRenderer* GameExit;
	GameEngineTextureRenderer* LeftArrow;
	GameEngineTextureRenderer* RightArrow;
};

