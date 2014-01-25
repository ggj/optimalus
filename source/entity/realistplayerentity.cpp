#include "realistplayerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("RealistPlayer", RealistPlayerEntity)

RealistPlayerEntity::RealistPlayerEntity()
	: PlayerEntity("RealistPlayer", "RealistPlayer", false)
{
}
