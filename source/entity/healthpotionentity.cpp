#include "healthpotionentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("HealthPotion", HealthPotionEntity)

HealthPotionEntity::HealthPotionEntity()
	: ItemEntity("HealthPotion", "HealthPotion")
{
}
