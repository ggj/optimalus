#include "optimistplayerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("OptimistPlayer", OptimistPlayerEntity)

OptimistPlayerEntity::OptimistPlayerEntity()
	: PlayerEntity("OptimistPlayer", "OptimistPlayer", true)
{
	sPlayer.displayName = "Optimist";
	sPlayer.iLevel = 1;
	sPlayer.iXP = 80;
	sPlayer.iAttackPower = 15;
	sPlayer.iGold = 10;
	sPlayer.iLife = 22;
	sPlayer.iStamina = 5;
}
