#include "EntityFactory.h"

#include <map>
#include <string>

#include <Log.h>

namespace
{
	typedef std::map<std::string, EntityFactory::CreateEntityProc_t> EntityCreatorMap_t;

	static EntityCreatorMap_t &GetCreatorMap()
	{
		static EntityCreatorMap_t mapCreators;

		return mapCreators;
	}
}

void EntityFactory::AddCreator(const char *name, CreateEntityProc_t proc)
{
	GetCreatorMap().insert(std::make_pair(name, proc));
}

Entity *EntityFactory::CreateEntity(const std::string &name)
{
	auto &mapCreators = GetCreatorMap();

	auto it = mapCreators.find(name);
	if(it == mapCreators.end())
	{
		Log("Entity %s not found.", name);

		return nullptr;
	}

	return it->second();
}

Entity *EntityFactory::CreateEntity(const char *name)
{
	return CreateEntity(std::string(name));
}
