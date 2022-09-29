#pragma once

enum class OBJECTORDER
{
	ColMap,
	BackGround,
	BackGroundObject,
	LightEffect,
	Ground,
	Player,
	Monster,
	Skill,
	FrontObject,
	StopObject,
	TrapObject,
	Coin=992,
	MoveCol1 = 993,
	MoveCol2 = 994,
	MoveCol3 = 995,
	MoveCol4 = 996,
	GameStartUI=997,
	GameSetUI=998,
	GameEndUI=999,
	UI=1000,
	Mouse=1111
};

enum class PLAYERDIR
{
	Right,
	Left
};
enum class MonsterDIR
{
	Right,
	Left
};

