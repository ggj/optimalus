#ifndef _WORLD_MANAGER_H
#define _WORLD_MANAGER_H

namespace Seed
{
	class IMetadataObject;
}

#include <vector>

class Entity;

class WorldManager
{
	public:
		void BuildEntity(Seed::IMetadataObject &metadata);

	private:
		std::vector<Entity *> vEntities;
};


#endif
