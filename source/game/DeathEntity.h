#ifndef _DEATHENTITY_H
#define _DEATHENTITY_H

#include "SpriteEntity.h"
#include "../defines.h"

class PlayerEntity;

#include <Box2D/Box2D.h>

class DeathEntity: public SpriteEntity
{
	public:
		DeathEntity();
		virtual ~DeathEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		virtual void Update(f32 dt);

		Vector3f GetPosition();
		Sprite *GetSprite() const;

		virtual void OnCollision(const CollisionEvent &event);

		virtual void Activate();

	private:
		b2Body		*pBody;

		f32			fpSleepConfig;
		f32			fpSleep;	
		f32			fpSpeedFactor;
		PlayerEntity *pTarget;
};

#endif
