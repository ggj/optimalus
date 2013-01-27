#include "SoundManager.h"

SoundManager::SoundManager():
	pSounds(NULL)
{
	//empty
}

void SoundManager::Init(SceneNode &sounds)
{
	pSounds = &sounds;	
}

void SoundManager::Play(const String &name)
{
	ISoundSource *source = static_cast<ISoundSource *>(pSounds->GetChildByName(name));

	if(source == NULL)
		return;

	source->Play();
}

void SoundManager::Update(f32 dt)
{

}
