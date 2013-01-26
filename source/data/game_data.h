#ifndef _GAME_DATA_H
#define _GAME_DATA_H

#include "../defines.h"

class GameData : public IObject
{
	public:
		GameData();
		virtual ~GameData();

		u32 GetScore() const;
		void SetScore(u32);

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

		// IObject
		virtual const String GetClassName() const override;
		virtual int GetObjectType() const override;

	private:
		u32 iScore;
		bool bGameOver;
		bool bSfxEnabled;
		bool bBgmEnabled;
		bool bFullScreenEnabled;
		f32 fSfxVol;
		f32 fBgmVol;
};

#endif // _GAME_DATA_H
