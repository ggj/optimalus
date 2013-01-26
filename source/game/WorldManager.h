#ifndef _WORLD_MANAGER_H
#define _WORLD_MANAGER_H

#include "../defines.h"

#include <vector>

class Entity;

class WorldManager
{
	public:
		void BuildEntity(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		~WorldManager();

	private:
		typedef std::vector<Entity *> EntitiesVector_t;
		EntitiesVector_t vEntities;
};


#endif
