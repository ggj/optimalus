#include "CollisionSensor.h"

#include "GameScene.h"

CollisionSensor::CollisionSensor():
	pBody(NULL)
{
	//empty
}

CollisionSensor::~CollisionSensor()
{
	gPhysics->DestroyBody(pBody);
}

void CollisionSensor::Load(Seed::ISceneObject &metadata, void *userData)
{
	this->Load(metadata, false, NULL, userData);
}

void CollisionSensor::Load(Seed::ISceneObject &metadata, bool track, b2Vec2 *customSize, void *userData)
{
	pBody = gPhysics->CreateStaticBody(&metadata, BodyType::SENSOR, track, customSize);
	pBody->GetFixtureList()->SetUserData(userData);	
}

void CollisionSensor::Disable()
{
	pBody->GetFixtureList()->SetUserData(NULL);	
}
