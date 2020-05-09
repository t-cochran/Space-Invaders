#ifndef HELPERS_H
#define HELPERS_H

#include <SFML/Audio.hpp>
#include <unistd.h>
#include <deque>
#include "entities.hpp"

/* Constants */
#define XMIN        0.0f
#define XMAX        920.0f
#define YMIN        -50.0f
#define YMAX        920.0f
#define BULLETSPEED 15.0f
#define MAXAMMO     100
#define VOLUME      20
#define ALIENSPEED  0.5f
#define SEE_HITBOX  0

/* 
 *  Function prototypes
 */
void initAmmo(int numBullets, std::deque<Entity>* bulletVector, sf::Texture* bulletTexture);
void initAliens(int numAliens, std::deque<Entity>* alienVector, sf::Texture* alienTexture, 
                int x, int y, float width, float length);
void updateAmmoText(int ammoRemaining, sf::Text* displayedText, sf::Clock clock);
void spawnAliens(std::deque<Entity>* alienVector, sf::RenderWindow* scrn, bool* leftDir, 
                 sf::Clock* clock);
void checkHit(std::deque<Entity>* alienVector, std::deque<Entity>::iterator bulletObj, sf::Sound* hitSound);

#endif