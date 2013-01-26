#include "SpriteEntity.h"

#include "GameScene.h"

SpriteEntity::SpriteEntity(const char *spriteName)
	:	pSprite(NULL)
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
	strName = metadata.GetProperty("Class");

	pSprite = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName(pszSpriteName))));
	pSprite->SetPosition(metadata.GetPosition());

	gScene->Add(pSprite);
}


