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
		Heart
	};
}

class PlayerEntity: public SpriteEntity,
					public IEventInputKeyboardListener
{
	public:
		PlayerEntity();
		virtual ~PlayerEntity();

		virtual void Load(MetadataObject &metadata, SceneNode *sprites);
		virtual void Update(f32 dt);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		Vector3f GetPosition();
		Sprite *GetSprite() const;
		b2Vec2 GetBodyPosition() const;

		void SetItem(ItemTypes::Enum item);
		ItemTypes::Enum GetItem() const;

		void Teleport(const b2Vec2 &position);
		bool OnDamage();

	private:
		bool CheckGround();
		void SetState(int newState);

	protected:
		b2Body *pBody;
		Sprite *pIcon;
		Vector3f vPlayerVectorDirection;

		ItemTypes::Enum eItem;

		enum eAnimationStates {Idle = 0, Run = 1, Jump = 2, Land = 3};
		s32 iPreviousState;
		s32 iCurrentState;

		f32 fVelocity;
		f32 fMove;
		f32 fLandTime;
		f32 fInvicibleTime;

		bool bIsRunning;
};

#endif
