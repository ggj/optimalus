#ifndef _PLAYER_ENTITY_H
#define _PLAYER_ENTITY_H

#include <Box2D/Box2D.h>
#include "../defines.h"
#include "spriteentity.h"

namespace ItemTypes
{
	enum Enum
	{
		None,
		Text
	};
}

class PlayerEntity: public SpriteEntity,
					public IEventInputKeyboardListener
{
	public:
		PlayerEntity();
		PlayerEntity(const char *className, const char *spriteName, bool bIsActive);

		virtual ~PlayerEntity();

		void Load(MetadataObject &metadata, SceneNode *sprites);
		void Update(f32 dt);

		// IEventInputKeyboardListener
		bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

		// IEventInputKeyboardListener
		bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		Vector3f GetPosition();
		Sprite *GetSprite() const;
		b2Vec2 GetBodyPosition() const;

		void SetItem(ItemTypes::Enum item);
		ItemTypes::Enum GetItem() const;

		void Teleport(const b2Vec2 &position);
		bool OnDamage(const b2Vec2 vec2Push);

		void StopPlayerMovement();
		void ChangePlayer();

		void SetIsActive(bool isActive);
		bool GetIsActive();

		void SetIsInputEnabled(bool isKeyboardEnabled);
		bool GetIsInputEnabled() const;

	protected:
		b2Body *pBody;
		Sprite *pText;
		Vector3f vPlayerVectorDirection;

		ItemTypes::Enum eItem;

		enum eAnimationStates {Idle = 0, Run = 1, Jump = 2, Land = 3};
		s32 iPreviousState;
		s32 iCurrentState;

		f32 fVelocity;
		f32 fMove;
		f32 fUpDownMove;
		f32 fLandTime;
		f32 fInvicibleTime;

		bool bIsActive;
		bool bIsInputEnabled;
		void SetState(int newState);
};

#endif
