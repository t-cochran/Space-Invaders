#include "entities.hpp"
#include "util.hpp"

/* 
 *  Entity class member functions
 */
Entity::Entity() {}

void Entity::setSpriteSize(sf::Vector2f objSize) 
{
    sprite.scale(objSize);
}

void Entity::setSpritePosition(sf::Vector2f coord)
{
    sprite.setPosition(coord);
}

void Entity::setHitboxPosition(sf::Vector2f coord)
{
    hitbox.left = coord.x;
    hitbox.top = coord.y;
    hitboxShape.setPosition(coord);
    hitboxShape.setFillColor(sf::Color::Green);
}

void Entity::setHitboxSize(sf::Vector2f hitboxSize)
{
    hitbox.width = hitboxSize.x;
    hitbox.height = hitboxSize.y;
    hitboxShape.setSize(hitboxSize);
}

void Entity::setTexture(const std::string path)
{
    sf::Texture* txtr = new sf::Texture;
    txtr -> loadFromFile(path);
    sprite.setTexture(*txtr);
    texture1 = txtr;
}

void Entity::loadTexture(const std::string path)
{
    sf::Texture* txtr = new sf::Texture;
    txtr -> loadFromFile(path);
    texture2 = txtr;
}

void Entity::updateTexture(int val)
{
    if (val == 0) { sprite.setTexture(*texture1); }
    if (val == 1) { sprite.setTexture(*texture2); }
    if (val == 2) { sprite.setTexture(*texture3); }
}

sf::Vector2f Entity::getPosition() 
{
    return sprite.getPosition();
}

// void Entity::setFiredWeapon(bool val)
// {
//     firedWeapon = val;
// }

// bool Entity::getFiredStatus()
// {
//     return firedWeapon;
// }

void Entity::setStatus(bool status)
{
    alive = status;
}

bool Entity::getStatus()
{
    return alive;
}

void Entity::drawEntity(sf::RenderWindow &window)
{
    window.draw(sprite);
    #if SEE_HITBOX
        window.draw(hitboxShape);
    #endif  
}

Entity::~Entity() { }