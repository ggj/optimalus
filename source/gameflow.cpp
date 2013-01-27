#include "gameflow.h"
#include "game/GameScene.h"

#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

#define GAME_ID	0xF0000001

GameFlow *gFlow = NULL;
GameData *gGameData = NULL;

GameFlow::GameFlow()
	: pScene(NULL)
	, pCamera(NULL)
	, pRocket(NULL)
	, pContext(NULL)
	, pDoc(NULL)
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
	pScreen->EnableCursor(true);
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
	pSaveSystem->Save(0, &gGameData->sPlayer, &gGameData->sOptions);

	if (cFlow.GetCurrentState() == &cGame)
	{
		cGame.OnStop(NULL);
		cGame.LateStop();
	}

	RocketEventManager::RemoveListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	this->ReleaseGUI();
	cPres.Unload();

	Delete(gGameData);

	return IGameApp::Shutdown();
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
//	if (k == Seed::KeyEscape && cFlow.GetCurrentState() == &cGame)
//		cFlow.OnEvent(&cOnGamePause);
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
	else if (k == Seed::KeyF4)
		pScene->Dump();
	else if (k == Seed::KeyF5)
		this->ReloadGUI();
	else if (k == Seed::KeyF10)
		Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
}

void GameFlow::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	RocketEventManager::AddListener(this);

	pScene = cPres.GetRendererByName("MainRenderer")->GetScene();
	Viewport *viewport = cPres.GetViewportByName("MainView");

	pCamera = viewport->GetCamera();

	this->InitializeGUI();
	cFlow.Initialize(&cMenu);

}


bool GameFlow::UnloadGUI()
{
	Log("Unloading GUI Document");
	if (pDoc)
	{
		pDoc->Hide();
		pContext->UnloadDocument(pDoc);
		pDoc->RemoveReference();
		pDoc = NULL;
	}

	return true;
}

bool GameFlow::ReloadGUI()
{
	return this->LoadGUI(sDocument);
}

bool GameFlow::LoadGUI(const String &doc)
{
	if (this->UnloadGUI())
	{
		Log("Loading GUI Document");
		pDoc = pContext->LoadDocument(doc.c_str());
		if (pDoc != NULL)
		{
			Rocket::Core::Element *title = pDoc->GetElementById("title");
			if (title != NULL)
				title->SetInnerRML(pDoc->GetTitle());

			pDoc->Focus();
			pDoc->Show();

			if (pDoc->GetElementById("lifes") != NULL)
				pElementLife = pDoc->GetElementById("lifes");

			if (pDoc->GetElementById("time") != NULL)
				pElementTime = pDoc->GetElementById("time");

			if (pDoc->GetElementById("hostages") != NULL)
				pElementHostage = pDoc->GetElementById("hostages");

			if (pDoc->GetElementById("sfx") != NULL && gGameData->IsSfxEnabled())
				pDoc->GetElementById("sfx")->SetAttribute("checked", "");

			if (pDoc->GetElementById("bgm") != NULL && gGameData->IsBgmEnabled())
				pDoc->GetElementById("bgm")->SetAttribute("checked", "");
		}

		sDocument = doc;
	}

	return true;
}

bool GameFlow::InitializeGUI()
{
	pRocket = New(RocketInterface());
	pRocket->sName = "GUI";
	Rocket::Core::SetRenderInterface(pRocket);
	Rocket::Core::SetFileInterface(pRocket);
	Rocket::Core::SetSystemInterface(pRocket);
	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	pContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(pScreen->GetWidth(), pScreen->GetHeight()));
	if (pContext == NULL)
	{
		Rocket::Core::Shutdown();
		return false;
	}

	Rocket::Core::String fonts[4];
	fonts[0] = "fonts/Delicious-Roman.otf";
	fonts[1] = "fonts/Delicious-Italic.otf";
	fonts[2] = "fonts/Delicious-Bold.otf";
	fonts[3] = "fonts/Delicious-BoldItalic.otf";

	for (unsigned int i = 0; i < sizeof(fonts) / sizeof(Rocket::Core::String); i++)
		Rocket::Core::FontDatabase::LoadFontFace(fonts[i]);

	Rocket::Debugger::Initialise(pContext);
	pRocket->SetCurrentContext(pContext);
	pInput->AddKeyboardListener(pRocket);
	pInput->AddPointerListener(pRocket);
	pRocket->SetZ(-1000.0f);
	pScene->Add(pRocket);

	return true;
}

