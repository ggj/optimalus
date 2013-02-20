#include "run_state.h"
#include "../../manager/guimanager.h"
#include "../../scene/gamescene.h"

RunState::RunState()
{
}

RunState::~RunState()
{
}

void RunState::OnStart(void *data)
{
	Log("Entering Run State");
	gGui->LoadGUI("gui/views/game.rml");

	if (data)
	{
		GameScene *g = static_cast<GameScene *>(data);
		g->Resume();
	}
}

void RunState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void RunState::OnStop(void *data)
{
	UNUSED(data)
	Log("Exiting Run State");
}
