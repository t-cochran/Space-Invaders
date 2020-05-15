#include "util.hpp"

/* 
 *  Projectile class member functions
 */
Projectile::Projectile() {}

sf::Vector2f Projectile::getPosition() 
{
    return sprite.getPosition();
}

void Projectile::setSpritePosition(sf::Vector2f coord)
{
    sprite.setPosition(coord);
}

void Projectile::setSpriteSize(sf::Vector2f objSize) 
{
    sprite.scale(objSize);
}

void Projectile::setHitboxSize(sf::Vector2f hitboxSize)
{
    hitbox.width = hitboxSize.x;
    hitbox.height = hitboxSize.y;
    hitboxShape.setSize(hitboxSize);
}

void Projectile::setHitboxPosition(sf::Vector2f coord)
{
    hitbox.left = coord.x;
    hitbox.top = coord.y;
    hitboxShape.setPosition(coord);
    hitboxShape.setFillColor(sf::Color::Green);
}

bool Projectile::getStatus()
{
    return status;
}

void Projectile::setStatus(bool val)
{
    status = val;
}

void Projectile::setTexture(const std::string path)
{
    sf::Texture* txtr = new sf::Texture;
    txtr -> loadFromFile(path);
    sprite.setTexture(*txtr);
}

void Projectile::drawProjectile(sf::RenderWindow &window)
{
    window.draw(sprite);
    #if SEE_HITBOX
        window.draw(hitboxShape);
    #endif  
}

Projectile::~Projectile() {}