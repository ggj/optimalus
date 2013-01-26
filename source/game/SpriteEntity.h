#ifndef _SPRITE_ENTITY_H
#define _SPRITE_ENTITY_H

#include "Entity.h"

class SpriteEntity: public Entity
{
	public:
		SpriteEntity(const char *spriteName);
		~SpriteEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

	private:
		Seed::Sprite *pSprite;

		const char *pszSpriteName;
};

#endif
