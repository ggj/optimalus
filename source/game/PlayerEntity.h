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

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);
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
		b2Body		*pBody;
		float		fVelocity;
		Vector3f	vPlayerVectorDirection;
		bool		bIsRunning;

		ItemTypes::Enum eItem;

		enum		eAnimationStates {Idle = 0, Run = 1, Jump = 2, Land = 3};
		int			iPreviousState;
		int			iCurrentState;

		f32			fpMove;
		f32			fpLandTime;
		f32			fpInvicibleTime;

		Sprite		*pIcon;
};

#endif
