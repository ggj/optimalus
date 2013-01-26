#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <LeakReport.h>

ENTITY_CREATOR("Player", PlayerEntity);

PlayerEntity::PlayerEntity()
{
	//empty
}
