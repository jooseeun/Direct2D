#include "PlayLevelManager.h"
#include "TopUI.h"
#include "PreCompile.h"
#include "ContentsEnum.h"

PlayLevelManager::PlayLevelManager() 
	:Map(nullptr)
{
}

PlayLevelManager::~PlayLevelManager() 
{
}

void PlayLevelManager::CreateMap(const std::string _BackGround
	, const std::string _BackObject
	, const std::string _Ground
	, const std::string _FrontObject
	, const std::string _ColMap)
{
	Map = CreateActor<MapSet>(OBJECTORDER::Player);

	Map->ColMap->SetTexture(_ColMap);
	Map->ColMap->GetTransform().SetLocalScale({
		Map->ColMap->GetCurTexture()->GetScale().x,
		Map->ColMap->GetCurTexture()->GetScale().y,
		100.0f });
	Map->ColMap->GetTransform().SetLocalPosition(
		float4{ Map->ColMap->GetCurTexture()->GetScale().x / 2,
		-Map->ColMap->GetCurTexture()->GetScale().y / 2, 300.0f });
	Map->ColMap->SetPivot(PIVOTMODE::CUSTOM);
	Map->ColMap->SetOrder((int)OBJECTORDER::ColMap);

	Map->BackGround->SetTexture(_BackGround);
	Map->BackGround->GetTransform().SetLocalScale({
		Map->BackGround->GetCurTexture()->GetScale().x,
		Map->BackGround->GetCurTexture()->GetScale().y,
		100.0f });
	Map->BackGround->GetTransform().SetLocalPosition(
		float4{ Map->BackGround->GetCurTexture()->GetScale().x / 2,
		-Map->BackGround->GetCurTexture()->GetScale().y / 2, 200.0f });
	Map->BackGround->SetPivot(PIVOTMODE::CUSTOM);
	Map->BackGround->SetOrder((int)OBJECTORDER::BackGround);

	Map->BackObject->SetTexture(_BackObject);
	Map->BackObject->GetTransform().SetLocalScale({
		Map->BackObject->GetCurTexture()->GetScale().x,
		Map->BackObject->GetCurTexture()->GetScale().y,
		100.0f });
	Map->BackObject->GetTransform().SetLocalPosition(
		float4{ Map->BackObject->GetCurTexture()->GetScale().x / 2,
		-Map->BackObject->GetCurTexture()->GetScale().y / 2, 100.0f });
	Map->BackObject->SetPivot(PIVOTMODE::CUSTOM);
	Map->BackObject->SetOrder((int)OBJECTORDER::BackGroundObject);

	Map->Ground->SetTexture(_Ground);
	Map->Ground->GetTransform().SetLocalScale({
		Map->Ground->GetCurTexture()->GetScale().x,
		Map->Ground->GetCurTexture()->GetScale().y,
		100.0f });
	Map->Ground->GetTransform().SetLocalPosition(
		float4{ Map->Ground->GetCurTexture()->GetScale().x / 2,
		-Map->Ground->GetCurTexture()->GetScale().y / 2, -50.0f });
	Map->Ground->SetPivot(PIVOTMODE::CUSTOM);
	Map->Ground->SetOrder((int)OBJECTORDER::Ground);

	Map->FrontObject->SetTexture(_FrontObject);
	Map->FrontObject->GetTransform().SetLocalScale({
		Map->FrontObject->GetCurTexture()->GetScale().x,
		Map->FrontObject->GetCurTexture()->GetScale().y,
		100.0f });
	Map->FrontObject->GetTransform().SetLocalPosition(
		float4{ Map->FrontObject->GetCurTexture()->GetScale().x / 2,
		-Map->FrontObject->GetCurTexture()->GetScale().y / 2, -100.0f });
	Map->FrontObject->SetPivot(PIVOTMODE::CUSTOM);
	Map->FrontObject->SetOrder((int)OBJECTORDER::FrontObject);



}