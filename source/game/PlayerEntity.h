#ifndef _PLAYER_ENTITY_H
#define _PLAYER_ENTITY_H

#include "Entity.h"

#include "../defines.h"

class PlayerEntity: public Entity,
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

	private:
		Seed::Sprite *pSprite;
		Vector3f	vPlayerVectorDirection;
		float		fVelocity;
};

#endif // _PLAYER_ENTITY_H
