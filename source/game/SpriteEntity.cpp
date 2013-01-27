#include "SpriteEntity.h"

#include "GameScene.h"

SpriteEntity::SpriteEntity(const char *className, const char *spriteName)
	:	Entity(className)
	,	pSprite(NULL)
	,	pszSpriteName(spriteName)
{
	SEED_ASSERT(spriteName);
}

SpriteEntity::~SpriteEntity()
{
	gScene->Remove(pSprite);
	Delete(pSprite);
}

void SpriteEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	Entity::Load(metadata, sprites);

	pSprite = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName(pszSpriteName))));
	pSprite->SetPosition(metadata.GetPosition());

	gScene->Add(pSprite);
}


