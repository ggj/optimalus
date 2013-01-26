#include "HeartEntity.h"

#include "EntityFactory.h"

ENTITY_CREATOR("Heart", HeartEntity)

HeartEntity::HeartEntity():
	ItemEntity("Heart")
{
	//empty
}

