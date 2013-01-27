#include "main_menu_state.h"
#include "../gameflow.h"

MainMenuState::MainMenuState()
{
	musTheme.Load("sounds/scottwills_time.ogg");
}

MainMenuState::~MainMenuState()
{
	musTheme.Unload();
}

void MainMenuState::OnStart(void *data)
{
	UNUSED(data)
	Log("Entering MainMenu State");

	gFlow->LoadGUI("gui/views/mainmenu.rml");

	if (gGameData->IsBgmEnabled() == true)
	{
//		pSoundSystem->PlayMusic(&musTheme);
	}
}

void MainMenuState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void MainMenuState::OnStop(void *data)
{
	UNUSED(data)
	Log("Exiting MainMenu State");

	musTheme.Unload();
}
