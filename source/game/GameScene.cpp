#include "GameScene.h"
#include "../gameflow.h"

#include <SceneNode.h>

SceneNode *gScene = NULL;
PhysicsManager *gPhysics = NULL;
SoundManager *gSoundManager =NULL;
WorldManager *gWorldManager = NULL;

enum
{
	kJobLoadScene
};

GameScene::GameScene(SceneNode *parent, Camera *mainCamera, const String &sceneFile)
	: pPlayer(NULL)
	, pCamera(mainCamera)
	, clCamera()
	, pParentScene(parent)
	, musTheme()
	, bPaused(false)
	, bInitialized(false)
	, sSceneFile(sceneFile)
{
	gScene = &cScene;
	gPhysics = &clPhysicsManager;
	gSoundManager = &clSoundManager;
	gWorldManager = &clWorldManager;
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

	// Create the State Machine.
	cFlow.RegisterTransition(&cRunToPause);
	cFlow.RegisterTransition(&cPauseToRun);
	cFlow.RegisterTransition(&cPauseToMenu);

	cFlow.Initialize(&cRun);

	RocketEventManager::AddListener(this);
	pInput->AddKeyboardListener(this);

	String f("scenes/");
	pJobManager->Add(New(FileLoader(f + sSceneFile, kJobLoadScene, this)));

	return true;
}

bool GameScene::Update(f32 dt)
{
/*
TEST: Bug de raster/texel.
*/
//	if (pPlayer)
//	{
//		//pPlayer->GetSprite()->AddY(0.5f);
//		pPlayer->GetSprite()->AddX(0.05f);
//		clCamera.LookAt(pPlayer->GetPosition());
//	}
//	return true;

	if (!bInitialized)
		return true;

	cFlow.Update(dt);
	if (!bPaused)
	{
		clPhysicsManager.Update(dt);
		clWorldManager.Update(dt);
		clCamera.LookAt(pPlayer->GetPosition());
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
	if (k == Seed::KeyEscape)
	{
		if (bPaused)
			cFlow.OnEvent(&cOnRun, this);
		else
			cFlow.OnEvent(&cOnPause, this);
	}
}

void GameScene::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
		case kJobLoadScene:
		{
			FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile);
			cScene.Load(r);
			Log("Scene Name: %s len %d", cScene.sName.c_str(), cScene.Size());
			Delete(job);

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

			MapLayerMetadata *game = pGameMap->GetLayerByName("Game")->AsMetadata();
			for (unsigned i = 0, len = game->Size(); i < len; ++i)
			{
				IMetadataObject *placeHolder = static_cast<IMetadataObject *>( game->GetChildAt(i));
				//const String &type = placeHolder->GetProperty("Type");
				//if (type == "Entity")
				{
					Entity* entity = clWorldManager.BuildEntity(*placeHolder, sprites);
					//Log("%s", entity->GetName().c_str());
					if (entity->GetClassName() == "Player")
					{
						pPlayer = static_cast<PlayerEntity*>(entity);
					}
				}
			}

			this->LoadMapColliders();

			clCamera.SetCamera(pCamera);
			clCamera.LookAt(pPlayer->GetSprite()->GetPosition());

			MapLayerTiled *bg = pGameMap->GetLayerByName("Background")->AsTiled();
			f32 hw = bg->GetWidth() * 0.5f;
			f32 hh = bg->GetHeight() * 0.5f;
			clCamera.SetArea(Rect4f(-hw, -hh, bg->GetWidth(), bg->GetHeight()));

			sprites->SetVisible(false);

			bInitialized = true;
		}
		break;
	}
}

void GameScene::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
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

void GameScene::LoadMapColliders()
{
	MapLayerMetadata *game = pGameMap->GetLayerByName("Colliders")->AsMetadata();
	for (unsigned i = 0, len = game->Size(); i < len; ++i)
	{
		IMetadataObject *placeHolder = static_cast<IMetadataObject *>( game->GetChildAt(i));

		clPhysicsManager.CreateStaticBody(placeHolder);
	}
}
