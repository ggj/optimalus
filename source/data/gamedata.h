#ifndef _GAME_DATA_H
#define _GAME_DATA_H

#include "../defines.h"

class GameData
{
	public:
		GameData();
		~GameData();

		u32 GetLevel() const;
		void SetLevel(u32);

		u32 GetXP() const;
		void SetXP(u32);

		u32 GetAttackPower() const;
		void SetAttackPower(u32);

		u32 GetGold() const;
		void SetGold(u32);
		void RemoveGold();

		u32 GetLife() const;
		void SetLife(u32);
		void RemoveLife();

		u32 GetLifePotion() const;
		void SetLifePotion(u32);
		void RemoveLifePotion();

		u32 GetMana() const;
		void SetMana(u32);
		void RemoveMana();

		u32 GetManaPotion() const;
		void SetManaPotion(u32);
		void RemoveManaPotion();

		bool IsSfxEnabled() const;
		void SetSfxEnabled(bool);

		bool IsBgmEnabled() const;
		void SetBgmEnabled(bool);

		bool IsFullScreenEnabled() const;
		void SetFullScreenEnabled(bool);

		f32 GetBgmVolume() const;
		void SetBgmVolume(f32 v);

		f32 GetSfxVolume() const;
		void SetSfxVolume(f32 v);

		struct PlayerData {
			u32 iLevel;
			u32 iXP;
			u32 iAttackPower;
			u32 iGold;
			u32 iLife;
			u32 iLifePotion;
			u32 iMana;
			u32 iManaPotion;
			bool bGameOver;
		} sPlayer;

		/*struct PlayerData {
			u32 iLevel;
			u32 iXP;
			u32 iAttackPower;
			u32 iGold;
			u32 iLife;
			u32 iLifePotion;
			u32 iMana;
			u32 iManaPotion;
			bool bGameOver;
		} sRealistPlayer;

		struct PlayerData {
			u32 iLevel;
			u32 iXP;
			u32 iAttackPower;
			u32 iGold;
			u32 iLife;
			u32 iLifePotion;
			u32 iMana;
			u32 iManaPotion;
			bool bGameOver;
		} sPessimistPlayer;*/

		struct OptinsData {
			bool bSfxEnabled;
			bool bBgmEnabled;
			bool bFullScreenEnabled;
			f32 fSfxVol;
			f32 fBgmVol;
		} sOptions;
};

#endif // _GAME_DATA_H
