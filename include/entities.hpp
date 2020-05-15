#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <deque>
#include <SFML/Graphics.hpp>

/* 
 *  Entity Class:
 *     Each entity consists of a sprite and a hitbox for detecing collision.
 *     The hitbox itself is not visible, however the member 'hitboxShape' can 
 *     be drawn to view the hitbox of an entity for debugging purposes.
 */
class Entity
{
    private:
        bool alive;
        bool firedWeapon;
        sf::Texture* texture1;
        sf::Texture* texture2;
        sf::Texture* texture3;
        sf::Sprite sprite;
        sf::FloatRect hitbox;
        sf::RectangleShape hitboxShape;

    public:
        Entity();
        sf::Vector2f getPosition();
        sf::Sprite* getSprite() { return &sprite; }
        sf::FloatRect* getHitbox() { return &hitbox; }
        bool getStatus();
        void updateTexture(int val);
        void setStatus(bool status);
        // bool getFiredStatus();
        // void setFiredWeapon(bool val);
        void loadTexture(const std::string path);
        void setSpriteSize(sf::Vector2f objSize);
        void setSpritePosition(sf::Vector2f coord);
        void setTexture(const std::string path);
        void setHitboxPosition(sf::Vector2f coord);
        void setHitboxSize(sf::Vector2f hitboxSize);
        void drawEntity(sf::RenderWindow &window);
        ~Entity();
};

#endif