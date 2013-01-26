#include "game_run_state.h"
#include "../gameflow.h"
#include "../game/GameScene.h"

GameRunState::GameRunState()
{
}

GameRunState::~GameRunState()
{
}

void GameRunState::OnStart(void *data)
{
	Log("Entering GameRun State");
	gFlow->LoadGUI("gui/views/game.rml");

	if (data)
	{
		GameScene *g = static_cast<GameScene *>(data);
		g->Resume();
	}
}

void GameRunState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void GameRunState::OnStop(void *data)
{
	UNUSED(data)
	Log("Exiting GameRun State");
}
