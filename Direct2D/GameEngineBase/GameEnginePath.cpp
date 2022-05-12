#include "GameEnginePath.h"

GameEnginePath::GameEnginePath()
{
}

GameEnginePath::GameEnginePath(const std::filesystem::path& _Path)
	: Path_(_Path)
{

}

GameEnginePath::~GameEnginePath()
{
}

void GameEnginePath::SetCurrentPath()
{
	// ���� ����.
	Path_ = std::filesystem::current_path();
}

bool GameEnginePath::IsExits()
{
	return std::filesystem::exists(Path_);
}

std::string GameEnginePath::GetExtension() const
{
	return Path_.extension().string();
}

std::string GameEnginePath::GetFileName()  const
{
	return Path_.filename().string();
}

std::string GameEnginePath::GetFullPath() const
{
	return Path_.string();
}