#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
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
        sf::Sprite sprite;
        sf::FloatRect hitbox;
        sf::RectangleShape hitboxShape;

    public:
        Entity();
        sf::Vector2f getPosition();
        sf::RectangleShape getHitboxShape();
        sf::Sprite* getSprite() { return &sprite; }
        sf::FloatRect* getHitbox() { return &hitbox; }
        void setSpriteSize(sf::Vector2f objSize);
        void setSpritePosition(sf::Vector2f coord);
        void setSpriteTexture(sf::Texture* texture);
        void setHitboxPosition(sf::Vector2f coord);
        void setHitboxSize(sf::Vector2f hitboxSize);
        void drawSprite(sf::RenderWindow* window);
        void drawHitbox(sf::RenderWindow* window);
        ~Entity();
};

#endif