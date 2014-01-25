#include "itementity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"

ItemEntity::ItemEntity(const char *className, const char *spriteName)
	: SpriteEntity(className, spriteName)
{
}

ItemEntity::~ItemEntity()
{
}

void ItemEntity::Load(MetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	pSprite->SetZ(-10);
	clSensor.Load(metadata, this);

	b2Vec2 customSize(32, 32);

	pBody = gPhysics->CreateStaticBody(pSprite, BodyType::Normal, false, &customSize);
	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);
}

void ItemEntity::Update(f32 dt)
{
	b2Vec2 vel = pBody->GetLinearVelocity();
	pBody->SetLinearVelocity(vel);
}

void ItemEntity::OnCollision(const CollisionEvent &event)
{
	UNUSED(event);
}

