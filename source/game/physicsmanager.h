#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "../defines.h"
#include <Box2D/Box2D.h>

class Entity;
class CollisionEvent;

namespace BodyType
{
	enum Enum
	{
		Normal,
		Sensor
	};
}

namespace CollisionEventType
{
	enum Enum
	{
		OnEnter,
		OnLeave
	};
}

typedef std::map<b2Body *, int> CollisionCounterMap;
typedef std::map<b2Body *, CollisionCounterMap>  CollisionCacheMap;
typedef std::list<CollisionEvent> CollisionEventList;

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
		b2Body &rOtherBody;
		b2Body &rTargetBody;
		Entity *pOtherEntity;
		Entity &rTarget;
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
		virtual ~PhysicsManager();

		void Update(f32 dt);

		b2Body *CreateBody(ISceneObject *obj, b2Vec2 *customSize = NULL);
		b2Body *CreateStaticBody(ISceneObject *obj, BodyType::Enum type = BodyType::Normal, bool track = false, b2Vec2 *customSize = NULL);
		void DestroyBody(b2Body *body);

		bool RayCast(b2Body *startingBody, b2Vec2 relativeDest);

	protected:
		//Contact listener
		virtual void BeginContact(b2Contact *contact);
		virtual void EndContact(b2Contact *contact);

	private:
		void ClearWorld();
		void AddContact(b2Fixture *fixture, b2Body *body, b2Fixture *otherFixture, b2Body *other);
		void RemoveContact(b2Fixture *fixture, b2Body *body, b2Fixture *otherFixture, b2Body *other);
		void ClearContacts(b2Body *body);

	private:
		b2World *pWorld;
		CollisionCacheMap mapCollisions;
		CollisionEventList lstEvents;
		f32 fpTimeLeft;
};

#endif // PHYSICSMANAGER_H
