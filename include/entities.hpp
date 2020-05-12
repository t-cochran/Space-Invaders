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
        sf::Texture* texture;
        sf::Sprite sprite;
        sf::FloatRect hitbox;
        sf::RectangleShape hitboxShape;
        std::deque<Entity> ammo;
        std::deque<Entity> ammoReload;

    public:
        Entity();
        sf::Vector2f getPosition();
        sf::RectangleShape getHitboxShape();
        sf::Sprite* getSprite() {return &sprite;}
        sf::FloatRect* getHitbox() {return &hitbox;}
        std::deque<Entity>* getAmmoQueue() {return &ammo;}
        std::deque<Entity>* getReloadQueue() {return &ammoReload;}
        bool getStatus();
        void setStatus(bool status);
        bool getFiredStatus();
        void setFiredWeapon(bool val);
        void setBulletAmmo(Entity bullet);
        void setBulletReload(Entity bullet);
        void setSpriteSize(sf::Vector2f objSize);
        void setSpritePosition(sf::Vector2f coord);
        void setTexture(const std::string path);
        void setHitboxPosition(sf::Vector2f coord);
        void setHitboxSize(sf::Vector2f hitboxSize);
        ~Entity();
};

#endif