#ifndef _GAME_PAUSE_STATE_H
#define _GAME_PAUSE_STATE_H

#include "../defines.h"

class GamePauseState : public StateMachineState
{
	public:
		GamePauseState();
		virtual ~GamePauseState();
		virtual void OnStart(void *);
		virtual void OnUpdate(f32);
		virtual void OnStop(void *);
};

#endif // _GAME_PAUSE_STATE_H
