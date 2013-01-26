#ifndef _PLAYER_ENTITY_H
#define _PLAYER_ENTITY_H

#include "Entity.h"

#include "../defines.h"

class PlayerEntity: public Entity
{
	public:
		PlayerEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

	private:
		Seed::Sprite *pSprite;
};

#endif

