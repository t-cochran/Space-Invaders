#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include "entities.hpp"
#include "sounds.hpp"
#include "projectiles.hpp"

/* Game window Settings */
#define X_MIN                0.0f
#define X_MAX                920.0f
#define Y_MIN                -1000.0f
#define Y_MAX                1000.0f

/* Projectile Settings */
#define SHIP_BULLET_SPEED    15.0f
#define ALIEN_BULLET_SPEED   4.0f
#define FIRED                1
#define NOT_FIRED            0
#define SHIP_AMMO            200

/* Entity Settings */
#define DEAD                 0
#define ALIVE                1
#define ALIEN_ROWS           4
#define ALIEN_COLUMNS        8
#define ALIEN_SPEED          0.5f

/* View hitboxes on/off */
#define SEE_HITBOX           0

/* Game Volume (0 to 100) */
#define VOLUME               20

/* 
 *  Function prototypes
 */
void 
updateAmmoText(int ammoRemaining, sf::Text* displayedText, 
               sf::Clock clock, sf::Color color);

Entity* 
initShip(sf::Vector2f spritePos,  sf::Vector2f hitboxPos, 
         sf::Vector2f spriteSize, sf::Vector2f hitboxSize, 
         const std::string shipTexture);

sf::Text* 
loadFont(const std::string path, sf::Font* font, 
         sf::Vector2f scale, unsigned int size);

void 
initAmmo(int numBullets, std::vector<Projectile*>* ammoVector, 
         const std::string texturePath, sf::Vector2f position, 
         sf::Vector2f spriteSize, sf::Vector2f hitboxSize);

void 
initAliens(int rows, int columns, std::vector<std::vector<Entity>*>* RefBlock, 
           std::vector<Projectile*>* alienAmmo, const std::string alienTexture, 
           const std::string bulletTexture, const std::string alienTexture2);

void 
updateAlienPosition(std::vector<std::vector<Entity>*>* RefBlock, 
                    sf::RenderWindow* scrn, bool* leftDir, 
                    sf::Clock* clock, Sounds* move1, Sounds* move2);

void 
updateAlienBullets(Projectile* bullet, std::vector<Projectile*> alienAmmo, 
                   std::vector<std::vector<Entity>*> refBlock, Entity* ship,
                   Sounds* explode, sf::RenderWindow &window);

void 
updatePlayerBullets(Projectile* bullet, std::vector<Projectile*> shipAmmo, 
                    std::vector<std::vector<Entity>*> refBlock, Entity* ship,
                    Sounds* hit, sf::RenderWindow &window);

void
moveShip(Entity* ship);

#endif