void GameFlow::ReleaseGUI()
{
	pInput->RemovePointerListener(pRocket);
	pInput->RemoveKeyboardListener(pRocket);

	pContext->UnloadAllDocuments();

	if (pDoc)
	{
		pDoc->RemoveReference();
		pDoc = NULL;
	}

	pContext->RemoveReference();

	Rocket::Core::Shutdown();

	pScene->Remove(pRocket);
	Delete(pRocket);
}

void GameFlow::OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script)
{
	UNUSED(ev)
	Rocket::Core::StringList commands;
	Rocket::Core::StringUtilities::ExpandString(commands, script, ';');
	for (size_t i = 0; i < commands.size(); ++i)
	{
		Rocket::Core::StringList values;
		Rocket::Core::StringUtilities::ExpandString(values, commands[i], ' ');
		if (values.empty())
			return;

		if (values[0] == "goto" && values.size() > 1)
		{
			if (values[1] == "credits")
				cFlow.OnEvent(&cOnCredits);
			else if (values[1] == "menu")
				cFlow.OnEvent(&cOnMenu);
			else if (values[1] == "options")
				cFlow.OnEvent(&cOnOptions);
			else if (values[1] == "game")
			{
				sSceneFile = "game.scene";
				this->DoLoad();
			}
		}
		else if (values[0] == "exit")
		{
			pSystem->Shutdown();
		}
		else if (values[0] == "toggle" && values.size() > 1)
		{
			if (values[1] == "sfx")
			{
				if (gGameData->IsSfxEnabled())
				{
					gGameData->SetSfxVolume(pSoundSystem->GetSfxVolume());
					pSoundSystem->SetSfxVolume(0.0f);
					gGameData->SetSfxEnabled(false);
				}
				else
				{
					pSoundSystem->SetSfxVolume(gGameData->GetSfxVolume());
					gGameData->SetSfxEnabled(true);
				}
			}
			else if (values[1] == "bgm")
			{
				if (gGameData->IsBgmEnabled())
				{
					gGameData->SetBgmVolume(pSoundSystem->GetMusicVolume());
					pSoundSystem->SetMusicVolume(0.0f);
					gGameData->SetBgmEnabled(false);
				}
				else
				{
					pSoundSystem->SetMusicVolume(gGameData->GetBgmVolume());
					gGameData->SetBgmEnabled(true);
				}
			}
			else if (values[1] == "fullscreen")
			{
				//this->ReleaseGUI();

				if (gGameData->IsFullScreenEnabled())
				{
					gGameData->SetFullScreenEnabled(false);
					//pScreen->ToggleFullscreen();
					pSystem->Shutdown();
				}
				else
				{
					gGameData->SetFullScreenEnabled(true);
					//pScreen->ToggleFullscreen();
					pSystem->Shutdown();
				}

				//this->InitializeGUI();
				//this->ReloadGUI();
			}
		}
	}
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

void GameFlow::DoLoad()
{
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

// GUI Elements
void GameFlow::SetLife(u32 life)
{
	char x[100];
	snprintf(x, 100, "%d", life);
	pElementLife->SetInnerRML(Rocket::Core::String(x));
	gGameData->SetLife(life);
}

void GameFlow::SetTime(u32 time)
{
	char x[100];
	snprintf(x, 100, "%d", time);
	pElementTime->SetInnerRML(Rocket::Core::String(x));
	gGameData->SetTime(time);
}

void GameFlow::SetHostage(u32 hostage)
{
	char x[100];
	snprintf(x, 100, "%d", hostage);
	pElementHostage->SetInnerRML(Rocket::Core::String(x));
	gGameData->SetHostage(hostage);
}

