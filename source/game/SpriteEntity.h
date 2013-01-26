#ifndef _SPRITE_ENTITY_H
#define _SPRITE_ENTITY_H

#include "Entity.h"

class SpriteEntity: public Entity
		    , public IEventInputKeyboardListener
{
	public:
		SpriteEntity(const char *spriteName);
        virtual ~SpriteEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

    protected:
		Seed::Sprite *pSprite;
		const char *pszSpriteName;
		float		fVelocity;
		Vector3f	vPlayerVectorDirection;
};

#endif
