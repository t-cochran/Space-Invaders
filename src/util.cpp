#include "util.hpp"

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

// Update the position of the aliens for each frame
void 
updateAlienPosition(std::vector<std::vector<Entity>*>* RefBlock, 
                    sf::RenderWindow* scrn, bool* leftDir, 
                    sf::Clock* clock, Sounds* move1, Sounds* move2)
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

            /* Move aliens left: (1) Not reached X_MIN, (2) Pause every other second */
            if (x >= X_MIN && *leftDir) 
            {
                if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
                {
                    alienObj -> updateTexture(0);
                    alienObj -> setSpritePosition(sf::Vector2f(x - ALIEN_SPEED, y));
                    alienObj -> setHitboxPosition(sf::Vector2f((x + 10) - ALIEN_SPEED, y + 10));   
                }else { alienObj -> updateTexture(1); }
                           
            } else { *leftDir = false; }

            /* Move aliens left: (1) Not reached X_MAX, (2) Pause every other second */
            if (x <= X_MAX && !(*leftDir)) 
            {
                if ((int)clock -> getElapsedTime().asSeconds() % 2 == 0) 
                {   
                    alienObj -> updateTexture(0);
                    alienObj -> setSpritePosition(sf::Vector2f(x + ALIEN_SPEED, y));
                    alienObj -> setHitboxPosition(sf::Vector2f((x + 10) + ALIEN_SPEED, y + 10));
                } else { alienObj -> updateTexture(1); }

            } else { *leftDir = true; }

            /* Draw living aliens */
            alienObj -> drawEntity(*scrn);
        }
    }
}

// Update the position of the alien bullets each frame
void 
updateAlienBullets(Projectile* bullet, std::vector<Projectile*> alienAmmo, 
                   std::vector<std::vector<Entity>*> refBlock, Entity* ship,
                   Sounds* explode, sf::RenderWindow &window) 
{
    for (size_t column = 0; column < refBlock.size(); column++) 
    { 
        /* Select the alien's bullet for this column */
        bullet = alienAmmo[column];
        float bullet_x = bullet -> getPosition().x;
        float bullet_y = bullet -> getPosition().y;

        /* Select the living alien at the bottom of the column */
        Entity* alien = NULL;
        for (std::vector<Entity>::iterator it = refBlock[column] -> begin();
                it != refBlock[column] -> end();
                it++)
        {
            /* Complete the motion of active bullets until they are out of bounds */
            if (bullet -> getStatus() == FIRED)
            {   
                /* Increment the position of the bullet across the game screen */
                bullet -> setSpritePosition(sf::Vector2f(bullet_x, bullet_y + ALIEN_BULLET_SPEED));
                bullet -> setHitboxPosition(sf::Vector2f(bullet_x, bullet_y + ALIEN_BULLET_SPEED));
                
                /* Check if the bullet hits the player's spaceship */
                if (bullet -> getHitbox() -> intersects(*ship -> getHitbox()))
                {
                    printf("GAME OVER!\n");
                    explode -> play();
                    ship -> getSprite() -> setColor(sf::Color::Transparent);
                }

                /* Draw the bullet */
                bullet -> drawProjectile(window);
            }

            /* Reset the state of the bullet if it is out of bounds */
            if (bullet -> getPosition().y >= Y_MAX)
            {
                bullet -> setStatus(NOT_FIRED);
            }

            /* Get the living alien at the bottom of the column */
            if (it -> getStatus() == ALIVE)
            {
                alien = &*it;
            } 
        }

        /* Go-to the next column if: 
            *    (1) All aliens in the current column are dead
            *                      - or -
            *    (2) The bullet is already active and needs to go off screen
            */
        if (alien == NULL || bullet -> getStatus() == FIRED) { continue; }


        /*  Logic so-far:
            *    (1) The bullet is not active yet
            *    (2) The column has some alive aliens
            *  
            *  => Select a living alien at the bottom of the column to fire
            */  
        float alien_x = alien -> getPosition().x;
        float alien_y = alien -> getPosition().y;

        /* Determine whether the alien should fire */
        /* Alien fired! */
        if (rand() % 100 == 5) 
        {
            /* Update the state of the alien and the bullet */
            bullet -> setStatus(FIRED);
        
            /* Increment the position of the bullet across the game screen */
            bullet -> setSpritePosition(sf::Vector2f(bullet_x, bullet_y + ALIEN_BULLET_SPEED));
            bullet -> setHitboxPosition(sf::Vector2f(bullet_x, bullet_y + ALIEN_BULLET_SPEED));

            /* Draw the bullet */
            bullet -> drawProjectile(window); 
        }
        /* Alien didn't fire */
        else
        {
            /* Set the position of the bullet to the alien's */
            bullet -> setSpritePosition(sf::Vector2f(alien_x + 30, alien_y + 30));
            bullet -> setHitboxPosition(sf::Vector2f(alien_x + 30, alien_y + 30));
        }         
    }
}

