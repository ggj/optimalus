#include "game_scene.h"
#include "../gameflow.h"

#include <SceneNode.h>

SceneNode *gScene = NULL;

enum
{
	kJobLoadScene
};

GameScene::GameScene(SceneNode *parent, Camera *mainCamera)
	: pPlayer(NULL)
	, pCamera(mainCamera)
	, pScene(parent)
	, musTheme()
	, bPaused(false)
{
	gScene = this->pScene;
}

GameScene::~GameScene()
{
	gScene = NULL;
}

bool GameScene::Initialize()
{
	gFlow->AddScene(pScene);

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

	pJobManager->Add(New(FileLoader("scenes/game.scene", kJobLoadScene, this)));
	return true;
}

bool GameScene::Update(f32 dt)
{
	UNUSED(dt)
	cFlow.Update(dt);

	if (pPlayer)
	{
		pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	}

	return true;
}

bool GameScene::Shutdown()
{
	musTheme.Unload();

	gFlow->RemoveScene(pScene);
	pScene->Unload();
	pScene = NULL;

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
			pScene->Load(r);
			Log("Scene Name: %s len %d", pScene->sName.c_str(), pScene->Size());
			Delete(job);

			// Validate the music to play
			if (gGameData->IsBgmEnabled() == true)
			{
				musTheme.Load("sounds/theme.ogg");
				musTheme.SetVolume(1.0f);
				pSoundSystem->PlayMusic(&musTheme);
			}

			SceneNode *sprites = (SceneNode *)pScene->GetChildByName("Sprites");

			pPlayer = static_cast<Sprite *>(pScene->GetChildByName("Player"));
			pGameMap = (GameMap *)pScene->GetChildByName("Map");

			MapLayerMetadata *game = pGameMap->GetLayerByName("Game")->AsMetadata();
			for (unsigned i = 0, len = game->Size(); i < len; ++i)
			{
				IMetadataObject *placeHolder = static_cast<IMetadataObject *>( game->GetChildAt(i));
				const String &type = placeHolder->GetProperty("Type");

				if (type == "Entity")
				{
					clWorldManager.BuildEntity(*placeHolder, sprites);
				}
			}

			sprites->SetVisible(false);
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
