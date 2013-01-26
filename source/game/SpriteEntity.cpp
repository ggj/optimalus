#include "SpriteEntity.h"

#include "../scenes/game_scene.h"

SpriteEntity::SpriteEntity(const char *spriteName)
	:	pszSpriteName(spriteName)
	,	pSprite(NULL)

{
	SEED_ASSERT(spriteName);
}

SpriteEntity::~SpriteEntity()
{
	Delete(pSprite);
}

void SpriteEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	pSprite = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName(pszSpriteName))));
	gScene->Add(pSprite);
}

