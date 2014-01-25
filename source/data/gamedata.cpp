#include "gamedata.h"

GameData::GameData()
	: sPlayer()
	//, sRealistPlayer()
	//, sPessimistPlayer()
	, sOptions()
{
	sPlayer.iLevel = 10;
	sPlayer.iXP = 1;
	sPlayer.iAttackPower = 15;
	sPlayer.iGold = 20;
	sPlayer.iLife = 0;
	sPlayer.iLifePotion = 0;
	sPlayer.iMana = 10;
	sPlayer.iManaPotion = 0;

	/*sRealistPlayer.iLevel = 1;
	sRealistPlayer.iXP = 1;
	sRealistPlayer.iAttackPower = 15;
	sRealistPlayer.iGold = 0;
	sRealistPlayer.iLife = 22;
	sRealistPlayer.iLifePotion = 0;
	sRealistPlayer.iMana = 10;
	sRealistPlayer.iManaPotion = 0;

	sPessimistPlayer.iLevel = 1;
	sPessimistPlayer.iXP = 1;
	sPessimistPlayer.iAttackPower = 15;
	sPessimistPlayer.iGold = 0;
	sPessimistPlayer.iLife = 22;
	sPessimistPlayer.iLifePotion = 0;
	sPessimistPlayer.iMana = 10;
	sPessimistPlayer.iManaPotion = 0;*/

	sOptions.bSfxEnabled = true;
	sOptions.bBgmEnabled = true;
	sOptions.bFullScreenEnabled = false;
	sOptions.fSfxVol = 1.0f;
	sOptions.fBgmVol = 1.0f;
}

GameData::~GameData()
{
}

u32 GameData::GetLevel() const
{
	return sPlayer.iLevel;
}
void GameData::SetLevel(const u32 level)
{
	sPlayer.iLevel = level;
}

u32 GameData::GetXP() const
{
	return sPlayer.iXP;
}
void GameData::SetXP(const u32 xp)
{
	sPlayer.iXP = xp;
}

u32 GameData::GetAttackPower() const
{
	return sPlayer.iAttackPower;
}
void GameData::SetAttackPower(const u32 attackPower)
{
	sPlayer.iAttackPower = attackPower;
}

u32 GameData::GetGold() const
{
	return sPlayer.iGold;
}
void GameData::SetGold(const u32 gold)
{
	sPlayer.iGold = gold;
}
void GameData::RemoveGold()
{
	sPlayer.iGold--;
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

u32 GameData::GetLifePotion() const
{
	return sPlayer.iLifePotion;
}
void GameData::SetLifePotion(const u32 lifePotion)
{
	sPlayer.iLifePotion = lifePotion;
}
void GameData::RemoveLifePotion()
{
	sPlayer.iLifePotion--;
}

u32 GameData::GetMana() const
{
	return sPlayer.iMana;
}
void GameData::SetMana(const u32 mana)
{
	sPlayer.iMana = mana;
}
void GameData::RemoveMana()
{
	sPlayer.iMana--;
}

u32 GameData::GetManaPotion() const
{
	return sPlayer.iManaPotion;
}
void GameData::SetManaPotion(const u32 manaPotion)
{
	sPlayer.iManaPotion = manaPotion;
}
void GameData::RemoveManaPotion()
{
	sPlayer.iManaPotion--;
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
