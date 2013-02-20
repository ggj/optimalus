#include "gameflow.h"
#include "scene/gamescene.h"
#include "manager/guimanager.h"

#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

#define GAME_ID	0xF0000001

GameFlow *gFlow = NULL;
GameData *gGameData = NULL;

GameFlow::GameFlow()
	: pScene(NULL)
	, pCamera(NULL)
	, sSceneFile("")
{
	gFlow = this;
}

GameFlow::~GameFlow()
{
	gFlow = NULL;
}

bool GameFlow::Initialize()
{
	pSoundSystem->SetMusicVolume(0.6f);
	pSoundSystem->SetSfxVolume(0.5f);

	bool init = cPres.Load("configs/game.config", this);

	// Create the State Machine Data
	gGameData = New(GameData());

	if (this->SaveSystemFlow())
		pSaveSystem->Load(0, &gGameData->sPlayer, &gGameData->sOptions);

	// Create the transitions
	cMenuToGame.Initialize(&cMenu, &cOnGame, &cGame);
	cMenuToOptions.Initialize(&cMenu, &cOnOptions, &cOptions);
	cMenuToCredits.Initialize(&cMenu, &cOnCredits, &cCredits);
	cOptionsToMenu.Initialize(&cOptions, &cOnMenu, &cMenu);
	cCreditsToMenu.Initialize(&cCredits, &cOnMenu, &cMenu);
	cGameToMenu.Initialize(&cGame, &cOnMenu, &cMenu);
	cGameToLoad.Initialize(&cGame, &cOnLoad, &cLoad);
	cLoadToGame.Initialize(&cLoad, &cOnGame, &cGame);

	// Create the State Machine.
	cFlow.RegisterTransition(&cMenuToGame);
	cFlow.RegisterTransition(&cMenuToOptions);
	cFlow.RegisterTransition(&cMenuToCredits);
	cFlow.RegisterTransition(&cOptionsToMenu);
	cFlow.RegisterTransition(&cCreditsToMenu);
	cFlow.RegisterTransition(&cGameToMenu);
	cFlow.RegisterTransition(&cGameToLoad);
	cFlow.RegisterTransition(&cLoadToGame);

	IGameApp::Initialize();

	return init;
}

bool GameFlow::Update(f32 dt)
{
	cFlow.Update(dt);
	cGame.LateStop();
	return true;
}

bool GameFlow::Shutdown()
{
	cMenu.OnStop(NULL);
	pSaveSystem->Save(0, &gGameData->sPlayer, &gGameData->sOptions);

	if (cFlow.GetCurrentState() == &cGame)
	{
		cGame.OnStop(NULL);
		cGame.LateStop();
	}

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	pScene->Remove(gGui->GetSceneObject());
	gGui->Shutdown();
	Delete(gGui);

	cPres.Unload();

	Delete(gGameData);

	return IGameApp::Shutdown();
}

void GameFlow::Credits()
{
	cFlow.OnEvent(&cOnCredits);
}

void GameFlow::Options()
{
	cFlow.OnEvent(&cOnOptions);
}

void GameFlow::Menu()
{
	cFlow.OnEvent(&cOnMenu);
}

void GameFlow::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void GameFlow::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape && cFlow.GetCurrentState() == &cMenu)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
	else if (k == Seed::KeyF4)
		pScene->Dump();
	else if (k == Seed::KeyF5)
		gGui->ReloadGUI();
	else if (k == Seed::KeyF10)
		Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
}

void GameFlow::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	pScene = cPres.GetRendererByName("MainRenderer")->GetScene();
	Viewport *viewport = cPres.GetViewportByName("MainView");

	pCamera = viewport->GetCamera();

	New(GuiManager());
	gGui->Initialize();
	pScene->Add(gGui->GetSceneObject());

	cFlow.Initialize(&cMenu);
}

void GameFlow::LoadSceneFile(const String &file)
{
	sSceneFile = file;
	cFlow.OnEvent(&cOnLoad);
}

const String &GameFlow::GetSceneFile() const
{
	return sSceneFile;
}

void GameFlow::DoLoad(const String &scene)
{
	if (scene != "")
		sSceneFile = scene;

	if (sSceneFile != "")
	{
		cFlow.OnEvent(&cOnGame);
		sSceneFile = "";
	}
}

bool GameFlow::SaveSystemFlow() const
{
	GameData data;
	pSaveSystem->SetTotalSlots(4);

	eCartridgeError error = pSaveSystem->Initialize(Seed::Cartridge262144b);
	if (error == Seed::ErrorNone)
		error = pSaveSystem->Prepare(GAME_ID, &data.sPlayer, sizeof(data.sPlayer), &data.sOptions, sizeof(data.sOptions));

	if (error == Seed::ErrorDeviceFull)
	{
		Log("Not enough space available on device.");
		return false;
	}

	if (error == Seed::ErrorInodeFull)
	{
		Log("Not enough inodes available on device.");
		return false;
	}

	if (error == Seed::ErrorNoCard || error == Seed::ErrorAccessDenied)
	{
		Log("Unknown file system error - no card or access denied - system hungup");
		pSystem->HangUp();
		return false;
	}

	if (error == Seed::ErrorNotFormatted)
	{
		Log("Save file doesn't exist or corrupted, creating one now.");
		error = pSaveSystem->FormatCard(&data.sPlayer, &data.sOptions);
	}

	if (error == Seed::ErrorDataCorrupt)
	{
		Log("One or more saved games were corrupted and had to be reset");
		error = Seed::ErrorNone;
	}

	if (error == Seed::ErrorFilesystemCorrupt)
	{
		Log("File system became corrupted - system hungup");
		pSystem->HangUp();
		return false;
	}

	if (error == Seed::ErrorNone)
	{
		Log("Save data CRC ok.");
		// Do your initial loading here.
		return true;
	}

	return false;
}
