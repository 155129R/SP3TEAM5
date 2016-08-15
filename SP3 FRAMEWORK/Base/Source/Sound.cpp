#include "Sound.h"

Sound::Sound() : BGM(NULL), Shoot(NULL), Reload(NULL)
{
	SoundEngine = createIrrKlangDevice();
}

Sound::~Sound()
{
	SoundEngine = NULL;
	BGM = NULL;
	Shoot = NULL;
	Reload = NULL;
}