#include "helpers.hpp"

// Initialize bullets as collision objects and put them into a vector
void initAmmo(int numBullets, std::deque<cObject>* bulletVector, sf::Texture* bulletTexture) {

    /* Fill the bullet vector with bullets */
    for (int i=0; i < numBullets; i++) 
    {
        /* Create the bullet collision objects (cObject) */
        cObject bulletObj(sf::Vector2f(-500.0f, -920.0f),   // Starting position (x, y)
                          sf::Vector2f(25.0f, 25.0f),       // Object size (length, width)
                          sf::Vector2f(25.0f, 25.0f),       // Hitbox size (length, width)
                          sf::Color::Yellow);               // Object color
        
        /* Add the bullet objects to a vector */
        bulletVector -> push_back(bulletObj);
    }
}

// Initialize alien sprites and put them into a vector
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

// Utility function that will update the ammo remaining after each frame
void updateAmmoText(int ammoRemaining, sf::Text* displayedText, sf::Clock gameClock) 
{
        std::string textString;
        displayedText -> setFillColor(ammoRemaining > 0 ? sf::Color::Cyan : sf::Color::Red);
        if (ammoRemaining > 0)
        {   // Print current ammo left
            textString = "Ammo: " + std::to_string(ammoRemaining);
        }
        else
        {   // Print out of ammo
            if ((int)std::round(gameClock.getElapsedTime().asMilliseconds()) % 2 == 0) 
            {
                displayedText -> setFillColor(sf::Color::Transparent);
            }
            displayedText -> setScale(sf::Vector2f(1.5, 1.5));
            displayedText -> setPosition(225, 500);
            textString = "OUT OF AMMO!";
        }
        displayedText -> setString(textString);
}
