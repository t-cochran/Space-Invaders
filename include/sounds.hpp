#ifndef SOUNDS_H
#define SOUNDS_H

#include <SFML/Audio.hpp>

/* 
 *  Sounds Class
 *     A simple sound class for loading and playing in game sounds. 
 */
class Sounds
{
    private:
        sf::Sound* sound;
        sf::SoundBuffer* buffer;

    public:
        Sounds(const std::string path);
        void play();
        void stop();
        sf::Sound getSound() { return *sound; }
        void setVolume(float volume);
        ~Sounds();
};

#endif
