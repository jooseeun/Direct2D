#pragma once
#include <map>
#include <chrono>

// ���� :
class GameEngineTime
{

private:
	static GameEngineTime* Inst_;

public:
	static GameEngineTime* GetInst()
	{
		return Inst_;
	}

	// �������� ������� ���� �ǵ��� �������� ����� �ͱ� ������
	// pointer�� �����ϴ� �̴ϴ�.
	static void Destroy()
	{
		if (nullptr != Inst_)
		{
			delete Inst_;
			Inst_ = nullptr;
		}
	}

public:
	void Reset();
	void Update();

	static inline float GetDeltaTime()
	{
		if (0.05f <= Inst_->DeltaTimef)
		{
			Inst_->DeltaTimef = 0.05f;
		}

		return Inst_->DeltaTimef * Inst_->GlobalScale;
	}

	template<typename EnumType>
	static inline float GetDeltaTime(EnumType _Key)
	{
		return GetDeltaTime(static_cast<int>(_Key));
	}

	static inline float GetDeltaTime(int _Key)
	{
		return Inst_->DeltaTimef * Inst_->GetTimeScale(_Key);
	}

	template<typename EnumType>
	static inline float SetTimeScale(EnumType _Key, float _TimeScale)
	{
		SetTimeScale(static_cast<int>(_Key), _TimeScale);
	}


	void SetTimeScale(int _Key, float _TimeScale) 
	{
		TimeScale_[_Key] = _TimeScale;
	}

	float GetTimeScale(int _Key)
	{
		if (TimeScale_.end() == TimeScale_.find(_Key))
		{
			TimeScale_[_Key] = 1.0f;
		}

		return TimeScale_[_Key];
	}

	void SetGlobalScale(float _GlobalScale) 
	{
		GlobalScale = _GlobalScale;
	}

protected:

private:
	std::chrono::steady_clock::time_point Prev;

	double DeltaTimed;
	float DeltaTimef;
	std::map<int, float> TimeScale_;
	float GlobalScale;

	GameEngineTime();
	~GameEngineTime();
	GameEngineTime(const GameEngineTime& _Other) = delete;
	GameEngineTime(GameEngineTime&& _Other) noexcept = delete;
	GameEngineTime& operator=(const GameEngineTime& _Other) = delete;
	GameEngineTime& operator=(GameEngineTime&& _Other) noexcept = delete;

};

