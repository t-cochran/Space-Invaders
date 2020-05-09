#include "helpers.hpp"
#include "entities.hpp"

// Initialize bullets as collision objects and put them into a vector
void initAmmo(int numBullets, std::deque<Entity>* bulletVector, sf::Texture* bulletTexture) {

    /* Fill the bullet vector with bullets */
    for (int i=0; i < numBullets; i++) 
    {
        Entity bulletObj;
        bulletObj.setSpritePosition(sf::Vector2f(-500.0f, -920.0f));
        bulletObj.setHitboxPosition(sf::Vector2f(-500.0f, -920.0f));
        bulletObj.setSpriteSize(sf::Vector2f(1.0f, 1.0f));
        bulletObj.setHitboxSize(sf::Vector2f(5.0f, 5.0f));
        bulletObj.setSpriteTexture(bulletTexture);
        bulletVector -> push_back(bulletObj);
    }
}

// Initialize alien sprites and put them into a vector
void initAliens(int numAliens, std::deque<Entity>* alienVector, sf::Texture* alienTexture, 
                int x, int y, float width, float length) {

    /* Fill the alien vector with aliens */
    for (int i=0; i < numAliens; i++) 
    {
        Entity alienObj;
        alienObj.setSpritePosition(sf::Vector2f(x + (i*80), y));
        alienObj.setHitboxPosition(sf::Vector2f(x + (i*80), y));
        alienObj.setSpriteSize(sf::Vector2f(width, length));
        alienObj.setHitboxSize(sf::Vector2f(50.0f, 50.0f));
        alienObj.setSpriteTexture(alienTexture);
        alienVector -> push_back(alienObj);
    }
}

// Utility function that will update the ammo remaining after each frame
void updateAmmoText(int ammoRemaining, sf::Text* displayedText, sf::Clock clock) 
{
        std::string textString;
        displayedText -> setFillColor(ammoRemaining > 0 ? sf::Color::Cyan : sf::Color::Red);
        if (ammoRemaining > 0)
        {   // Print current ammo left
            textString = "Ammo: " + std::to_string(ammoRemaining);
        }
        else
        {   // Print out of ammo
            if ((int)clock.getElapsedTime().asMilliseconds() % 2 == 0) 
            {
                displayedText -> setFillColor(sf::Color::Transparent);
            }
            displayedText -> setScale(sf::Vector2f(1.5, 1.5));
            displayedText -> setPosition(225, 500);
            textString = "OUT OF AMMO!";
        }
        displayedText -> setString(textString);
}

// Spawn a row of aliens
void spawnAliens(std::deque<Entity>* alienVector, sf::RenderWindow* scrn, bool* leftDir, 
                 sf::Clock* clock)
{
    for (std::deque<Entity>::iterator alienObj = alienVector -> begin(); alienObj != alienVector -> end(); alienObj++) 
    {
        /* Get current alien position */
        float x = alienObj -> getPosition().x;
        float y = alienObj -> getPosition().y;

        /* Move aliens left: (1) Not reached Xmin, (2) Pause every other second */
        if (alienVector -> front().getPosition().x >= XMIN && *leftDir) 
        {
            if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
            {
                alienObj -> setSpritePosition(sf::Vector2f(x - ALIENSPEED, y));
                alienObj -> setHitboxPosition(sf::Vector2f((x + 10) - ALIENSPEED, y + 10));   
            }            
        } else { *leftDir = false; }

        /* Move aliens left: (1) Not reached Xmax, (2) Pause every other second */
        if (alienVector -> back().getPosition().x <= XMAX && !(*leftDir)) 
        {
            if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
            {
                alienObj -> setSpritePosition(sf::Vector2f(x + ALIENSPEED, y));
                alienObj -> setHitboxPosition(sf::Vector2f((x + 10) + ALIENSPEED, y + 10));
            }
        } else { *leftDir = true; }

        /* Draw the aliens */
        scrn -> draw(*alienObj -> getSprite());
        #if SEE_HITBOX
            scrn -> draw(alienObj -> getHitboxShape());
        #endif
    }
}

// Checks whether a bullet has hit an alien
void checkHit(std::deque<Entity>* alienVector, std::deque<Entity>::iterator bulletObj, sf::Sound* hitSound)
{
    for (std::deque<Entity>::iterator alienObj = alienVector -> begin(); alienObj != alienVector -> end();) {
        if (bulletObj -> getHitbox() -> intersects(*alienObj -> getHitbox())) 
        {
            hitSound -> play();                                  // Play a hit sound
            alienObj = alienVector -> erase(alienObj);           // Remove the alien from the vector
            bulletObj -> getSprite() -> setPosition(0.0, YMIN);  // Set bullet out of bounds to be reloaded
        } else { ++alienObj; }
    }
}
