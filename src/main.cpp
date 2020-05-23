#include "../include/util.hpp"

int main(int argc, char* argv[])
{
    /* Set game window size and framerate */
    sf::Clock gameClock;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game");
    window.setFramerateLimit(100);

    /* Disable repeat key press if a key is held down */
    window.setKeyRepeatEnabled(false);

    /* Load background */
    sf::Texture space;
    sf::Sprite background;
    space.loadFromFile("../Textures/spacebg.jpg");
    background.setTexture(space);

    /* Load sounds */
    Sounds shot("../Sounds/shoot.wav");
    Sounds outOfAmmo("../Sounds/click.wav");
    Sounds hit("../Sounds/shotdown.wav");
    Sounds explode("../Sounds/explode.wav");
    Sounds alienMove1("../Sounds/movement1.wav");
    Sounds alienMove2("../Sounds/movement2.wav");

    /* Load fonts */
    sf::Text* text = loadFont("../Fonts/retro.ttf",        // Font path 
                              sf::Vector2f(0.75, 0.75),    // Text scale
                              50);                         // Text size

    /* Initialize player spaceship */
    Entity* ship = initShip(sf::Vector2f(500.0f, 920.0f),  // Sprite position
                            sf::Vector2f(493.0f, 940.0f),  // Hitbox position
                            sf::Vector2f(0.1f, .1f),       // Sprite size
                            sf::Vector2f(90.0f, 50.0f),    // Hitbox size
                            "../Textures/ship.png");       // Texture

    /* Initialize player ammo */
    Projectile* bullet = NULL;
    int ammoRemain = SHIP_AMMO;
    std::vector<Projectile*> shipAmmo;
    initAmmo(10, &shipAmmo, "../Textures/playerBullet.png", 
             sf::Vector2f(-500.0f, 920.0f),    // position
             sf::Vector2f(1.0f, 1.0f),         // sprite size
             sf::Vector2f(5.0f, 5.0f));        // hitbox size

    /* Initialize aliens */
    bool moveLeft = true;
    std::vector<Projectile*> alienAmmo;
    std::vector<std::vector<Entity>*> alienRefBlock; 
    initAliens(ALIEN_ROWS, ALIEN_COLUMNS, &alienRefBlock, &alienAmmo,
              "../Textures/alien.png", "../Textures/alienBullet4.png", "../Textures/alien2.png");
    
    /* Main game loop */
    while (window.isOpen())
    {
        /* Event Loop ------------------------------------------------------------------------ */
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Click the window to close it
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
            // Check keyboard events
            if (event.type == sf::Event::KeyPressed) 
            {
                switch(event.key.code) 
                {
                    /* Spacebar: Fire gun */
                    case sf::Keyboard::Space:
                        if (ammoRemain > 0) 
                        {
                            // Play a fire sound
                            shot.play();

                            /* Find a bullet that has not been fired */
                            for (std::vector<Projectile*>::iterator bullet = shipAmmo.begin();
                                bullet != shipAmmo.end();
                                bullet++)
                            {
                                /* Fire the bullet */
                                if ((*bullet) -> getStatus() == NOT_FIRED)
                                {
                                    (*bullet) -> setStatus(FIRED);
                                    ammoRemain--;
                                    break;
                                }
                            }
                        }
                        else // Out of ammo
                        {
                            outOfAmmo.play();
                            std::cout << "Reloading!\n" << std::endl;
                        }
                        break;

                    default :
                        break;
                }
            }
        }

        /* Game Frames ----------------------------------------------------------------------- */
        window.clear();

        /* Draw the background and player ship */
        window.draw(background);
        ship -> drawEntity(window);

        /* Update ammo text */
        std::string textString;
        text -> setFillColor(ammoRemain > 0 ? sf::Color(133, 202, 255, 255) : sf::Color::Red);
        if (ammoRemain > 0)
        {   // Print current ammo left
            textString = "Ammo: " + std::to_string(ammoRemain);
        }
        else
        {   // Print out of ammo
            if ((int)gameClock.getElapsedTime().asMilliseconds() % 2 == 0) 
            {
                text -> setFillColor(sf::Color::Transparent);
            }
            text -> setScale(sf::Vector2f(1.5, 1.5));
            text -> setPosition(225, 500);
            textString = "OUT OF AMMO!";
        }
        text -> setString(textString);
        
        /* Draw ammo text */
        window.draw(*text);

        // Update the positions of the aliens
        /* Loop through each column of aliens referenced by the reference block */
        for (std::vector<std::vector<Entity>*>::iterator column = alienRefBlock.begin(); 
             column != alienRefBlock.end(); 
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
                if (x >= X_MIN && moveLeft) 
                {
                    if ((int)gameClock.getElapsedTime().asSeconds() % 2 == 0) 
                    {
                        alienObj -> updateTexture(0);
                        alienObj -> setSpritePosition(sf::Vector2f(x - ALIEN_SPEED, y));
                        alienObj -> setHitboxPosition(sf::Vector2f((x + 10) - ALIEN_SPEED, y + 10));   
                    }else { alienObj -> updateTexture(1); }
                               
                } else { moveLeft = false; }

                /* Move aliens left: (1) Not reached X_MAX, (2) Pause every other second */
                if (x <= X_MAX && !(moveLeft)) 
                {
                    if ((int)gameClock.getElapsedTime().asSeconds() % 2 == 0) 
                    {   
                        alienObj -> updateTexture(0);
                        alienObj -> setSpritePosition(sf::Vector2f(x + ALIEN_SPEED, y));
                        alienObj -> setHitboxPosition(sf::Vector2f((x + 10) + ALIEN_SPEED, y + 10));
                    } else { alienObj -> updateTexture(1); }

                } else { moveLeft = true; }

                /* Draw living aliens */
                alienObj -> drawEntity(window);
            }
        }

        // Update the positions of bullets fired by the aliens 
        /* Loop through each column in aliens via the reference block */
        for (size_t column = 0; column < alienRefBlock.size(); column++) 
        { 
            /* Select the alien's bullet for this column */
            bullet = alienAmmo[column];
            float bullet_x = bullet -> getPosition().x;
            float bullet_y = bullet -> getPosition().y;

            /* Select the living alien at the bottom of the column */
            Entity* alien = NULL;
            for (std::vector<Entity>::iterator it = alienRefBlock[column] -> begin();
                    it != alienRefBlock[column] -> end();
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
                        /* Ship is hit while alive: Remove a life or game over */
                        if (ship -> getStatus() == ALIVE)
                        {
                            /* Play an explosion sound */
                            explode.play();

                            /* If the ship has more lives respawn and continue playing */
                            int currentLives = ship -> getLives();
                            if (currentLives > 0)
                            {
                                /* Decrement total lives and update ship's status */
                                ship -> setLives(--currentLives); ship -> setStatus(DEAD);

                                // /* Move the ship off screen temporarily */
                                // ship -> setSpritePosition(sf::Vector2f(-500, -2000));
                                // ship -> setHitboxPosition(sf::Vector2f(-500, -2000));

                                /* Move the ship back into the game window and continue playing */
                                printf("continue\n");
                            }
                            /* Game over: the ship has no more lives */
                            else
                            {
                                ship -> getSprite() -> setColor(sf::Color::Transparent);
                                printf("GAME OVER\n");
                            }
                        }
                        /* Ship is hit while dead: Ignore the hits and respawn the ship */
                        if (ship -> getStatus() == DEAD)
                        {
                            /* Move the ship back to the game screen */
                            ship -> setSpritePosition(sf::Vector2f(500.0f, 920.0f));
                            ship -> setHitboxPosition(sf::Vector2f(493.0f, 940.0f));


                            // for (int i=0; i < 10; i++)
                            // {
                            //     if ((int) clock.getElapsedTime().asMilliseconds() % 2 == 0)
                            //     {
                            //         ship -> setTexture("../Textures/transparent.png");
                            //     }
                            //     else
                            //     {
                            //         ship -> setTexture("../Textures/ship.png");
                            //     }   
                            // }
                        }
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

        // Update the positions of bullets fired by the player 
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
            for (std::vector<std::vector<Entity>*>::iterator column = alienRefBlock.begin(); 
                column != alienRefBlock.end(); 
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
                        hit.play();                       // Play a hit sound
                        alienObj -> setStatus(DEAD);        // Update alien status
                        (*bullet) -> setStatus(NOT_FIRED);  // Update bullet status
                    }
                }
            }

            /* Draw the bullet */
            (*bullet) -> drawProjectile(window);
        }

        /* Update ship position given keyboard input */
        moveShip(ship);

        /* Display the frame in the game window */
        window.display();
    }

    /* Cleanup */
    free(ship);

    return 0;
}
