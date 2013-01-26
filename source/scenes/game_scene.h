#ifndef _GAME_H_
#define _GAME_H_

#include <Seed.h>
using namespace Seed;

#include "../game/WorldManager.h"

class GameScene : public IEventInputKeyboardListener,
                  public IEventJobListener
{
	public:
		GameScene(SceneNode *parent, Camera *mainCamera);
		virtual ~GameScene();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev);
		virtual void OnJobAborted(const EventJob *ev);

	private:
		SEED_DISABLE_COPY(GameScene);

		ISceneObject	*pPlayer;
		Music		musTheme;
		Camera			*pCamera;
		SceneNode		*pScene;
		GameMap			*pGameMap;

		WorldManager	clWorldManager;
};

#endif // _GAMEFLOW_H_
