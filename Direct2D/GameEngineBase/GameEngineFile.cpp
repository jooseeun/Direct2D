#include "PreCompile.h"
#include "GameEngineFile.h"
#include "GameEngineDebug.h"

GameEngineFile::GameEngineFile() 
	: FilePtr(nullptr)
{

}

GameEngineFile::GameEngineFile(const char* _Path)
	: FilePtr(nullptr)
{
	Path_ = _Path;
}

GameEngineFile::GameEngineFile(const std::filesystem::path& _Path)
	: FilePtr(nullptr)
{
	Path_ = _Path;
}

GameEngineFile::GameEngineFile(const GameEngineFile& _Other) 
	: FilePtr(nullptr)
{
	Path_ = _Other.Path_;
}

GameEngineFile::~GameEngineFile() 
{
	GameEngineFile::Close();
}

void GameEngineFile::Close()
{
	if (nullptr != FilePtr)
	{
		fclose(FilePtr);
		FilePtr = nullptr;
	}
}

// "text"
// 

void GameEngineFile::Open(OpenMode _OpenMode, FileMode _FileMode)
{
	// b�� t�Ŀ� ���� ������ ����Ǿ������� �������� ������ �޶����� �ȴ�.
	std::string OpenMode = "";
	switch (_OpenMode)
	{
	case OpenMode::Read:
		OpenMode += "r";
		break;
	case OpenMode::Write:
		OpenMode += "w";
		break;
	default:
		break;
	}

	switch (_FileMode)
	{
	case FileMode::Text:
		OpenMode += "t";
		break;
	case FileMode::Binary:
		OpenMode += "b";
		break;
	default:
		break;
	}

	// "wb" ���� ���̳ʸ�
	// "wt" ���� �ؽ�Ʈ

	// "rb" �д´� ���̳ʸ�
	// "rt" �д´� �ؽ�Ʈ

	fopen_s(&FilePtr, Path_.string().c_str(), OpenMode.c_str());

	if (nullptr == FilePtr)
	{
		MsgBoxAssert("������ ���������� ������ �ʾҽ��ϴ�.");
	}
}

void GameEngineFile::Write(void* _WriteData, size_t _DataSize)
{
	// openmode "wt"
	
	// "aaaaaaa"

	// _Buffer ���Ͽ� ������ �Ѵ�.
	// _DataSize�� ũ�⸸ŭ
	// 1��
	fwrite(_WriteData, _DataSize, 1, FilePtr);
}

void GameEngineFile::Read(void* _Buffer, size_t _BufferSize, size_t _ReadSize)
{
	// 
	fread_s(_Buffer, _BufferSize, _ReadSize, 1, FilePtr);
}

std::string GameEngineFile::GetString()
{
	std::string AllString;

	uintmax_t Size = GetFileSize();

	AllString.resize(Size);

	Read(&AllString[0], Size, Size);

	return AllString;
}


uintmax_t GameEngineFile::GetFileSize() const
{
	return GetFileSize(Path_);
}

/*static */uintmax_t GameEngineFile::GetFileSize(const std::filesystem::path& _Path)
{
	return std::filesystem::file_size(_Path);
}