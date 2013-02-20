#ifndef _ENTITY_H_
#define _ENTITY_H_

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
		inline const String &GetTarget() const;

		virtual void Activate() {}

	protected:
		void DoActivateAll();

	private:
		SEED_DISABLE_COPY(Entity);

		String strClassName;
		String strName;
		String strTarget;
};

const String &Entity::GetClassName() const
{
	return strClassName;
}

const String &Entity::GetName() const
{
	return strName;
}

inline const String &Entity::GetTarget() const
{
	return strTarget;
}

#endif
