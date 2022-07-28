#pragma once
#include "GameEnginePath.h"

class GameEngineFile;
class GameEngineDirectory : public GameEnginePath
{
public:
	// constrcuter destructer
	GameEngineDirectory();
	GameEngineDirectory(const char* _Path);
	GameEngineDirectory(const std::filesystem::path& _Path);
	GameEngineDirectory(const GameEngineDirectory& _Other);
	~GameEngineDirectory();

	void MoveParent();

	void MoveParent(const std::string& _Name);

	bool MoveParentToExitsChildDirectory(const std::string& _Name);

	bool IsRoot();

	void Move(const std::string& _Name);

	std::string PlusFilePath(const std::string& _Name);

	// ��������

	std::vector<GameEngineFile> GetAllFile(const std::string& _Ext = "");

	// ��Ŀ�ú��� ���� �༮�� �������� �� ���� �༮
	std::vector<GameEngineDirectory> GetAllDirectory();

	// std::vector<GameEngineFile> GetRecursiveAllFile(const std::string& _Ext = "");

	// "C:adskjfklasdjfklas"
	std::vector<GameEngineDirectory> GetRecursiveAllDirectory();


protected:
	

private:

};

