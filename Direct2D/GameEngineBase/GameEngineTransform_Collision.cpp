#include "PreCompile.h"
#include "GameEngineTransform.h"
// #include <DirectXCollision.inl>


void GameEngineTransform::CollisionScaleSetting() {
	CollisionDataObject.OBB.Extents = (Data.WorldScaling * 0.5f);
}
void GameEngineTransform::CollisionRotationSetting() {
	CollisionDataObject.OBB.Orientation = Data.WorldRotation.DegreeRotationToQuaternionReturn();
}
void GameEngineTransform::CollisionPositionSetting() {
	CollisionDataObject.OBB.Center = Data.WorldPosition;
}

void GameEngineTransform::CollisionDataSetting()
{
	CollisionScaleSetting();
	CollisionRotationSetting();
	CollisionPositionSetting();
}


bool GameEngineTransform::SphereToSphere(const GameEngineTransform& _Left, const GameEngineTransform& _Right)
{
	return _Left.CollisionDataObject.SPHERE.Intersects(_Right.CollisionDataObject.SPHERE);
}

bool GameEngineTransform::AABBToAABB(const GameEngineTransform& _Left, const GameEngineTransform& _Right)
{
	return _Left.CollisionDataObject.AABB.Intersects(_Right.CollisionDataObject.AABB);
}

bool GameEngineTransform::OBBToOBB(const GameEngineTransform& _Left, const GameEngineTransform& _Right)
{
	return _Left.CollisionDataObject.OBB.Intersects(_Right.CollisionDataObject.OBB);
}