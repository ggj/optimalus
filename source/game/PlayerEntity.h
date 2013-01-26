#ifndef _PLAYER_ENTITY_H
#define _PLAYER_ENTITY_H

#include "SpriteEntity.h"

#include "../defines.h"

class PlayerEntity: public SpriteEntity
{
	public:
		PlayerEntity();
		virtual ~PlayerEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

        Vector3f GetPosition();
	private:
};

#endif
