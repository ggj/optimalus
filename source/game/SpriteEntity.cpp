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
	pInput->RemoveKeyboardListener(this);
	Delete(pSprite);
}

void SpriteEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	strName = metadata.GetProperty("Class");

	pSprite = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName(pszSpriteName))));
	pSprite->SetPosition(metadata.GetPosition());

	gScene->Add(pSprite);

	fVelocity = 5.0f;
	vPlayerVectorDirection = VECTOR_ZERO;

	pInput->AddKeyboardListener(this);
}

void SpriteEntity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyUp || k == Seed::KeyW)
	{
		// Sum the normalized vector up with the current vector
		vPlayerVectorDirection += VECTOR_UP;
	}

	if (k == Seed::KeyLeft || k == Seed::KeyA)
	{
		// Sum the normalized vector left with the current vector
		vPlayerVectorDirection += VECTOR_LEFT;

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() > 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyRight || k == Seed::KeyD)
	{
		// Sum the normalized vector right with the current vector
		vPlayerVectorDirection += VECTOR_RIGHT;

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() < 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyDown || k == Seed::KeyS)
	{
		// Sum the normalized vector down with the current vector
		vPlayerVectorDirection += VECTOR_DOWN;
	}

	pSprite->AddPosition(vPlayerVectorDirection * fVelocity);
}

void SpriteEntity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	// Remove the directions
	if (k == Seed::KeyUp)
	{
		vPlayerVectorDirection -= VECTOR_UP;
	}

	if (k == Seed::KeyLeft)
	{
		vPlayerVectorDirection -= VECTOR_LEFT;
	}

	if (k == Seed::KeyRight)
	{
		vPlayerVectorDirection -= VECTOR_RIGHT;
	}

	if (k == Seed::KeyDown)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
}

