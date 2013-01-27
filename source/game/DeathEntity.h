#ifndef _DEATHENTITY_H
#define _DEATHENTITY_H

#include "SpriteEntity.h"
#include "../defines.h"

class DeathEntity: public SpriteEntity
{
	public:
		DeathEntity();
		virtual ~DeathEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		virtual void Update(f32 dt);

		Vector3f GetPosition();
		Sprite *GetSprite() const;
};

#endif
