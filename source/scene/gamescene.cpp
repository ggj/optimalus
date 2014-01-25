#include "gamescene.h"
#include "../gameflow.h"
#include "../manager/guimanager.h"

SceneNode *gScene = NULL;
PhysicsManager *gPhysics = NULL;
SoundManager *gSoundManager =NULL;
WorldManager *gWorldManager = NULL;
GameScene *gGameScene = NULL;

GameScene::GameScene(SceneNode *parent, Camera *mainCamera, const String &sceneFile)
	: pPlayer(NULL)
	, pCamera(mainCamera)
	, clCamera()
	, pParentScene(parent)
	, cScene()
	, musTheme()
	, pGameMap(NULL)
	, bPaused(false)
	, bInitialized(false)
	, sSceneFile(sceneFile)
	, fTimeToNextLevel(3.0f)
	, bChangeLevel(false)
	, pGameOverImg(NULL)
{
	gScene = &cScene;
	gPhysics = &clPhysicsManager;
	gSoundManager = &clSoundManager;
	gWorldManager = &clWorldManager;
	gGameScene = this;
}

GameScene::~GameScene()
{
	gScene = NULL;
}

bool GameScene::Initialize()
{
	pParentScene->Add(&cScene);

	// Create the transitions
	cRunToPause.Initialize(&cRun, &cOnPause, &cPause);
	cPauseToRun.Initialize(&cPause, &cOnRun, &cRun);
	cPauseToMenu.Initialize(&cPause, &cOnMenu, &cMenu);
	cRunToGameOver.Initialize(&cRun, &cOnGameOver, &cGameOver);
	cGameOverToMenu.Initialize(&cGameOver, &cOnMenu, &cMenu);

	// Create the State Machine.
	cFlow.RegisterTransition(&cRunToPause);
	cFlow.RegisterTransition(&cPauseToRun);
	cFlow.RegisterTransition(&cPauseToMenu);
	cFlow.RegisterTransition(&cRunToGameOver);
	cFlow.RegisterTransition(&cGameOverToMenu);

	cFlow.Initialize(&cRun);

	auto cb = [&](Job *self)
	{
		auto job = static_cast<FileLoader *>(self);

		if(job->GetState() == eJobState::Completed)
		{
			OnJobCompleted(job);
		}
		else if(job->GetState() == eJobState::Aborted)
		{
			OnJobAborted();
		}
		sdDelete(self);
	};

	String f{"scenes/"};
	pJobManager->Add(sdNew(FileLoader(f + sSceneFile, cb)));

	RocketEventManager::AddListener(this);
	pInput->AddKeyboardListener(this);

	// Get the initial value from game data
	gGui->SetLife(gGameData->GetLife());
	gGui->SetHostage(gGameData->GetHostage());

	return true;
}

bool GameScene::Update(f32 dt)
{
	if (!bInitialized)
		return true;

	cFlow.Update(dt);
	if (!bPaused)
	{
		clPhysicsManager.Update(dt);
		clWorldManager.Update(dt);
		clCamera.LookAt(pPlayer->GetPosition());
	}

	if (bChangeLevel)
	{
		fTimeToNextLevel -= dt;
		if (fTimeToNextLevel <= 0)
		{
			gFlow->LoadSceneFile(strNextLevel);
		}
	}
	if (gGameData->GetLife() == 0)
	{
		pGameOverImg->SetVisible(true);
		pGameOverImg->SetPosition(pCamera->GetPosition() - Vector3f(-400.0f, -300.0f, 0.0f));
		pPlayer->GetSprite()->SetVisible(false);
		cFlow.OnEvent(&cOnGameOver, this);

		gGameData->SetLife(3);
	}

	return true;
}

bool GameScene::Shutdown()
{
	musTheme.Unload();

	clWorldManager.Clear();

	pParentScene->Remove(&cScene);
	cScene.Unload();
	pParentScene = NULL;

	pInput->RemoveKeyboardListener(this);
	RocketEventManager::RemoveListener(this);

	return true;
}

void GameScene::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();
	if (k == eKey::Escape)
	{
		if (bPaused)
			cFlow.OnEvent(&cOnRun, this);
		else
			cFlow.OnEvent(&cOnPause, this);
	}
}

void GameScene::OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script)
{
	UNUSED(ev)
	if (script == "resume")
	{
		cFlow.OnEvent(&cOnRun, this);
	}
	if (script == "quit")
	{
		cFlow.OnEvent(&cOnMenu);
		gFlow->Menu();
	}
}

void GameScene::Pause()
{
	bPaused = true;
}

void GameScene::Resume()
{
	bPaused = false;
}

