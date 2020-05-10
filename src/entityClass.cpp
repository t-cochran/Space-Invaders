#include "entities.hpp"

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

void Entity::setSpriteTexture(sf::Texture* texture)
{
    sprite.setTexture(*texture);
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

sf::Vector2f Entity::getPosition() 
{
    return sprite.getPosition();
}

sf::RectangleShape Entity::getHitboxShape()
{
    return hitboxShape;
}

void Entity::setBullet(Entity bullet)
{
    ammo.push_back(bullet);
}

void Entity::setBulletReload(Entity bullet)
{
    ammoReload.push_back(bullet);
}

void Entity::setFiredWeapon(bool val)
{
    firedWeapon = val;
}

bool Entity::getFiredStatus()
{
    return firedWeapon;
}

Entity::~Entity() {}