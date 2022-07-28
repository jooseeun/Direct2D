#pragma once
#include <crtdbg.h>
#include <string>
#include <assert.h>
#include <Windows.h>


// Ό³Έν :
class GameEngineDebug
{
public:
	static void LeakCheckOn();
	static void ConsoleOpen();

	static void OutPutString(const std::string& _Text);

	static void Destroy();

protected:

private:
	static HANDLE hConsole;
	// constrcuter destructer
	GameEngineDebug();
	~GameEngineDebug();

	// delete Function
	GameEngineDebug(const GameEngineDebug& _Other) = delete;
	GameEngineDebug(GameEngineDebug&& _Other) noexcept = delete;
	GameEngineDebug& operator=(const GameEngineDebug& _Other) = delete;
	GameEngineDebug& operator=(GameEngineDebug&& _Other) noexcept = delete;

};

#define MsgBox(Text) 	MessageBeep(0); \
MessageBoxA(nullptr, Text, "Error", MB_OK);  

#define MsgBoxAssert(Text) 	MessageBeep(0); \
MessageBoxA(nullptr, Text, "Error", MB_OK); \
assert(false); 

#define MsgBoxAssertString(Text) 	MessageBeep(0); \
MessageBoxA(nullptr, (Text).c_str(), "Error", MB_OK); \
assert(false); 
