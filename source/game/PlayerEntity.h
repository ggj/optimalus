#ifndef _PLAYER_ENTITY_H
#define _PLAYER_ENTITY_H

#include "SpriteEntity.h"
#include "../defines.h"
#include <Box2D/Box2D.h>

class PlayerEntity: public SpriteEntity,
					public IEventInputKeyboardListener
{
	public:
		PlayerEntity();
		virtual ~PlayerEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);


		// IEventInputKeyboardListener
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		Vector3f GetPosition();
		Sprite *GetSprite() const;

		bool CheckGround();

	private:		

	protected:
		b2Body		*pBody;
		float		fVelocity;
		Vector3f	vPlayerVectorDirection;
		bool		bIsRunning;
};

#endif