void GameScene::OnJobCompleted(FileLoader *job)
{
	Reader r(job->pFile);
	cScene.Load(r);
	Log("Scene Name: %s len %d", cScene.sName.c_str(), cScene.Size());

	// Validate the music to play
	if (gGameData->IsBgmEnabled() == true)
	{
		musTheme.Load("sounds/theme.ogg");
		musTheme.SetVolume(1.0f);
		pSoundSystem->PlayMusic(&musTheme);
	}

	SceneNode *sounds = (SceneNode *)cScene.GetChildByName("Sounds");
	clSoundManager.Init(*sounds);

	SceneNode *sprites = (SceneNode *)cScene.GetChildByName("Sprites");
	pGameMap = (GameMap *)cScene.GetChildByName("Map");

	int hostageNum = 0;

	strNextLevel = pGameMap->GetProperty("NextLevel");

	MapLayerMetadata *game = pGameMap->GetLayerByName("Game")->AsMetadata();
	game->SetVisible(false);
	for (unsigned i = 0, len = game->Size(); i < len; ++i)
	{
		MetadataObject *placeHolder = static_cast<MetadataObject *>( game->GetChildAt(i));
		//const String &type = placeHolder->GetProperty("Type");
		//if (type == "Entity")
		{
			Entity* entity = clWorldManager.BuildEntity(*placeHolder, sprites);
			//Log("%s", entity->GetName().c_str());
			if (entity->GetClassName() == "RealistPlayer")
			{
				pPlayerRealist = static_cast<RealistPlayerEntity*>(entity);
			}
			if (entity->GetClassName() == "PessimistPlayer")
			{
				pPlayerPessimist = static_cast<PessimistPlayerEntity*>(entity);
			}
			if (entity->GetClassName() == "OptimistPlayer")
			{
				pPlayerOptimist = static_cast<OptimistPlayerEntity*>(entity);
			}
			else if (entity->GetClassName() == "Hostage")
			{
				++hostageNum;
			}
		}
	}

	// If the player is not set, the player will be optimist
	if (pPlayer == NULL)
	{
		pPlayer = pPlayerOptimist;
	}

	gGui->SetHostage(hostageNum);

	this->LoadMapColliders();

	clCamera.SetCamera(pCamera);
	clCamera.LookAt(pPlayer->GetSprite()->GetPosition());

	MapLayerTiled *bg = pGameMap->GetLayerByName("Background")->AsTiled();

	f32 hw = bg->GetWidth() * 0.5f;
	f32 hh = bg->GetHeight() * 0.5f;
	clCamera.SetArea(Rect4f(-hw, -hh, bg->GetWidth(), bg->GetHeight()));

	sprites->SetVisible(false);

	pGameOverImg = (Image *)cScene.GetChildByName("GameOverImage");
	pGameOverImg->SetVisible(false);

	bInitialized = true;

/*
	auto tex = static_cast<Texture *>(pResourceManager->Get("textures/tileset2.png", ITexture::GetTypeId()));
	auto tiles = pGameMap->GetLayerByName("Background")->AsTiled();
	auto set = tiles->GetTileSet();
	set->SetTexture(tex);
*/
}

void GameScene::ChangePlayer(const String currentPlayer)
{
	OptimistPlayerEntity *optimistPlayer = static_cast<OptimistPlayerEntity *>(gWorldManager->FindEntityByClassName("OptimistPlayer"));
	RealistPlayerEntity *realistPlayer = static_cast<RealistPlayerEntity *>(gWorldManager->FindEntityByClassName("RealistPlayer"));
	PessimistPlayerEntity *pessimistPlayer = static_cast<PessimistPlayerEntity *>(gWorldManager->FindEntityByClassName("PessimistPlayer"));

	Log("Current player: %s", currentPlayer.c_str());

	if (currentPlayer == "OptimistPlayer")
	{
		optimistPlayer->SetIsActive(false);
		pessimistPlayer->SetIsActive(false);
		realistPlayer->SetIsActive(true);

		pPlayer = realistPlayer;
		clCamera.LookAt(pPlayer->GetSprite()->GetPosition());

		auto tex = static_cast<Texture *>(pResourceManager->Get("textures/realist_ground_tileset.png", ITexture::GetTypeId()));
		auto tiles = pGameMap->GetLayerByName("Background")->AsTiled();
		auto set = tiles->GetTileSet();
		set->SetTexture(tex);
	}
	else if (currentPlayer == "RealistPlayer")
	{
		realistPlayer->SetIsActive(false);
		optimistPlayer->SetIsActive(false);
		pessimistPlayer->SetIsActive(true);

		pPlayer = pessimistPlayer;
		clCamera.LookAt(pPlayer->GetSprite()->GetPosition());

		auto tex = static_cast<Texture *>(pResourceManager->Get("textures/pessimist_ground_tileset.png", ITexture::GetTypeId()));
		auto tiles = pGameMap->GetLayerByName("Background")->AsTiled();
		auto set = tiles->GetTileSet();
		set->SetTexture(tex);
	}
	else if (currentPlayer == "PessimistPlayer")
	{
		pessimistPlayer->SetIsActive(false);
		realistPlayer->SetIsActive(false);
		optimistPlayer->SetIsActive(true);

		pPlayer = optimistPlayer;
		clCamera.LookAt(pPlayer->GetSprite()->GetPosition());

		auto tex = static_cast<Texture *>(pResourceManager->Get("textures/optimist_ground_tileset.png", ITexture::GetTypeId()));
		auto tiles = pGameMap->GetLayerByName("Background")->AsTiled();
		auto set = tiles->GetTileSet();
		set->SetTexture(tex);
	}
}

void GameScene::OnJobAborted()
{
}

void GameScene::LoadMapColliders()
{
	MapLayerMetadata *game = pGameMap->GetLayerByName("Colliders")->AsMetadata();
	game->SetVisible(false);
	for (unsigned i = 0, len = game->Size(); i < len; ++i)
	{
		MetadataObject *placeHolder = static_cast<MetadataObject *>( game->GetChildAt(i));

		clPhysicsManager.CreateStaticBody(placeHolder);
	}
}

void GameScene::RemoveLife()
{
	gGui->RemoveLife();
}
