#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <LeakReport.h>

namespace
{
	EntityFactory::AutoCreator acPlayer("Player", []() -> Entity * { return new PlayerEntity(); });
}

PlayerEntity::PlayerEntity()
{
	//empty
}
