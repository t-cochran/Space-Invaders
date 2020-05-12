#ifndef HELPERS_H
#define HELPERS_H

#include <SFML/Audio.hpp>
#include <unistd.h>
#include <deque>
#include "entities.hpp"
#include "sounds.hpp"

/* Constants */
#define DEAD        0
#define ALIVE       1
#define XMIN        0.0f
#define XMAX        920.0f
#define YMIN        -50.0f
#define YMAX        1000.0f
#define SHIP_BULLET_SPEED 15.0f
#define ALIEN_BULLET_SPEED 5.0f
#define ALIEN_ROWS 5
#define ALIEN_COLUMNS 10
#define MAX_AMMO     75
#define VOLUME      20
#define ALIEN_SPEED  0.5f
#define SEE_HITBOX  0

/* 
 *  Function prototypes
 */
void updateAmmoText(int ammoRemaining, sf::Text* displayedText, sf::Clock clock);
sf::Text* loadFont(const std::string path, sf::Font* font, sf::Vector2f scale, unsigned int size);
void initAmmo(int numBullets, Entity* gameEntity, const std::string texturePath, 
              sf::Vector2f position, sf::Vector2f spriteSize, sf::Vector2f hitboxSize);
void checkHit(std::vector<std::vector<Entity>*>* RefBlock, std::deque<Entity>::iterator bulletObj, 
              Sounds* hitSound);
void initAliens(int rows, int columns, std::vector<std::vector<Entity>*>* RefBlock, 
                const std::string alienTexture, const std::string bulletTexture);
void updateAlienPosition(std::vector<std::vector<Entity>*>* RefBlock, sf::RenderWindow* scrn, 
                         bool* leftDir, sf::Clock* clock);

#endif