// Update the position of the player ship's bullets each frame
void 
updatePlayerBullets(Projectile* bullet, std::vector<Projectile*> shipAmmo, 
                    std::vector<std::vector<Entity>*> refBlock, Entity* ship,
                    Sounds* hit, sf::RenderWindow &window)
{
    /* Get the location of the player ship */
    float ship_x = ship -> getPosition().x;
    float ship_y = ship -> getPosition().y;

    /* Update player bullet positions */
    for (std::vector<Projectile*>::iterator bullet = shipAmmo.begin();
        bullet != shipAmmo.end();
        bullet++)
    {            
        /* Get the current (x, y) position of the bullet */
        float bullet_x = (*bullet) -> getPosition().x;
        float bullet_y = (*bullet) -> getPosition().y;

        /* Bullet not fired */
        if ((*bullet) -> getStatus() == NOT_FIRED)
        {
            /* Set bullet position to ship's position and go-to the next bullet */
            (*bullet) -> setSpritePosition(sf::Vector2f(ship_x + 33.0f, ship_y));
            (*bullet) -> setHitboxPosition(sf::Vector2f(ship_x + 33.0f, ship_y));
            continue;
        }

        /* Bullet fired: Bullet out of bounds */
        if ((*bullet) -> getPosition().y <= Y_MIN)
        {
            /* Reset the bullet's status */
            (*bullet) -> setStatus(NOT_FIRED);

            /* Set the bullet's position to the ship's position */
            (*bullet) -> setSpritePosition(sf::Vector2f(ship_x + 33.0f, ship_y));
            (*bullet) -> setHitboxPosition(sf::Vector2f(ship_x + 33.0f, ship_y));
            continue;
        }

        /* Bullet fired: Bullet in bounds */
        if ((*bullet) -> getStatus() == FIRED)
        {
            /* Move the bullet forward */
            (*bullet) -> setSpritePosition(sf::Vector2f(bullet_x, bullet_y - SHIP_BULLET_SPEED));
            (*bullet) -> setHitboxPosition(sf::Vector2f(bullet_x, bullet_y - SHIP_BULLET_SPEED));
        }

        /* Detect a bullet hitting the aliens */
        for (std::vector<std::vector<Entity>*>::iterator column = refBlock.begin(); 
            column != refBlock.end(); 
            column++) 
        {
            /* Loop through each alien in a given column */
            for (std::vector<Entity>::iterator alienObj = (*column) -> begin(); 
                alienObj != (*column) -> end();
                alienObj++)
            {
                /* Check if the bullet intersects a live alien */
                if (alienObj -> getStatus() == ALIVE && (*bullet) -> getHitbox() -> intersects(*alienObj -> getHitbox())) 
                {
                    hit -> play();                         // Play a hit sound
                    alienObj -> setStatus(DEAD);        // Update alien status
                    (*bullet) -> setStatus(NOT_FIRED);  // Update bullet status
                }
            }
        }

        /* Draw the bullet */
        (*bullet) -> drawProjectile(window);
    }
}

// Utility function that updates the ammo remaining after each frame
void 
updateAmmoText(int ammoRemaining, sf::Text* displayedText, 
               sf::Clock clock, sf::Color color) 
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
        ship -> setSpritePosition(sf::Vector2f(ship_x - 5.0f, ship_y));
        ship -> setHitboxPosition(sf::Vector2f(ship_x - 5.0f, ship_y + 20));
    }

    /* Right arrow key: Move the ship right */
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && ship_x < X_MAX) 
    {
        ship -> setSpritePosition(sf::Vector2f(ship_x + 5.0f, ship_y));
        ship -> setHitboxPosition(sf::Vector2f(ship_x - 3.0f, ship_y + 20));
    }
}

// Utility function that sets the font used by the game
sf::Text* 
loadFont(const std::string path, sf::Font* font, 
                   sf::Vector2f scale, unsigned int size) 
{
    sf::Text* text = new sf::Text;
    font -> loadFromFile(path);
    text -> setFont(*font);
    text -> setScale(scale);
    text -> setCharacterSize(size);
    return text;
}