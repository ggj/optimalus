#include "game_pause_state.h"
#include "../gameflow.h"
#include "../game/GameScene.h"

GamePauseState::GamePauseState()
{
}

GamePauseState::~GamePauseState()
{
}

void GamePauseState::OnStart(void *data)
{
	Log("Entering GamePause State");
	gFlow->LoadGUI("gui/views/gamepause.rml");
	GameScene *g = static_cast<GameScene *>(data);
	g->Pause();
}

void GamePauseState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void GamePauseState::OnStop(void *data)
{
	UNUSED(data)
	Log("Exiting GamePause State");
}
