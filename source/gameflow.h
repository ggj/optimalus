#ifndef _GAMEFLOW_H_
#define _GAMEFLOW_H_

#include <api/rocket/RocketInterface.h>
#include <Rocket/Core.h>
#include <Seed.h>

#include "states/main_menu_state.h"
#include "states/options_state.h"
#include "states/credits_state.h"
#include "states/game_state.h"
#include "states/game_pause_state.h"
#include "data/game_data.h"

using namespace Seed;
using namespace Seed::RocketGui;

class GameFlow;
extern GameFlow *gFlow;
extern GameData *gGameData;

class GameFlow : public IGameApp,
			public IEventSystemListener,
			public IEventInputKeyboardListener,
			public IEventPresentationListener,
			public IRocketEventListener
{
	public:
		GameFlow();
		virtual ~GameFlow();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		void AddScene(ISceneObject *node);
		void RemoveScene(ISceneObject *node);

		// GUI
		bool LoadGUI(const String &doc);
		bool ReloadGUI();
		bool UnloadGUI();
		bool InitializeGUI();
		void ReleaseGUI();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

		// IRocketEventListener
		virtual void OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script);

		inline Camera *GetCamera() const;

	private:
		SEED_DISABLE_COPY(GameFlow);

		SceneNode			*pScene;
		Camera				*pCamera;
		Presentation		cPres;
		String				sDocument;

		// State Machine
		StateMachine		cFlow;

		// State Machine states
		MainMenuState			cMenu;
		OptionsState			cOptions;
		CreditsState				cCredits;
		GameState			cGame;
		GamePauseState			cGamePause;

		// State Machine Events
		StateMachineEvent	cOnMenu;
		StateMachineEvent	cOnOptions;
		StateMachineEvent	cOnCredits;
		StateMachineEvent	cOnGame;
		StateMachineEvent	cOnGamePause;

		// State Machine transitions
		StateMachineTransition cMenuToGame;
		StateMachineTransition cMenuToOptions;
		StateMachineTransition cMenuToCredits;
		StateMachineTransition cOptionsToMenu;
		StateMachineTransition cCreditsToMenu;
		StateMachineTransition cGameToGamePause;
		StateMachineTransition cGamePauseToGame;
		StateMachineTransition cGamePauseToMenu;

		// GUI
		RocketInterface			*pRocket;
		Rocket::Core::Context	*pContext;
		Rocket::Core::ElementDocument *pDoc;
};

inline Camera *GameFlow::GetCamera() const
{
	return pCamera;
}

#endif // _GAMEFLOW_H_
