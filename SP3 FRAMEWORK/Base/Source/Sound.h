#ifndef SOUND_H
#define SOUND_H

#include <irrKlang.h>
using namespace irrklang;

class Sound
{
public:
	ISoundEngine* SoundEngine;
	ISound* BGM;
	ISound* Shoot;
	ISound* Reload;

	Sound();
	~Sound();
};

#endif