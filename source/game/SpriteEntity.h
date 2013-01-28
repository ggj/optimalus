#ifndef _SPRITE_ENTITY_H
#define _SPRITE_ENTITY_H

#include "entity.h"

class SpriteEntity: public Entity
{
	public:
		SpriteEntity(const char *className, const char *spriteName);
		SpriteEntity();
		virtual ~SpriteEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

	protected:
		Seed::Sprite	*pSprite;
		const char		*pszSpriteName;
};

#endif
