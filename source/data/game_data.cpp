#include "game_data.h"

GameData::GameData()
	: iScore(0)
	, bGameOver(false)
	, bSfxEnabled(true)
	, bBgmEnabled(true)
	, fSfxVol(1.0f)
	, fBgmVol(1.0f)
{
}

GameData::~GameData()
{
}

u32 GameData::GetScore() const
{
	return iScore;
}

void GameData::SetScore(const u32 score)
{
	iScore = score;
}

bool GameData::IsGameOver() const
{
	return bGameOver;
}

void GameData::SetGameOver(bool b)
{
	bGameOver = b;
}

bool GameData::IsSfxEnabled() const
{
	return bSfxEnabled;
}

void GameData::SetSfxEnabled(bool b)
{
	bSfxEnabled = b;
}

bool GameData::IsBgmEnabled() const
{
	return bBgmEnabled;
}

void GameData::SetBgmEnabled(bool b)
{
	bBgmEnabled = b;
}

f32 GameData::GetSfxVolume() const
{
	return fSfxVol;
}

void GameData::SetSfxVolume(f32 v)
{
	fSfxVol = v;
}

f32 GameData::GetBgmVolume() const
{
	return fBgmVol;
}

void GameData::SetBgmVolume(f32 v)
{
	fBgmVol = v;
}

const String GameData::GetClassName() const
{
	return "GameData";
}

int GameData::GetObjectType() const
{
	return nGameData;
}
