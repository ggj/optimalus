#ifndef _DEATHENTITY_H
#define _DEATHENTITY_H

#include "spriteentity.h"
#include "collisionsensor.h"

class PlayerEntity;

class DeathEntity: public SpriteEntity
{
	public:
		DeathEntity();
		virtual ~DeathEntity();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);
		virtual void Update(f32 dt);

		Vector3f GetPosition();
		Sprite *GetSprite() const;

		virtual void OnCollision(const CollisionEvent &event);
		virtual void Activate();

	private:
		PlayerEntity *pTarget;
		CollisionSensor clSensor;
		f32 fpSleepConfig;
		f32 fpSleep;
		f32 fpSpeedFactor;

};

#endif
