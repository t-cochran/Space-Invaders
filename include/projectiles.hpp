#ifndef PROJECTILES_H
#define PROJECTILES_H

#include <SFML/Graphics.hpp>

/*
 *  Projectile Class
 *     A class for projectile objects that represent bullets fired by 
 *     the aliens and player space ship
 */
class Projectile
{
    private:
        bool status;
        sf::Sprite sprite;
        sf::FloatRect hitbox;
        sf::RectangleShape hitboxShape;

    public:
        Projectile();
        sf::Vector2f getPosition();
        sf::FloatRect* getHitbox() { return &hitbox; }
        void setSpritePosition(sf::Vector2f coord);
        void setSpriteSize(sf::Vector2f objSize);
        void setHitboxSize(sf::Vector2f hitboxSize);
        void setHitboxPosition(sf::Vector2f coord);
        bool getStatus();
        void setStatus(bool val);
        void setTexture(const std::string path);        
        void drawProjectile(sf::RenderWindow &window);
        ~Projectile();
};

#endif
