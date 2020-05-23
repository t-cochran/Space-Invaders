#include "../include/sounds.hpp"

/* 
 *  Sound class member functions
 */
Sounds::Sounds(const std::string path) 
{
    sf::Sound* newSound = new sf::Sound;
    sf::SoundBuffer* buf = new sf::SoundBuffer;
    buf -> loadFromFile(path);
    newSound -> setBuffer(*buf);
    newSound -> setVolume(5);
    sound = newSound;
    buffer = buf;
}

void Sounds::play()
{
    sound -> play();
}

void Sounds::setVolume(float volume)
{
    sound -> setVolume(volume);
}

void Sounds::stop()
{
    sound -> stop();
}

Sounds::~Sounds() 
{ 
    free(Sounds::sound);
    free(Sounds::buffer);
}
