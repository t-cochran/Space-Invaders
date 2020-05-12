#include "util.hpp"
#include "entities.hpp"

// Initialize bullets as collision objects and put them into a vector
void initAmmo(int numBullets, Entity* gameEntity, const std::string texturePath, 
              sf::Vector2f position, sf::Vector2f spriteSize, sf::Vector2f hitboxSize) 
{

    /* Fill the bullet vector with bullets */
    for (int i=0; i < numBullets; i++) 
    {
        Entity bulletObj;
        bulletObj.setSpritePosition(position);
        bulletObj.setHitboxPosition(position);
        bulletObj.setSpriteSize(spriteSize);
        bulletObj.setHitboxSize(hitboxSize);
        bulletObj.setTexture(texturePath);
        gameEntity -> setBulletAmmo(bulletObj);
    }
}

// Initialize alien sprites and put them into a vector
void initAliens(int rows, int columns, std::vector<std::vector<Entity>*>* RefBlock, 
                const std::string alienTexture, const std::string bulletTexture)
{
    int x = 0;
    /* Create vectors to store alien entities in columns */
    for (int col=0; col < columns; col++)
    {
        int y = 25;     // Initial alien position on y-axis
        int row = rows; // Total rows of aliens to be made

        /* Allocate a vector for each column of aliens */
        std::vector<Entity>* alienEntityVector = new std::vector<Entity>;

        /* Fill the vector with alien entities */
        while (row > 0)
        {
            /* Create an alien entity */
            Entity alienObj;
            alienObj.setSpritePosition(sf::Vector2f(XMIN + x, y + 75)); // Set alien position
            alienObj.setHitboxPosition(sf::Vector2f(XMIN + x, y + 75)); // Set hitbox position
            alienObj.setSpriteSize(sf::Vector2f(0.07, 0.07));           // Set alien size
            alienObj.setHitboxSize(sf::Vector2f(50.0f, 50.0f));         // Set hitbox size
            alienObj.setTexture(alienTexture);                          // Set alien texture
            alienObj.setStatus(ALIVE);

            /* Initialize the alien's bullets */
            initAmmo(1, &alienObj, bulletTexture, 
                    sf::Vector2f(XMIN + x + 30, y + 30),  // bullet position (sprite and hitbox)
                    sf::Vector2f(1.0f, 1.0f),             // bullet sprite size
                    sf::Vector2f(15.0f, 15.0f));          // bullet hitbox size

            /* Push the alien onto a stack */
            alienEntityVector -> push_back(alienObj);

            y += 75; // Increment y-axis distance between aliens
            row--;   // Go-to the next alien in the row
        }
        x += 75;  // Increment x-axis distance between aliens
        
        /* Add a reference to the stack to a reference block */
        RefBlock -> push_back(alienEntityVector);
    }
}

// Update the position of the aliens for each frame
void updateAlienPosition(std::vector<std::vector<Entity>*>* RefBlock, sf::RenderWindow* scrn, 
                         bool* leftDir, sf::Clock* clock)
{
    /* Loop through each column of aliens referenced by the reference block */
    for (std::vector<std::vector<Entity>*>::iterator column = RefBlock -> begin(); 
         column != RefBlock -> end(); 
         column++) 
    {
        /* Loop through each alien in a given column */
        for (std::vector<Entity>::iterator alienObj = (*column) -> begin(); 
             alienObj != (*column) -> end();
             alienObj++)
        {
            /* Get current (x, y) position of the alien */
            float x = alienObj -> getPosition().x;
            float y = alienObj -> getPosition().y;

            /* Move aliens left: (1) Not reached Xmin, (2) Pause every other second */
            if (x >= XMIN && *leftDir) 
            {
                if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
                {
                    alienObj -> setSpritePosition(sf::Vector2f(x - ALIEN_SPEED, y));
                    alienObj -> setHitboxPosition(sf::Vector2f((x + 10) - ALIEN_SPEED, y + 10));   
                }            
            } else { *leftDir = false; }

            /* Move aliens left: (1) Not reached Xmax, (2) Pause every other second */
            if (x <= XMAX && !(*leftDir)) 
            {
                if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
                {
                    alienObj -> setSpritePosition(sf::Vector2f(x + ALIEN_SPEED, y));
                    alienObj -> setHitboxPosition(sf::Vector2f((x + 10) + ALIEN_SPEED, y + 10));
                }
            } else { *leftDir = true; }

            /* Draw living aliens */
            if (alienObj -> getStatus() == ALIVE)
            {
                scrn -> draw(*alienObj -> getSprite());
                #if SEE_HITBOX
                    scrn -> draw(alienObj -> getHitboxShape());
                #endif
            }
            
            /* Remove dead aliens */
            if (alienObj -> getStatus() == DEAD)
            {
                alienObj = (*column) -> erase(alienObj);
                if (alienObj == (*column) -> end())
                {
                    break;
                } 
                else
                {
                    ++alienObj;
                }
                
            }
        }
    }
}

// Checks whether a bullet has hit an alien
void checkHit(std::vector<std::vector<Entity>*>* RefBlock, 
              std::deque<Entity>::iterator bulletObj, 
              Sounds* hitSound)
{
    /* Loop through each column of aliens referenced by the reference block */
    for (std::vector<std::vector<Entity>*>::iterator column = RefBlock -> begin(); 
         column != RefBlock -> end(); 
         column++) 
    {
        /* Loop through each alien in a given column */
        for (std::vector<Entity>::iterator alienObj = (*column) -> begin(); 
             alienObj != (*column) -> end();
             alienObj++)
        {
            /* Check if the bullet intersects a live alien */
            if (alienObj ->getStatus() == ALIVE && bulletObj -> getHitbox() -> intersects(*alienObj -> getHitbox())) 
            {
                hitSound -> play();                                  // Play a hit sound
                alienObj -> setStatus(DEAD);                         // Update alien status
                bulletObj -> getSprite() -> setPosition(0.0, YMIN);  // Set bullet out of bounds to be reloaded
            }
            else
            {
                continue;
            }
        }
    }
}

// Utility function that updates the ammo remaining after each frame
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

// Utility function that sets the font used by the game
sf::Text* loadFont(const std::string path, sf::Font* font, sf::Vector2f scale, unsigned int size) 
{
    sf::Text* text = new sf::Text;
    font -> loadFromFile(path);
    text -> setFont(*font);
    text -> setScale(scale);
    text -> setCharacterSize(size);
    return text;
}