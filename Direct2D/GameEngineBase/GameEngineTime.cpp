#include "PreCompile.h"
#include "GameEngineTime.h"


GameEngineTime* GameEngineTime::Inst_ = new GameEngineTime();


GameEngineTime::GameEngineTime() 
	: GlobalScale(1.0f)
	, FrameLimit(- 1)
	, FrameUpdate(true)
{
	Reset();
}

GameEngineTime::~GameEngineTime() 
{
}

void GameEngineTime::Reset() 
{
	SumFPS = 0;
	SumFPSCount = 0;
	FrameCheckTime = 1.0f;
	Prev = std::chrono::steady_clock::now();
	Update();
}

void GameEngineTime::Update() 
{
	std::chrono::steady_clock::time_point Current = std::chrono::steady_clock::now();

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(Current - Prev);

	DeltaTimed = time_span.count();

	Prev = Current;
	// ���� �ٸ� �������� �ٲܶ� ���
	DeltaTimef = static_cast<float>(DeltaTimed);

	// -0.0001

	FrameCheckTime -= DeltaTimef;

	FrameUpdate = false;

	CurFrameTime -= DeltaTimef;

	if (FrameLimit == -1)
	{
		FrameUpdate = true;

		if (DeltaTimef >= 0.00001f)
		{
			++SumFPSCount;
			SumFPS += static_cast<int>(1.0f / DeltaTimef);

			if (0 >= FrameCheckTime)
			{
				FPS = SumFPS / SumFPSCount;
				FrameCheckTime = 1.0f;
				SumFPSCount = 0;
				SumFPS = 0;
			}
		}
	}
	else if(CurFrameTime <= 0.0f)
	{
		FrameUpdate = true;
		++SumFPSCount;

		if (0 >= FrameCheckTime)
		{
			FPS = SumFPSCount;
			FrameCheckTime = 1.0f;
			SumFPS = 0;
			SumFPSCount = 0;
		}
		CurFrameTime += FrameTime;
		//	FPS = SumFPS / SumFPSCount;
		//	FrameCheckTime = 1.0f;
		//	SumFPSCount = 0;
		//	SumFPS = 0;
		//}

		//SumDeltaTime = 0.0f;
		//CurFrameTime += FrameTime;
	}


}