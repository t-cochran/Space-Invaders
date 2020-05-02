#include "helpers.hpp"


// Given the number of bullets, initialize a vector of bullet structures
void initAmmo(int numBullets, std::deque<sf::Sprite>* bulletVector, sf::Texture* bulletTexture) {

    /* Fill the bullet vector with bullets */
    for (int i=0; i < numBullets; i++) 
    {
        /* Create the bullet sprite and set attributes */
        sf::Sprite bulletSprite(*bulletTexture);
        bulletSprite.scale(sf::Vector2f(0.1f, .1f));
        bulletSprite.setOrigin(sf::Vector2f(-4500.f, -9300.f));

        /* Add the bullet to the bullet vector */
        bulletVector -> push_back(bulletSprite);
    }
}

void initAliens(int numAliens, std::deque<sf::Sprite>* alienVector, sf::Texture* alienTexture) {

    for (int i=0; i < numAliens; i++) 
    {
        /* Create an alien sprite */
        sf::Sprite alien(*alienTexture);
        alien.scale(sf::Vector2f(0.1f, 0.1f));
        alien.setPosition(325 + (i*120), 100);

        /* Add the alien to the alien vector */
        alienVector -> push_back(alien);
    }
}
