#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>

/* 
 *  Entity Class:
 *     The entity class is used to represent objects in the game world.
 *     
 *     The main components are:
 *        (1) Sprite: The visual representation of the entity.
 *        (2) Hitbox: A box used to detect collisions with other entities.
 */
class Entity
{
    private:
        bool status;
        sf::Texture* texture1;
        sf::Texture* texture2;
        sf::Sprite sprite;
        sf::FloatRect hitbox;
        sf::RectangleShape hitboxShape;

    public:
        Entity();
        sf::Vector2f getPosition();
        sf::Sprite* getSprite() { return &sprite; }
        sf::FloatRect* getHitbox() { return &hitbox; }
        bool getStatus();
        void setStatus(bool stat);
        void setSpriteSize(sf::Vector2f objSize);
        void setSpritePosition(sf::Vector2f coord);
        void setHitboxSize(sf::Vector2f hitboxSize);
        void setHitboxPosition(sf::Vector2f coord);
        void setTexture(const std::string path);
        void loadTexture(const std::string path);
        void updateTexture(int val);
        void drawEntity(sf::RenderWindow &window);
        ~Entity();
};

#endif