#pragma once
#include <GameEngineCore/CoreMinimal.h>

// Ό³Έν :
class GameEngineCollision;
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
	GameEngineCollision* GameStartCol;



	GameEngineTextureRenderer* BackGround;
	GameEngineTextureRenderer* TitleLogo;
	GameEngineTextureRenderer* GameStartFont;
	GameEngineTextureRenderer* GameSet;
	GameEngineTextureRenderer* GameExit;
	GameEngineTextureRenderer* LeftArrow;
	GameEngineTextureRenderer* RightArrow;
};

