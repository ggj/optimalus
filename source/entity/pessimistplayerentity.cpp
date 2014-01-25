#include "pessimistplayerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("PessimistPlayer", PessimistPlayerEntity)

PessimistPlayerEntity::PessimistPlayerEntity()
	: PlayerEntity("PessimistPlayer", "PessimistPlayer", false)
{
	sPlayer.displayName = "Pessimist";
	sPlayer.iLevel = 1;
	sPlayer.iXP = 200;
	sPlayer.iAttackPower = 10;
	sPlayer.iGold = 0;
	sPlayer.iLife = 12;
	sPlayer.iMana = 20;
}

