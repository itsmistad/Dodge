#pragma once
#include "Throwable.h"
#include "PowerUp.h"
#include "Player.h"
#include "Camera.h"

class ObjectManager
{
private:
	vector<Player*> players, playersToCull;
	vector<Throwable*> throwables, throwablesToCull;
	vector<PowerUp*> powerUps, powerUpsToCull;
	Object * mapObject;
public:
	static Player * You;
	static int TargetsKilled, TotalTargets;
	static ObjectManager * Current;
	
	ObjectManager();
	void SetMap(Object * object);
	void Create(Player * player);
	void Create(Throwable * throwable);
	void Create(PowerUp * powerUp);
	void SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha);
	void Update(float deltaTime);
	void Draw(Matrix viewMatrix, Matrix perspectiveMatrix);
	void Reset();
	void Cull();
	static void ResetField();
};