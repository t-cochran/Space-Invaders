#include "../include/util.hpp"

// Initialize the player space ship
Entity* 
initShip(sf::Vector2f spritePos,  sf::Vector2f hitboxPos, 
         sf::Vector2f spriteSize, sf::Vector2f hitboxSize, 
         const std::string shipTexture)
{
    Entity* ship = new Entity;
    ship -> setSpritePosition(spritePos);
    ship -> setHitboxPosition(hitboxPos);
    ship -> setSpriteSize(spriteSize);
    ship -> setHitboxSize(hitboxSize);
    ship -> setTexture(shipTexture);
    ship -> setStatus(ALIVE);
    ship -> setLives(SHIP_LIVES);
    return ship;
}

// Initialize bullet entities and stores them in a vector
void 
initAmmo(int numBullets, std::vector<Projectile*>* ammoVector, 
         const std::string texturePath, sf::Vector2f position, 
         sf::Vector2f spriteSize, sf::Vector2f hitboxSize) 
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
void 
initAliens(int rows, int columns, std::vector<std::vector<Entity>*>* RefBlock, 
           std::vector<Projectile*>* alienAmmo, const std::string alienTexture, 
           const std::string bulletTexture, const std::string alienTexture2)
{
    int x = 0;
    /* Create vectors to store alien entities in columns */
    for (int col=0; col < columns; col++)
    {
        int y = 25;     // Initial alien position on y-axis
        int row = rows; // Total rows of aliens to be made

        /* Allocate a vector to hold a set of aliens in a column */
        std::vector<Entity>* alienEntityVector = new std::vector<Entity>;

        /* Fill the alien vector with alien entities */
        while (row > 0)
        {
            /* Create an alien entity */
            Entity* alienObj = new Entity;
            alienObj -> setSpritePosition(sf::Vector2f(X_MIN + x, y + 75)); // Set alien position
            alienObj -> setHitboxPosition(sf::Vector2f(X_MIN + x, y + 75)); // Set hitbox position
            alienObj -> setSpriteSize(sf::Vector2f(0.07, 0.07));            // Set alien size
            alienObj -> setHitboxSize(sf::Vector2f(50.0f, 50.0f));          // Set hitbox size
            alienObj -> setTexture(alienTexture);                           // Set alien textures
            alienObj -> loadTexture(alienTexture2);
            alienObj -> setStatus(ALIVE);                                   // Alien begins alive
            alienEntityVector -> push_back(*alienObj);                      // Put the alien in a vector

            /* Initialize ammo for the last row of aliens */
            if (row == ALIEN_ROWS)
            {
                initAmmo(1, alienAmmo, bulletTexture, 
                        sf::Vector2f(X_MIN + x + 30, y + 30),  // position
                        sf::Vector2f(1.0f, 1.0f),              // sprite size
                        sf::Vector2f(15.0f, 15.0f));           // hitbox size
            }

            y += 75; // Increment y-axis distance between aliens in the column
            row--;   // Go-to the next alien in the row
        }
        x += 75;  // Increment x-axis distance between columns
        
        /* Add a reference to the column of aliens */
        RefBlock -> push_back(alienEntityVector);
    }
}

// Control movement of the player space ship using left/right arrow keys
void
moveShip(Entity* ship)
{
    /* Get the current (x, y)-position of the ship */
    float ship_x = ship -> getPosition().x;
    float ship_y = ship -> getPosition().y;

    /* Left arrow key: move the ship left */
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && ship_x > X_MIN) 
    {
        ship -> setSpritePosition(sf::Vector2f(ship_x - SHIP_SPEED, ship_y));
        ship -> setHitboxPosition(sf::Vector2f(ship_x - SHIP_SPEED, ship_y + 20));
    }

    /* Right arrow key: Move the ship right */
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && ship_x < X_MAX) 
    {
        ship -> setSpritePosition(sf::Vector2f(ship_x + SHIP_SPEED, ship_y));
        ship -> setHitboxPosition(sf::Vector2f(ship_x - SHIP_SPEED, ship_y + 20));
    }
}

// Utility function that sets the font used by the game
sf::Text* 
loadFont(const std::string path, 
                   sf::Vector2f scale, unsigned int size) 
{
    sf::Text* text = new sf::Text;
    sf::Font* font = new sf::Font;
    font -> loadFromFile(path);
    text -> setFont(*font);
    text -> setScale(scale);
    text -> setCharacterSize(size);
    return text;
}
