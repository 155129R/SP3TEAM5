#include "Sound.h"

Sound::Sound()
{
	musicEngine = irrklang::createIrrKlangDevice();
}
Sound::~Sound()
{
}
void Sound::playMusic(string Music)
{
	BGM = musicEngine->play2D(Music.c_str(), true, false, true);
	BGM->setVolume(0.5);
}
void Sound::stopMusic()
{
	BGM->stop();
}
void Sound::playSoundEffect2D(string Music)
{
	SoundEffect2D = musicEngine->play2D(Music.c_str(), false, false, false);
}
