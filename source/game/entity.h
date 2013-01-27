#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <Seed.h>
#include <string>

#include "../defines.h"

class CollisionEvent;

class Entity
{
	public:
		Entity(const String &className);
		virtual ~Entity();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);

		virtual void OnCollision(const CollisionEvent &event) { UNUSED(event) }

		virtual void Update(f32 dt) { UNUSED(dt) }

		inline const String &GetClassName() const;
		inline const String &GetName() const;

		virtual void Activate() {}

	protected:

	private:
		SEED_DISABLE_COPY(Entity);

		String strClassName;
		String strName;
};

const String &Entity::GetClassName() const
{
	return strClassName;
}

const String &Entity::GetName() const
{
	return strName;
}

#endif
