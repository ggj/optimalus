#ifndef _ENEMY_ENTITY_H
#define _ENEMY_ENTITY_H

#include "spriteentity.h"
#include "../util/collisionsensor.h"
#include "playerentity.h"

class EnemyEntity: public SpriteEntity
{
	public:
		EnemyEntity();
		virtual ~EnemyEntity();

		virtual void Load(MetadataObject &metadata, SceneNode *sprites);
		void Update(f32 dt);

		virtual void OnCollision(const CollisionEvent &event);

		bool OnDamage();

		struct EnemyData
		{
			String displayName;
			u32 iLevel;
			u32 iAttackPower;
			u32 iDefensePower;
		} sEnemy;

	private:
		PlayerEntity *pTarget;
		b2Body *pBody;
		CollisionSensor clSensor;

		f32 fInvicibleTime;
};

#endif
