#include "staminapotionentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("StaminaPotion", StaminaPotionEntity)

StaminaPotionEntity::StaminaPotionEntity()
	: ItemEntity("StaminaPotion", "StaminaPotion")
{
}
