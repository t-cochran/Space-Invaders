#include "util.hpp"
#include "entities.hpp"

// Initialize bullets as collision objects and put them into a vector
void initAmmo(int numBullets, Entity* gameEntity, const std::string texturePath, 
              sf::Vector2f position, sf::Vector2f spriteSize, sf::Vector2f hitboxSize) {

    /* Fill the bullet vector with bullets */
    for (int i=0; i < numBullets; i++) 
    {
        Entity bulletObj;
        bulletObj.setSpritePosition(position);
        bulletObj.setHitboxPosition(position);
        bulletObj.setSpriteSize(spriteSize);
        bulletObj.setHitboxSize(hitboxSize);
        bulletObj.setTexture(texturePath);
        gameEntity -> setBullet(bulletObj);
    }
}

// Initialize alien sprites and put them into a vector
void initAliens(int numAliens, std::vector<Entity>* alienVector, const std::string alienTexture, 
                const std::string bulletTexture, int x, int y, float width, float length) {

    /* Fill the alien vector with aliens */
    for (int i=0; i < numAliens; i++) 
    {
        /* Create an alien */
        Entity alienObj;
        alienObj.setSpritePosition(sf::Vector2f(x + (i*80), y));  // Set alien position
        alienObj.setHitboxPosition(sf::Vector2f(x + (i*80), y));  // Set alien hitbox position
        alienObj.setSpriteSize(sf::Vector2f(width, length));      // Set alien size
        alienObj.setHitboxSize(sf::Vector2f(50.0f, 50.0f));       // Set alien hitbox size
        alienObj.setTexture(alienTexture);                  // Set alien texture

        /* Initialize the alien's bullets */
        initAmmo(1, &alienObj, bulletTexture, 
                 sf::Vector2f(x + 30 + (i*80), y + 30),   // bullet position (sprite and hitbox)
                 sf::Vector2f(1.0f, 1.0f),      // bullet sprite size
                 sf::Vector2f(15.0f, 15.0f));   // bullet hitbox size
        
        /* Add the alien to a row */
        alienVector -> push_back(alienObj);  // Add alien to the 'row' vector
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
void spawnAliens(std::vector<Entity>* alienVector, sf::RenderWindow* scrn, bool* leftDir, 
                 sf::Clock* clock)
{
    for (std::vector<Entity>::iterator alienObj = alienVector -> begin(); alienObj != alienVector -> end(); alienObj++) 
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
void checkHit(std::vector<Entity>* alienVector, std::deque<Entity>::iterator bulletObj, 
              Sounds* hitSound)
{
    for (std::vector<Entity>::iterator alienObj = alienVector -> begin(); alienObj != alienVector -> end();) {
        if (bulletObj -> getHitbox() -> intersects(*alienObj -> getHitbox())) 
        {
            hitSound -> play();                                  // Play a hit sound
            alienObj = alienVector -> erase(alienObj);           // Remove the alien from the vector
            bulletObj -> getSprite() -> setPosition(0.0, YMIN);  // Set bullet out of bounds to be reloaded
        } else { ++alienObj; }
    }
}

sf::Text* loadFont(const std::string path, sf::Font* font, sf::Vector2f scale, unsigned int size) 
{
    sf::Text* text = new sf::Text;
    font -> loadFromFile(path);
    text -> setFont(*font);
    text -> setScale(scale);
    text -> setCharacterSize(size);
    return text;
}