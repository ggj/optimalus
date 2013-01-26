#include "game_data.h"

GameData::GameData()
	: sPlayer()
	, sOptions()
{
	sPlayer.iScore = 0;
	sPlayer.bGameOver = false;

	sOptions.bSfxEnabled = true;
	sOptions.bBgmEnabled = true;
	sOptions.fSfxVol = 1.0f;
	sOptions.fBgmVol = 1.0f;
}

GameData::~GameData()
{
}

u32 GameData::GetScore() const
{
	return sPlayer.iScore;
}

void GameData::SetScore(const u32 score)
{
	sPlayer.iScore = score;
}

bool GameData::IsGameOver() const
{
	return sPlayer.bGameOver;
}

void GameData::SetGameOver(bool b)
{
	sPlayer.bGameOver = b;
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
