#pragma once
#include "GameEngineCore.h"

// ���� : ����� GameEngineCore�� Ŭ�������� GEngine�� ������� �ʾƾ� �Ѵ�.
//        ���� => �� Ŭ������ ��ɵ��� ö���ϰ� �������鿡�� �����Ǵ� �̴ϴ�.
class GEngine
{
public:
	static void ChangeLevel(std::string _Name);
	

protected:

private:
	// constrcuter destructer
	GEngine();
	virtual ~GEngine() = 0;
	// delete Function
	GEngine(const GEngine& _Other) = delete;
	GEngine(GEngine&& _Other) noexcept = delete;
	GEngine& operator=(const GEngine& _Other) = delete;
	GEngine& operator=(GEngine&& _Other) noexcept = delete;

};

