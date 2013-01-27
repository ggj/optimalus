#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <Seed.h>
#include <string>

#include "../defines.h"

class CollisionEvent;

class Entity
{
	public:
		Entity();
		virtual ~Entity();

        inline const String &GetName() const;

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites) = 0;

		virtual void OnCollision(const CollisionEvent &event) {};

    protected:
        String strName;

	private:
		SEED_DISABLE_COPY(Entity);
};

const String &Entity::GetName() const
{
    return strName;
}

#endif
