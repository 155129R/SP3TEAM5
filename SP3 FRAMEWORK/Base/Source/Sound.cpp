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
}
void Sound::stopMusic(string Music)
{
	BGM->stop();
}
void Sound::playSoundEffect2D(string Music)
{
	SoundEffect2D = musicEngine->play2D(Music.c_str(), false, false, false);
}
void Sound::playSoundEffect3D(std::string Music, irrklang::vec3df pos, irrklang::vec3df view, irrklang::vec3df TargetPos)
{
	SoundEffect3D = musicEngine->play3D(Music.c_str(), TargetPos);
	musicEngine->setListenerPosition(pos, view);
}