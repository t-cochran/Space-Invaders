#ifndef PROJECTILES_H
#define PROJECTILES_H

#include <SFML/Graphics.hpp>

/* 
 *  A class for projectile objects fired by the aliens and the player
 */
class Projectile
{
    private:
        bool busy;
        sf::Texture* texture;
        sf::Sprite sprite;
        sf::FloatRect hitbox;
        sf::RectangleShape hitboxShape;

    public:
        Projectile();
        sf::Vector2f getPosition();
        sf::RectangleShape getHitboxShape();
        sf::Sprite* getSprite() {return &sprite;}
        sf::FloatRect* getHitbox() {return &hitbox;}
        bool getStatus();
        void setStatus(bool val);
        void setSpriteSize(sf::Vector2f objSize);
        void setSpritePosition(sf::Vector2f coord);
        void setTexture(const std::string path);
        void setHitboxPosition(sf::Vector2f coord);
        void setHitboxSize(sf::Vector2f hitboxSize);
        void drawProjectile(sf::RenderWindow &window);
        ~Projectile();
};
#endif