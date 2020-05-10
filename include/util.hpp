#ifndef HELPERS_H
#define HELPERS_H

#include <SFML/Audio.hpp>
#include <unistd.h>
#include <deque>
#include "entities.hpp"
#include "sounds.hpp"

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
sf::Text* loadFont(const std::string path, sf::Font* font, sf::Vector2f scale, unsigned int size);
void initAmmo(int numBullets, Entity* gameEntity, const std::string texturePath, 
              sf::Vector2f position, sf::Vector2f spriteSize, sf::Vector2f hitboxSize);
void checkHit(std::vector<Entity>* alienVector, std::deque<Entity>::iterator bulletObj, 
              Sounds* hitSound);
void initAliens(int numAliens, std::vector<Entity>* alienVector, const std::string alienTexture, 
                const std::string bulletTexture, int x, int y, float width, float length);
void spawnAliens(std::vector<Entity>* alienVector, sf::RenderWindow* scrn, bool* leftDir, 
                 sf::Clock* clock);

#endif