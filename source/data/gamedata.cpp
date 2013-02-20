#include "gamedata.h"

GameData::GameData()
	: sPlayer()
	, sOptions()
{
	sPlayer.iLife = 3;
	sPlayer.iTime = 59;
	sPlayer.iHostage = 5;

	sOptions.bSfxEnabled = true;
	sOptions.bBgmEnabled = true;
	sOptions.fSfxVol = 1.0f;
	sOptions.fBgmVol = 1.0f;
}

GameData::~GameData()
{
}

u32 GameData::GetLife() const
{
	return sPlayer.iLife;
}

void GameData::SetLife(const u32 life)
{
	sPlayer.iLife = life;
}

void GameData::RemoveLife()
{
	sPlayer.iLife--;
}

u32 GameData::GetTime() const
{
	return sPlayer.iTime;
}

void GameData::SetTime(const u32 time)
{
	sPlayer.iTime = time;
}

u32 GameData::GetHostage() const
{
	return sPlayer.iHostage;
}

void GameData::SetHostage(const u32 hostage)
{
	sPlayer.iHostage = hostage;
}

void GameData::RemoveHostage()
{
	sPlayer.iHostage --;
}

bool GameData::IsSfxEnabled() const
{
	return sOptions.bSfxEnabled;
}

void GameData::SetSfxEnabled(bool b)
{
	sOptions.bSfxEnabled = b;
}

bool GameData::IsBgmEnabled() const
{
	return sOptions.bBgmEnabled;
}

void GameData::SetBgmEnabled(bool b)
{
	sOptions.bBgmEnabled = b;
}

bool GameData::IsFullScreenEnabled() const
{
	return sOptions.bFullScreenEnabled;
}

void GameData::SetFullScreenEnabled(bool b)
{
	sOptions.bFullScreenEnabled = b;
}

f32 GameData::GetSfxVolume() const
{
	return sOptions.fSfxVol;
}

void GameData::SetSfxVolume(f32 v)
{
	sOptions.fSfxVol = v;
}

f32 GameData::GetBgmVolume() const
{
	return sOptions.fBgmVol;
}

void GameData::SetBgmVolume(f32 v)
{
	sOptions.fBgmVol = v;
}
