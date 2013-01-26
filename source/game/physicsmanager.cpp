#include "physicsmanager.h"
#include "../defines.h"

#define DEG2RAD		0.0174532925199432957f
#define RAD2DEG		57.295779513082320876f

#define PIX2M		0.01f
#define M2PIX		(1.0f / PIX2M)

PhysicsManager::PhysicsManager()
	: pWorld(NULL)
{
	pWorld = New(b2World(b2Vec2(0.0f, 10.0f)));
}

PhysicsManager::~PhysicsManager()
{
	this->ClearWorld();
	Delete(pWorld);
}

void PhysicsManager::Update(f32 dt)
{
	pWorld->Step(dt, 8, 3);
	pWorld->ClearForces();

	for (b2Body *b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		ISceneObject *obj = (ISceneObject *)b->GetUserData();
		if (obj != NULL)
		{
			b2Vec2 p = b->GetPosition();
			f32 a = b->GetAngle() * RAD2DEG;
			obj->SetPosition(p.x * M2PIX, p.y * M2PIX);
			obj->SetRotation(a);
		}
	}
}

void PhysicsManager::ClearWorld()
{
	for (b2Body *b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		ISceneObject *obj = (ISceneObject *)b->GetUserData();
		if (obj != NULL)
		{
//			pScene->Remove(obj);
//			Delete(obj);
			pWorld->DestroyBody(b);
		}
	}
}

b2Body* PhysicsManager::CreateBody(ISceneObject *obj)
{
	if (!obj)
		return NULL;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(obj->GetX() * PIX2M, obj->GetY() * PIX2M);
	bodyDef.angle = 0;
	bodyDef.userData = obj;
	b2Body *b = pWorld->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(obj->GetWidth() * 0.5f * PIX2M, obj->GetHeight() * 0.5f * PIX2M);

	b2FixtureDef fixDef;
	fixDef.shape = &boxShape;
	fixDef.density = 1.0f;
	fixDef.restitution = 0.10f;
	fixDef.friction = 0.0f;
	b->CreateFixture(&fixDef);

	return b;

	/*
	b2FixtureDef sensorDef;
	sensorDef.shape = &boxShape;
	sensorDef.isSensor = true;
	b->CreateFixture(&sensorDef);
	*/
}

void PhysicsManager::DestroyBody(b2Body *body)
{
	if (body)
		pWorld->DestroyBody(body);
}

void PhysicsManager::CreateStaticBody(ISceneObject *obj)
{
	if (!obj)
		return;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	// FIXME: Metadata objects nao tem o pivot no centro do objeto - corrigir na Seed durante o load dos objects.
	// Aqui estou compensando o x e y com metade da largura e altura.
	bodyDef.position.Set((obj->GetX() + (obj->GetWidth() * 0.5f)) * PIX2M, (obj->GetY() + (obj->GetHeight() * 0.5f)) * PIX2M);
	bodyDef.angle = 0.0f;
	b2Body *b = pWorld->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(obj->GetWidth() * 0.5f * PIX2M, obj->GetHeight() * 0.5f * PIX2M);

	b2FixtureDef fixDef;
	fixDef.shape = &boxShape;
	fixDef.density = 1.0f;
	b->CreateFixture(&fixDef);
}
