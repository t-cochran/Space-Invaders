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
#define YMAX        1000.0f
#define SHIPBULLETSPEED 15.0f
#define ALIENBULLETSPEED 5.0f
#define MAXAMMO     75
#define VOLUME      20
#define ALIENSPEED  0.5f
#define SEE_HITBOX  0

/* 
 *  Function prototypes
 */
void updateAmmoText(int ammoRemaining, sf::Text* displayedText, sf::Clock clock);
void initAmmo(int numBullets, Entity* gameEntity, sf::Texture* bulletTexture, 
              sf::Vector2f position, sf::Vector2f spriteSize, sf::Vector2f hitboxSize);
void checkHit(std::vector<Entity>* alienVector, std::deque<Entity>::iterator bulletObj, 
              sf::Sound* hitSound);
void initAliens(int numAliens, std::vector<Entity>* alienVector, sf::Texture* alienTexture, 
                sf::Texture* bulletTexture, int x, int y, float width, float length);
void spawnAliens(std::vector<Entity>* alienVector, sf::RenderWindow* scrn, bool* leftDir, 
                 sf::Clock* clock);

#endif