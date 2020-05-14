#include "util.hpp"
#include "entities.hpp"

// Initialize the player space ship
Entity* initShip(sf::Vector2f spritePos, sf::Vector2f hitboxPos, sf::Vector2f spriteSize, 
                 sf::Vector2f hitboxSize, const std::string shipTexture)
{
    Entity* ship = new Entity;
    ship -> setSpritePosition(spritePos);
    ship -> setHitboxPosition(hitboxPos);
    ship -> setSpriteSize(spriteSize);
    ship -> setHitboxSize(hitboxSize);
    ship -> setTexture(shipTexture);
    return ship;
}

// Initialize bullet entities and stores them in a vector
void initAmmo(int numBullets, std::vector<Projectile*>* ammoVector, const std::string texturePath, 
              sf::Vector2f position, sf::Vector2f spriteSize, sf::Vector2f hitboxSize) 
{
    /* Fill the bullet vector with bullets */
    for (int i=0; i < numBullets; i++) 
    {
        Projectile* bullet = new Projectile;
        bullet -> setSpritePosition(position);
        bullet -> setHitboxPosition(position);
        bullet -> setSpriteSize(spriteSize);
        bullet -> setHitboxSize(hitboxSize);
        bullet -> setTexture(texturePath);
        bullet -> setStatus(NOT_FIRED);
        ammoVector -> push_back(bullet);
    }
}

// Initialize alien sprites and put them into a vector
void initAliens(int rows, int columns, std::vector<std::vector<Entity>*>* RefBlock, 
                std::vector<Projectile*>* alienAmmo, const std::string alienTexture, 
                const std::string bulletTexture, const std::string alienTexture2)
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
            Entity* alienObj = new Entity;
            alienObj -> setSpritePosition(sf::Vector2f(XMIN + x, y + 75)); // Set alien position
            alienObj -> setHitboxPosition(sf::Vector2f(XMIN + x, y + 75)); // Set hitbox position
            alienObj -> setSpriteSize(sf::Vector2f(0.07, 0.07));           // Set alien size
            alienObj -> setHitboxSize(sf::Vector2f(50.0f, 50.0f));         // Set hitbox size
            alienObj -> setTexture(alienTexture);                          // Set alien texture
            alienObj -> setFiredWeapon(false);                             // Aliens have not fired
            alienObj -> setStatus(ALIVE);                                  // Aliens are alive
            alienObj -> loadTexture(alienTexture2);

            /* Allow the last row of aliens to shoot back */
            if (row == ALIEN_ROWS)
            {
                /* Initialize ammo for the last row of aliens */
                initAmmo(1, alienAmmo, bulletTexture, 
                        sf::Vector2f(XMIN + x + 30, y + 30),  // position
                        sf::Vector2f(1.0f, 1.0f),             // sprite size
                        sf::Vector2f(15.0f, 15.0f));          // hitbox size
            }

            /* Push the alien onto a a vector */
            alienEntityVector -> push_back(*alienObj);

            y += 75; // Increment y-axis distance between aliens
            row--;   // Go-to the next alien in the row
        }
        x += 75;  // Increment x-axis distance between aliens
        
        /* Add a reference to the vector to a reference block */
        RefBlock -> push_back(alienEntityVector);
    }
}

// Update the position of the aliens for each frame
void updateAlienPosition(std::vector<std::vector<Entity>*>* RefBlock, sf::RenderWindow* scrn, 
                         bool* leftDir, sf::Clock* clock, Sounds* move1, Sounds* move2)
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
            /* Ignore dead aliens */
            if (alienObj -> getStatus() == DEAD) { continue; }

            /* Get current (x, y) position of the alien */
            float x = alienObj -> getPosition().x;
            float y = alienObj -> getPosition().y;

            /* Move aliens left: (1) Not reached Xmin, (2) Pause every other second */
            if (x >= XMIN && *leftDir) 
            {
                if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
                {
                    alienObj -> updateTexture(0);
                    alienObj -> setSpritePosition(sf::Vector2f(x - ALIEN_SPEED, y));
                    alienObj -> setHitboxPosition(sf::Vector2f((x + 10) - ALIEN_SPEED, y + 10));   
                }else { alienObj -> updateTexture(1); }
                           
            } else { *leftDir = false; }

            /* Move aliens left: (1) Not reached Xmax, (2) Pause every other second */
            if (x <= XMAX && !(*leftDir)) 
            {
                if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
                {   
                    alienObj -> updateTexture(0);
                    alienObj -> setSpritePosition(sf::Vector2f(x + ALIEN_SPEED, y));
                    alienObj -> setHitboxPosition(sf::Vector2f((x + 10) + ALIEN_SPEED, y + 10));
                } else { alienObj -> updateTexture(1); }
            } else { *leftDir = true; }

            /* Draw living aliens */
            //alienObj -> updateTexture(1);
            alienObj -> drawEntity(*scrn);
        }
    }
}

// Checks whether a bullet has hit an alien
void checkHit(std::vector<std::vector<Entity>*>* RefBlock, 
              std::vector<Entity>::reverse_iterator bulletObj, 
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
            if (alienObj -> getStatus() == ALIVE && bulletObj -> getHitbox() -> intersects(*alienObj -> getHitbox())) 
            {
                hitSound -> play();                                  // Play a hit sound
                alienObj -> setStatus(DEAD);                         // Update alien status
                bulletObj -> getSprite() -> setPosition(0.0, YMIN);  // Set bullet out of bounds to be reloaded
            }
        }
    }
}

// Utility function that updates the ammo remaining after each frame
void updateAmmoText(int ammoRemaining, sf::Text* displayedText, sf::Clock clock, sf::Color color) 
{
        std::string textString;
        displayedText -> setFillColor(ammoRemaining > 0 ? color : sf::Color::Red);
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