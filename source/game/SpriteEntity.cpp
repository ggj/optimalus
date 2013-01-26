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
	strName = metadata.GetProperty("Class");

	pSprite = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName(pszSpriteName))));
	pSprite->SetPosition(metadata.GetPosition());

	gScene->Add(pSprite);
}

