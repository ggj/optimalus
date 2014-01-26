#include "manapotionentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("ManaPotion", ManaPotionEntity)

ManaPotionEntity::ManaPotionEntity()
	: ItemEntity("ManaPotion", "ManaPotion")
{
}
