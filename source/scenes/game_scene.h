#ifndef _GAME_H_
#define _GAME_H_

#include <api/rocket/RocketInterface.h>
#include <Seed.h>
using namespace Seed;
using namespace Seed::RocketGui;

#include "../game/WorldManager.h"
#include "../states/game_pause_state.h"
#include "../states/game_run_state.h"

extern SceneNode *gScene;

class GameScene : public IEventInputKeyboardListener,
				  public IEventJobListener,
				  public IRocketEventListener
{
	public:
		GameScene(SceneNode *parent, Camera *mainCamera);
		virtual ~GameScene();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		void Pause();
		void Resume();

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev);
		virtual void OnJobAborted(const EventJob *ev);

		// IRocketEventListener
		virtual void OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script);

	private:
		SEED_DISABLE_COPY(GameScene);

		Sprite			*pPlayer;
		Camera			*pCamera;
		SceneNode		*pScene;
		Music			musTheme;
		GameMap			*pGameMap;
		bool			bPaused;

		WorldManager	clWorldManager;

		// State Machine
		StateMachine		cFlow;
		GameRunState		cRun;
		GamePauseState		cPause;
		StateMachineState	cMenu;
		StateMachineEvent	cOnPause;
		StateMachineEvent	cOnRun;
		StateMachineEvent	cOnMenu;
		StateMachineTransition cRunToPause;
		StateMachineTransition cPauseToRun;
		StateMachineTransition cPauseToMenu;
};

#endif // _GAMEFLOW_H_
