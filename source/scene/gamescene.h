#ifndef _GAME_H_
#define _GAME_H_

#include <api/rocket/RocketInterface.h>

#include "../defines.h"
#include "../manager/physicsmanager.h"
#include "../manager/worldmanager.h"
#include "../states/game/pause_state.h"
#include "../states/gameover_state.h"
#include "../states/game/run_state.h"
#include "../entity/playerentity.h"
#include "../entity/optimistplayerentity.h"
#include "../entity/realistplayerentity.h"
#include "../entity/pessimistplayerentity.h"
#include "../controller/cameracontroller.h"
#include "../manager/soundmanager.h"

using namespace Seed::RocketGui;

class GameScene;

extern SceneNode *gScene;
extern PhysicsManager *gPhysics;
extern SoundManager *gSoundManager;
extern WorldManager *gWorldManager;
extern GameScene *gGameScene;

class GameScene : public IEventInputKeyboardListener,
				  public IRocketEventListener
{
	SEED_DISABLE_COPY(GameScene)

	public:
		GameScene(SceneNode *parent, Camera *mainCamera, const String &sceneFile);
		virtual ~GameScene();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		void Pause();
		void Resume();

		virtual void OnJobCompleted(FileLoader *job);
		virtual void OnJobAborted();

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IRocketEventListener
		virtual void OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script);

		// Load
		virtual void LoadMapColliders();

		void RemoveLife();

	private:
		PlayerEntity	 *pPlayer;
		PlayerEntity	 *pPlayerRealist;
		PlayerEntity	 *pPlayerPessimist;
		PlayerEntity	 *pPlayerOptimist;
		Camera			 *pCamera;
		CameraController clCamera;
		SceneNode		*pParentScene;
		SceneNode		cScene;
		Music			musTheme;
		GameMap			*pGameMap;
		bool			bPaused;
		bool			bInitialized;

		WorldManager	clWorldManager;
		PhysicsManager	clPhysicsManager;
		SoundManager	clSoundManager;

		// State Machine
		StateMachine		cFlow;

		// State Machine State
		RunState			cRun;
		PauseState			cPause;
		GameOverState		cGameOver;
		StateMachineState	cMenu;

		// State Machine Event
		StateMachineEvent	cOnPause;
		StateMachineEvent	cOnRun;
		StateMachineEvent	cOnMenu;
		StateMachineEvent	cOnGameOver;

		// State Machine Transition
		StateMachineTransition cRunToPause;
		StateMachineTransition cPauseToRun;
		StateMachineTransition cPauseToMenu;
		StateMachineTransition cRunToGameOver;
		StateMachineTransition cGameOverToMenu;

		String sSceneFile;
		String strNextLevel;

		f32 fTimeToNextLevel;
		bool bChangeLevel;

		Image *pGameOverImg;

};

#endif // _GAMEFLOW_H_
