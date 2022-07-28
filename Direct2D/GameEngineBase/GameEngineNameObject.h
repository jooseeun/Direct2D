#pragma once
#include <string>

// ���� :
class GameEngineNameObject
{
public:
	// constrcuter destructer
	GameEngineNameObject();
	virtual ~GameEngineNameObject();

	// �׳� std::string���� �ϸ� ���� ���簡 �Ǹ鼭 ������ ��������.
	inline void SetName(const std::string& _Name) 
	{
		ObjectName_ = _Name;
	}

	// �� �Ⱦ��ϴ� ������� �̽��ϴ�.
	// �ʿ��Ҷ� ��߰���.
	std::string GetNameCopy() const
	{
		return ObjectName_;
	}

	const std::string& GetNameConstRef()
	{
		return ObjectName_;
	}

	const char* GetNameConstPtr()
	{
		return ObjectName_.c_str();
	}


protected:

private:
	std::string ObjectName_;

};

