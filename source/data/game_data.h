#ifndef _GAME_DATA_H
#define _GAME_DATA_H

#include "../defines.h"

class GameData
{
	public:
		GameData();
		~GameData();

		u32 GetLife() const;
		void SetLife(u32);
		void RemoveLife();

		u32 GetTime() const;
		void SetTime(u32);

		u32 GetHostage() const;
		void SetHostage(u32);
		void RemoveHostage();

		bool IsGameOver() const;
		void SetGameOver(bool);

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
			u32 iLife;
			u32 iTime;
			u32 iHostage;
			bool bGameOver;
		} sPlayer;

		struct OptinsData {
			bool bSfxEnabled;
			bool bBgmEnabled;
			bool bFullScreenEnabled;
			f32 fSfxVol;
			f32 fBgmVol;
		} sOptions;
};

#endif // _GAME_DATA_H
