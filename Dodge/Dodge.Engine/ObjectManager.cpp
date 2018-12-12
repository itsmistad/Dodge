#include "Headers.h"
#include "ObjectManager.h"
#include "Menu.h"
#include "MouseController.h"

ObjectManager * ObjectManager::Current = NULL;
Player * ObjectManager::You = NULL;
int ObjectManager::TotalTargets = 0;
int ObjectManager::TargetsKilled = 0;

ObjectManager::ObjectManager()
{
	mapObject = NULL;
}

void ObjectManager::SetMap(Object * object)
{
	mapObject = object;
}

void ObjectManager::Create(Player * player)
{
	TotalTargets++;
	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);
	EffectPipelineStateDescription pdAlpha(
		nullptr,
		CommonStates::AlphaBlend,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);
	player->SetNormals(pd, pdAlpha);
	players.push_back(player);
}

void ObjectManager::Create(Throwable * throwable)
{
	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);
	EffectPipelineStateDescription pdAlpha(
		nullptr,
		CommonStates::AlphaBlend,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);
	throwable->SetNormals(pd, pdAlpha);
	throwables.push_back(throwable);
}

void ObjectManager::Create(PowerUp * powerUp)
{
	//powerUp->SetNormals(*psd, *psdAlpha);
	powerUps.push_back(powerUp);
}

void ObjectManager::SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha)
{
	if (mapObject != NULL)
		mapObject->SetNormals(psd, psdAlpha);
	for (auto player : players)
		player->SetNormals(psd, psdAlpha);
}

void ObjectManager::Update(float deltaTime)
{
	Cull();
	if (mapObject != NULL)
		mapObject->Update();
	for (auto player : players)
		if (player->isDead)
			playersToCull.push_back(player);
		else
			player->Update(deltaTime);
	for (auto throwable : throwables)
		if (throwable->isDead)
			throwablesToCull.push_back(throwable);
		else
			throwable->Update(deltaTime, players);
	for (auto powerUp : powerUps)
		if (powerUp->isDead)
			powerUpsToCull.push_back(powerUp);
		else
			powerUp->Update();
}

void ObjectManager::Draw(Matrix viewMatrix, Matrix perspectiveMatrix)
{
	if (mapObject != NULL)
		mapObject->Draw(viewMatrix, perspectiveMatrix);
	for (auto player : players)
		player->Draw(viewMatrix, perspectiveMatrix);
	for (auto throwable : throwables)
		throwable->Draw(viewMatrix, perspectiveMatrix);
	for (auto powerUp : powerUps)
		powerUp->Draw(viewMatrix, perspectiveMatrix);
}
void ObjectManager::ResetField()
{
	for (auto player : Current->players)
		Current->playersToCull.push_back(player);
	for (auto player : Current->playersToCull)
	{
		player->Reset();
		Current->players.erase(remove(Current->players.begin(), Current->players.end(), player), Current->players.end());
		delete player;
	}
	for (auto throwable : Current->throwables)
		Current->throwablesToCull.push_back(throwable);
	for (auto powerUp : Current->powerUps)
		Current->powerUpsToCull.push_back(powerUp);
	Current->playersToCull.clear();
}

void ObjectManager::Reset()
{
	if (mapObject != NULL)
		mapObject->Reset();
	for (auto player : Current->players)
		Current->playersToCull.push_back(player);
	for (auto throwable : Current->throwables)
		Current->throwablesToCull.push_back(throwable);
	for (auto powerUp : Current->powerUps)
		Current->powerUpsToCull.push_back(powerUp);

	Current->Cull();
}

void ObjectManager::Cull()
{
	for (auto player : playersToCull)
	{
		TargetsKilled++;
		player->Reset();
		players.erase(remove(players.begin(), players.end(), player), players.end());
		delete player;
	}
	for (auto throwable : throwablesToCull)
	{
		throwable->Reset();
		throwables.erase(remove(throwables.begin(), throwables.end(), throwable), throwables.end());
		delete throwable;
	}
	for (auto powerUp : powerUpsToCull)
	{
		powerUp->Reset();
		powerUps.erase(remove(powerUps.begin(), powerUps.end(), powerUp), powerUps.end());
		delete powerUp;
	}
	playersToCull.clear();
	throwablesToCull.clear();
	powerUpsToCull.clear();
	if (TargetsKilled == TotalTargets)
	{
		TargetsKilled = 0;
		if (Menu::currentMenu != NULL)
			Menu::currentMenu->Hide();
		Menu::all["gameover"]->Show();
		MouseController::Current->SetLock(false);
	}
}
