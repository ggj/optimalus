#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "../defines.h"
#include <Box2D/Box2D.h>

#include <map>

class Entity;

namespace BodyType
{
	enum Enum
	{
		NORMAL,
		SENSOR
	};
}

namespace CollisionEventType
{
	enum Enum
	{
		ON_ENTER,
		ON_LEAVE
	};
}

class CollisionEvent
{
	public:
		CollisionEvent(CollisionEventType::Enum type, b2Body &targetBody, Entity &target, b2Body &otherBody, Entity *otherEntity);

		inline CollisionEventType::Enum GetType() const;
		inline b2Body &GetOtherBody() const;
		inline Entity *GetOtherEntity() const;
		inline Entity &GetTarget() const;
		inline b2Body &GetTargetBody() const;

	private:
		CollisionEventType::Enum eType;
		b2Body	&rOtherBody;
		b2Body  &rTargetBody;
		Entity	*pOtherEntity;
		Entity  &rTarget;
};

inline CollisionEventType::Enum CollisionEvent::GetType() const
{
	return eType;
}

inline b2Body &CollisionEvent::GetTargetBody() const
{
	return rTargetBody;
}

inline Entity &CollisionEvent::GetTarget() const
{
	return rTarget;
}

inline b2Body &CollisionEvent::GetOtherBody() const
{
	return rOtherBody;
}

inline Entity *CollisionEvent::GetOtherEntity() const
{
	return pOtherEntity;
}

class PhysicsManager: public b2ContactListener
{
	public:
		PhysicsManager();
		~PhysicsManager();

		void Update(f32 dt);
		b2Body* CreateBody(ISceneObject *obj, b2Vec2 *customSize = NULL);
		void DestroyBody(b2Body *body);
		b2Body* CreateStaticBody(ISceneObject *obj, BodyType::Enum type = BodyType::NORMAL, bool track = false, b2Vec2 *customSize = NULL);

		bool RayCast(b2Body *startingBody, b2Vec2 relativeDest);

	private:
		void ClearWorld();

		void AddContact(b2Fixture *fixture, b2Body *body, b2Fixture *otherFixture, b2Body *other);
		void RemoveContact(b2Fixture *fixture, b2Body *body, b2Fixture *otherFixture, b2Body *other);

		void ClearContacts(b2Body *body);		
					
		//Contact listener
		virtual void BeginContact(b2Contact *contact);

		virtual void EndContact(b2Contact *contact);

	private:
		b2World *pWorld;

		typedef std::map<b2Body*, int> CollisionCounterMap_t;

		typedef std::map<b2Body*, CollisionCounterMap_t>  CollisionCache_t;
		CollisionCache_t mapCollisions;

		typedef std::list<CollisionEvent> CollisionEventList_t;
		CollisionEventList_t lstEvents;

		f32		fpTimeLeft;

};

#endif // PHYSICSMANAGER_H
