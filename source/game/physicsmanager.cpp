#include "physicsmanager.h"
#include "../defines.h"

#include "entity.h"

#define DEG2RAD		0.0174532925199432957f
#define RAD2DEG		57.295779513082320876f

#define PIX2M		0.01f
#define M2PIX		(1.0f / PIX2M)

PhysicsManager::PhysicsManager()
	: pWorld(NULL)
{
	pWorld = New(b2World(b2Vec2(0.0f, 10.0f)));

	pWorld->SetContactListener(this);
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

	while(!lstEvents.empty())
	{
		CollisionEvent event = *lstEvents.begin();
		lstEvents.pop_front();

		event.GetTarget().OnCollision(event);
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
	fixDef.restitution = 0.75f;
	b->CreateFixture(&fixDef);

	return b;
}

void PhysicsManager::DestroyBody(b2Body *body)
{
	this->ClearContacts(body);
	pWorld->DestroyBody(body);
}

b2Body* PhysicsManager::CreateStaticBody(ISceneObject *obj, BodyType::Enum type)
{
	if (!obj)
		return NULL;

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
	fixDef.isSensor = type == BodyType::SENSOR;	
	b->CreateFixture(&fixDef);

	return b;
}

void PhysicsManager::AddContact(b2Fixture *fixture, b2Body *body, b2Fixture *otherFixture, b2Body *other)
{
	CollisionCache_t::iterator it = mapCollisions.lower_bound(body);
	if((it != mapCollisions.end()) && !(mapCollisions.key_comp()(body, it->first)))	
	{
		//found		
	}
	else
	{
		//not found, so insert new body in the cache
		it = mapCollisions.insert(it, std::make_pair(body, CollisionCounterMap_t()));
	}

	CollisionCounterMap_t &counter = it->second;

	//Now check if the body in the cache is colliding with the other body
	CollisionCounterMap_t::iterator counterIt = counter.lower_bound(other);
	if((counterIt != counter.end()) && !(counter.key_comp()(body, counterIt->first)))	
	{
		//Already exists, increment counter
		counterIt->second++;
	}
	else
	{
		//Does not exists, insert
		counter.insert(counterIt, std::make_pair(other, 1));

		lstEvents.push_back(CollisionEvent(
			CollisionEventType::ON_ENTER, 
			*body, 
			*reinterpret_cast<Entity*>(fixture->GetUserData()),
			*other,
			reinterpret_cast<Entity*>(otherFixture->GetUserData())
		));
	}
}

void PhysicsManager::RemoveContact(b2Fixture *fixture, b2Body *body, b2Fixture *otherFixture, b2Body *other)
{
	CollisionCache_t::iterator it = mapCollisions.find(body);
	if(it == mapCollisions.end())
	{
		Log("[PhysicsManager::RemoveContact] Warning: Body already removed");

		return;
	}

	CollisionCounterMap_t::iterator counterIt = it->second.find(other);
	if(counterIt == it->second.end())
	{
		Log("[PhysicsManager::RemoveContact] Warning: Other already removed");
	}

	counterIt->second--;
	if(counterIt->second == 0)
	{
		lstEvents.push_back(CollisionEvent(
			CollisionEventType::ON_LEAVE, 
			*body, 
			*reinterpret_cast<Entity*>(fixture->GetUserData()),
			*other,
			reinterpret_cast<Entity*>(otherFixture->GetUserData())
		));


		Log("OnLeave");

		it->second.erase(counterIt);
		if(it->second.empty())
		{
			mapCollisions.erase(it);
		}
	}
}

void PhysicsManager::ClearContacts(b2Body *body)
{
	mapCollisions.erase(body);
	
	for(CollisionCache_t::iterator it = mapCollisions.begin(), end = mapCollisions.end(); it != end; )
	{
		CollisionCache_t::iterator original = it++;

		original->second.erase(body);
		if(original->second.empty())
			mapCollisions.erase(original);
	}

	//
	//Clear events
	CollisionEventList_t::iterator next;
	for(CollisionEventList_t::iterator it = lstEvents.begin(), end = lstEvents.end();it != end;it = next)
	{
		next = it++;

		if(&it->GetTargetBody() == body || &it->GetOtherBody() == body)
		{
			lstEvents.erase(it);			
		}				
	}
}

void PhysicsManager::BeginContact(b2Contact *contact)
{
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();

	b2Body *a = fixtureA->GetBody();
	b2Body *b = fixtureB->GetBody();

	if(fixtureA->GetUserData())
	{
		this->AddContact(fixtureA, a, fixtureB, b);
	}

	if(fixtureB->GetUserData())
	{
		this->AddContact(fixtureB, b, fixtureA, a);
	}
}

void PhysicsManager::EndContact(b2Contact *contact)
{
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();

	b2Body *a = fixtureA->GetBody();
	b2Body *b = fixtureB->GetBody();

	if(fixtureA->GetUserData())
	{
		this->RemoveContact(fixtureA, a, fixtureB, b);
	}

	if(fixtureB->GetUserData())
	{
		this->RemoveContact(fixtureB, b, fixtureA, a);
	}
}


CollisionEvent::CollisionEvent(CollisionEventType::Enum type, b2Body &targetBody, Entity &target, b2Body &otherBody, Entity *otherEntity):
	eType(type),
	rTargetBody(targetBody),
	rTarget(target),
	rOtherBody(otherBody),
	pOtherEntity(otherEntity)
{
	//
}


