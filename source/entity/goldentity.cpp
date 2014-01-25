#include "goldentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("Gold", GoldEntity)

GoldEntity::GoldEntity()
	: ItemEntity("Gold", "Gold")
{
}
