#include "pessimistplayerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("PessimistPlayer", PessimistPlayerEntity)

PessimistPlayerEntity::PessimistPlayerEntity()
	: PlayerEntity("PessimistPlayer", "PessimistPlayer", false)
{
}
