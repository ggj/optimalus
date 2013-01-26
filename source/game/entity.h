#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <Seed.h>
#include <string>

#include "../defines.h"

class Entity
{
	public:
		Entity();
		virtual ~Entity();

        inline const String &GetName() const;

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites) = 0;

    protected:
        String strName;
};

const String &Entity::GetName() const
{
    return strName;
}

#endif
