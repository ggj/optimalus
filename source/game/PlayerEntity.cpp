#include <Box2D/Common/b2Math.h>
#include "playerentity.h"
#include "entityfactory.h"
#include "gamescene.h"
#include "sounds.h"

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity()
	: SpriteEntity("Player", "Player")
	, eItem(ItemTypes::None)
	, fpMove(0)
	, fpLandTime(0)
	, fpInvicibleTime(0)
	, pIcon(NULL)
{
}

PlayerEntity::~PlayerEntity()
{
	gScene->Remove(pIcon);
	Delete(pIcon);

	pInput->RemoveKeyboardListener(this);
	gPhysics->DestroyBody(pBody);
}

void PlayerEntity::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	pSprite->SetZ(-10);

	pIcon = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName("Heart"))));
	pIcon->SetPosition(0, 0);
	pIcon->SetVisible(false);
	gScene->Add(pIcon);

	b2Vec2 customSize(40, 46);

	pBody = gPhysics->CreateBody(pSprite, &customSize);
	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);

	pInput->AddKeyboardListener(this);
	fVelocity = 5.0f;
	vPlayerVectorDirection = VECTOR_ZERO;
}

Vector3f PlayerEntity::GetPosition()
{
	return pSprite->GetPosition();
}

b2Vec2 PlayerEntity::GetBodyPosition() const
{
	return pBody->GetPosition();
}

Sprite *PlayerEntity::GetSprite() const
{
	return pSprite;
}

void PlayerEntity::Teleport(const b2Vec2 &position)
{
	pBody->SetTransform(position, pBody->GetAngle());

	fpMove= 0;
	this->SetState(Idle);

	gSoundManager->Play(SND_TELEPORT);
}

void PlayerEntity::Update(f32 dt)
{
	pIcon->SetPosition(pSprite->GetPosition() + Vector3f(0, -40, 0));

	b2Vec2 vel = pBody->GetLinearVelocity();

	bool ground = this->CheckGround();

	if (fpInvicibleTime > 0)
	{
		pSprite->SetVisible(!pSprite->IsVisible());

		fpInvicibleTime -= dt;
		if (fpInvicibleTime <= 0)
		{
			pSprite->SetVisible(true);
			fpInvicibleTime = 0;
		}
	}

	if (fpMove != 0)
	{
		vel.x = 5 * fpMove;
		pBody->SetLinearVelocity(vel);
	}

	if (iPreviousState ==Jump && ground)
	{
		SetState(Land);
		fpLandTime = 0.3f;
	}

	if (iCurrentState != Jump && !ground)
	{
		this->SetState(Jump);
	}

	if (fpLandTime > 0 && iCurrentState == Land)
	{
		fpLandTime -= dt;
		if (fpLandTime <= 0)
		{
			if (fpMove != 0)
				this->SetState(Run);
			else
				this->SetState(Idle);
		}
	}

	if (iCurrentState == iPreviousState)
		return;

	if (iCurrentState == Jump)
	{
		pSprite->SetAnimation("Jump");
	}
	else if (iCurrentState == Land)
	{
		pSprite->SetAnimation("Land");
	}
	else if (iCurrentState == Run)
	{
		pSprite->SetAnimation("Run");
	}
	else
	{
		pSprite->SetAnimation("Idle");
	}

	iPreviousState = iCurrentState;
}

void PlayerEntity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	b2Vec2 vel = pBody->GetLinearVelocity();

	if ((k == Seed::KeyUp || k == Seed::KeyW || k == Seed::KeySpace) && iCurrentState != Jump)
	{
		SetState(Jump);
		pBody->ApplyForce(b2Vec2(0,500), pBody->GetWorldCenter());
		gSoundManager->Play(SND_JUMP);
	}

	if (k == Seed::KeyLeft || k == Seed::KeyA)
	{
		SetState(Run);

		fpMove = -1;

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() > 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyRight || k == Seed::KeyD)
	{
		SetState(Run);

		fpMove = 1;

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() < 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyDown || k == Seed::KeyS)
	{
		// Sum the normalized vector down with the current vector
		// Maybe later, maybe if the player can duck
	}

	//pSprite->AddPosition(vPlayerVectorDirection * fVelocity);
}

void PlayerEntity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	b2Vec2 vel = pBody->GetLinearVelocity();
	vel.x = 0;

	// Remove the directions
	if (k == Seed::KeyUp|| k == Seed::KeyW)
	{

	}

	if (k == Seed::KeyLeft|| k == Seed::KeyA)
	{
		pBody->SetLinearVelocity(vel);
		fpMove = 0;

		if (CheckGround())
			SetState(Idle);
		else
			SetState(Jump);
	}

	if (k == Seed::KeyRight|| k == Seed::KeyD)
	{
		pBody->SetLinearVelocity(vel);
		fpMove = 0;

		if (CheckGround())
			SetState(Idle);
		else
			SetState(Jump);
	}

	if (k == Seed::KeyDown|| k == Seed::KeyS)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}
}

bool PlayerEntity::CheckGround()
{
	if (gPhysics->RayCast(pBody, b2Vec2(0, 0.32f)))
		return true;
	if (gPhysics->RayCast(pBody, b2Vec2(0.16f, 0.32f)))
		return true;
	if (gPhysics->RayCast(pBody, b2Vec2(-0.16f, 0.32f)))
		return true;

	return false;
}

void PlayerEntity::SetItem(ItemTypes::Enum item)
{
	eItem = item;
	pIcon->SetVisible(eItem == ItemTypes::Heart);
}

ItemTypes::Enum PlayerEntity::GetItem() const
{
	return eItem;
}

void PlayerEntity::SetState(int newState)
{
	iPreviousState = iCurrentState;
	iCurrentState = newState;
}

bool PlayerEntity::OnDamage()
{
	if (fpInvicibleTime > 0)
		return false;

	fpInvicibleTime = 3;
	return true;
}
