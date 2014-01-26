#include "realistplayerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("RealistPlayer", RealistPlayerEntity)

RealistPlayerEntity::RealistPlayerEntity()
	: PlayerEntity("RealistPlayer", "RealistPlayer", false)
{
	sPlayer.displayName = "Realist";
	sPlayer.iLevel = 1;
	sPlayer.iXP = 100;
	sPlayer.iAttackPower = 15;
	sPlayer.iGold = 0;
	sPlayer.iLife = 22;
	sPlayer.iStamina = 10;
}
