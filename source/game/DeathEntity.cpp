#include "DeathEntity.h"

DeathEntity::DeathEntity():
	SpriteEntity("Death")
{
	//empty
}

DeathEntity::~DeathEntity()
{
	pInput->RemoveKeyboardListener(this);
}

void DeathEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
}

Vector3f DeathEntity::GetPosition()
{
	return pSprite->GetPosition();
}

Sprite *DeathEntity::GetSprite() const
{
	return pSprite;
}

void DeathEntity::Update(f32 dt)
{

}
