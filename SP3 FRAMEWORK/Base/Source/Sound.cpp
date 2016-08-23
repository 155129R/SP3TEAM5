#include "Sound.h"

Sound::Sound()
{
	musicEngine = irrklang::createIrrKlangDevice();
	SoundEffect3D = irrklang::createIrrKlangDevice();
}
Sound::~Sound()
{

}
void Sound::Init()
{
	SoundEffect3D->setDefault3DSoundMinDistance(10.0f);
	SoundEffect3D->setDefault3DSoundMaxDistance(5000.0f);
	//SoundEffect3D->setSoundVolume(10.0f);
}
void Sound::Update(irrklang::vec3df pos, irrklang::vec3df view)
{
	SoundEffect3D->setListenerPosition(pos, view);
}
void Sound::playMusic(string Music)
{
	BGM = musicEngine->play2D(Music.c_str(), true, false, true);
}
void Sound::stopMusic()
{
	BGM->stop();
}
void Sound::playSoundEffect2D(string Music)
{
	SoundEffect2D = musicEngine->play2D(Music.c_str(), false, false, false);
}
void Sound::playSoundEffect3D(std::string Music, irrklang::vec3df pos, bool repeat)
{
	SoundEffect3D->play3D(Music.c_str(), pos, repeat);
}
void Sound::stopSoundEffect3D()
{
	SoundEffect3D->stopAllSounds();
}