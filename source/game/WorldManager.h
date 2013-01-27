#ifndef _WORLD_MANAGER_H
#define _WORLD_MANAGER_H

#include "../defines.h"
#include <vector>

class Entity;

class WorldManager
{
	public:
        Entity* BuildEntity(IMetadataObject &metadata, SceneNode *sprites);
		~WorldManager();

		void Clear();

		void Update(f32 dt);

		Entity *FindEntityByClassName(const char *className);
		Entity *FindEntityByName(const char *name);

		void ActivateAllEntites(const char *name);

	private:
		typedef std::vector<Entity *> EntitiesVector_t;
		EntitiesVector_t vEntities;
};


#endif
