#include "PreCompile.h"
#include "GameEngineDebug.h"
#include <iostream>

HANDLE GameEngineDebug::hConsole = nullptr;

GameEngineDebug::GameEngineDebug() 
{
}

GameEngineDebug::~GameEngineDebug() 
{
}

void GameEngineDebug::LeakCheckOn()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
}

void GameEngineDebug::ConsoleOpen()
{

	if (AllocConsole())
	{
		FILE* FIle;
		freopen_s(&FIle, "CONIN$", "rb", stdin);
		freopen_s(&FIle, "CONOUT$", "wb", stdout);
		freopen_s(&FIle, "CONOUT$", "wb", stderr);

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		std::cout << "Console Start" << std::endl;
	}
}

//void GameEngineDebug::MsgBoxAssert(const std::string& _Text)
//{
//	MessageBeep(0); 
//	MessageBoxA(nullptr, _Text.c_str(), "Error", MB_OK); 
//	assert(false);
//	// �׳� �ڱ��ڽ����� ���ϴ�.
//	// ������ �ڵ� �־�޶�� �մϴ�
//	// �ϰ� ����Ÿ�
//	// �ȳ־��ָ� �˾Ƽ� window�� �ϳ� ������ش�.
//	
//	
//}

void GameEngineDebug::OutPutString(const std::string& _Text)
{
// #ifdef _DEBUG
	OutputDebugStringA((_Text + "\n").c_str());
// #endif
}

void GameEngineDebug::Destroy()
{
	if (nullptr != hConsole)
	{
		FreeConsole();
	}
}