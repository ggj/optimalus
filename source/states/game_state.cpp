#include "game_state.h"
#include "../guimanager.h"
#include "../gameflow.h"
#include "../game/gamescene.h"

GameState::GameState()
	: pGame(NULL)
	, bDoStop(false)
{
}

GameState::~GameState()
{
}

void GameState::OnStart(void *data)
{
	UNUSED(data)
	Log("Entering Game State");
	bDoStop = false;

	gGui->LoadGUI("gui/views/game.rml");
	pGame = New(GameScene(gFlow->GetScene(), gFlow->GetCamera(), gFlow->GetSceneFile()));
	pGame->Initialize();
}

void GameState::OnUpdate(f32 dt)
{
	pGame->Update(dt);
}

void GameState::OnStop(void *data)
{
	UNUSED(data)
	Log("Exiting Game State");
	bDoStop = true;
}

/*
We need this because we will mess with the SceneGraph from an unexpected event (ie. user input)
and the Renderer may be traversing the graph, so we need be sure that the destruction will happen
inside an update.
*/
void GameState::LateStop()
{
	if (bDoStop && pGame)
	{
		pGame->Shutdown();
		Delete(pGame);

		// Reset da posicao da camera quando sai do jogo...
		// salvar a posicao original?
		gFlow->GetCamera()->SetPosition(-400.0f, -300.0f);
		gFlow->DoLoad();
	}
}
