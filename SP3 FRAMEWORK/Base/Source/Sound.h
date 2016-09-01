#ifndef SOUND_H
#define SOUND_H

#include "irrKlang.h"
#include <string>
using std::string;

#pragma comment(lib, "irrKlang.lib")

class Sound
{
public:

	Sound();
	~Sound();
	void playMusic(string Music);
	void stopMusic();
	void playSoundEffect2D(string Music);

private:
	irrklang::ISoundEngine* musicEngine;
	irrklang::ISound* BGM;
	irrklang::ISound* SoundEffect2D;
};
#endif