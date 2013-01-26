#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>

#include "../scenes/game_scene.h"

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity()
	: pSprite(NULL)
{
    // empty
}

PlayerEntity::~PlayerEntity()
{
	pInput->RemoveKeyboardListener(this);

	Delete(pSprite);
}

void PlayerEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	//FIXME
	pSprite = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName("Player"))));
	gScene->Add(pSprite);

	fVelocity = 5.0f;
	vPlayerVectorDirection = VECTOR_ZERO;

	pInput->AddKeyboardListener(this);
}

void PlayerEntity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyUp)
	{
		// Sum the normalized vector up with the current vector
		vPlayerVectorDirection += VECTOR_UP;
	}

	if (k == Seed::KeyLeft)
	{
		// Sum the normalized vector left with the current vector
		vPlayerVectorDirection += VECTOR_LEFT;

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() < 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyRight)
	{
		// Sum the normalized vector right with the current vector
		vPlayerVectorDirection += VECTOR_RIGHT;

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() > 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyDown)
	{
		// Sum the normalized vector down with the current vector
		vPlayerVectorDirection += VECTOR_DOWN;
	}

	pSprite->AddPosition(vPlayerVectorDirection * fVelocity);
}

void PlayerEntity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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
