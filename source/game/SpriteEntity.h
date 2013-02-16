#ifndef _SPRITE_ENTITY_H
#define _SPRITE_ENTITY_H

#include "../defines.h"
#include "entity.h"

class SpriteEntity: public Entity
{
	public:
		SpriteEntity(const char *className, const char *spriteName);
		virtual ~SpriteEntity();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);

	protected:
		Sprite *pSprite;
		const char *pszSpriteName;
};

#endif
