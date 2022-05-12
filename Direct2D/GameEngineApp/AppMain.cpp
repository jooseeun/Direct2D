#include <Windows.h>

#pragma comment(lib, "GameEngineBase.lib")

#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineSound.h>
#include <GameEngineBase/GameEngineWindow.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GameEngineDirectory Dir;
	// Path_ = std::filesystem::current_path();

	if (false == Dir.MoveParentToExitsChildDirectory("Resources"))
	{
		MsgBoxAssert("경로를 찾지 못했습니다.");
		return 0;
	}

	Dir.Move("Resources");
	Dir.Move("Sound");

	std::vector<GameEngineFile> Files = Dir.GetAllFile();

	// Range for
	// Files 특정한 오퍼레이터가 되어있으면 
	for (GameEngineFile& File : Files)
	{
		GameEngineSound::LoadRessource(File);
	}

	GameEngineSound::SoundPlayOneShot("BGM.mp3");



	GameEngineWindow::GetInst().CreateGameWindow(hInstance, "MainTitle");
	GameEngineWindow::GetInst().SetWindowScaleAndPosition({ 0,0 }, { 1280, 720 });
	GameEngineWindow::GetInst().ShowGameWindow();


	return 0;
}