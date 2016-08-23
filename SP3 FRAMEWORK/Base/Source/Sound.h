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
	void Init();
	void Update(irrklang::vec3df pos, irrklang::vec3df view);
	void playMusic(string Music);
	void stopMusic();
	void playSoundEffect2D(string Music);
	void playSoundEffect3D(string Music, irrklang::vec3df pos, bool repeat);
	void stopSoundEffect3D();

private:
	irrklang::ISoundEngine* musicEngine;
	irrklang::ISound* BGM;
	irrklang::ISound* SoundEffect2D;
	irrklang::ISoundEngine* SoundEffect3D;
};
#endif