#ifndef _ENEMY_ENTITY_H
#define _ENEMY_ENTITY_H

#include "spriteentity.h"
#include "../util/collisionsensor.h"

class EnemyEntity: public SpriteEntity
{
	public:
		EnemyEntity();
		virtual ~EnemyEntity();

		virtual void Load(MetadataObject &metadata, SceneNode *sprites);
		virtual void OnCollision(const CollisionEvent &event);

	private:
		b2Body *pBody;
		CollisionSensor clSensor;
};

#endif
