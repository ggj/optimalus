#ifndef _GAMEOVER_STATE_H
#define _GAMEOVER_STATE_H

#include <Seed.h>
using namespace Seed;

class GameOverState : public StateMachineState
{
	public:
		GameOverState();
		virtual ~GameOverState();
		virtual void OnStart(void *);
		virtual void OnUpdate(f32);
		virtual void OnStop(void *);

	private:
		u64 iBeginStateTime;
};

#endif // _GAMEOVER_STATE_H
