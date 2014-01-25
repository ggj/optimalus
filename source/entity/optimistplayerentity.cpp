#include "optimistplayerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("OptimistPlayer", OptimistPlayerEntity)

OptimistPlayerEntity::OptimistPlayerEntity()
	: PlayerEntity("OptimistPlayer", "OptimistPlayer", true)
{
}
