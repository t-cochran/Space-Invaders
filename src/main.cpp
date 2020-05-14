#include <SFML/Audio.hpp>
#include "util.hpp"
#include <cstdio>
#include <stdlib.h>

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
    sf::Font retro;
    sf::Text* text = loadFont("../Fonts/retro.ttf",        // Font path 
                              &retro,                      // Text to receive font
                              sf::Vector2f(0.75, 0.75),    // Text scale
                              50);                         // Text size

    /* Initialize player spaceship */
    Entity* ship = initShip(sf::Vector2f(500.0f, 920.0f),  // Sprite position
                            sf::Vector2f(493.0f, 940.0f),  // Hitbox position
                            sf::Vector2f(0.1f, .1f),       // Sprite size
                            sf::Vector2f(90.0f, 50.0f),    // Hitbox size
                            "../Textures/ship.png");       // Texture

    /* Initialize player ammo */
    Projectile* bullet;
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
              "../Textures/alien.png", "../Textures/alienBullet.png", "../Textures/alien2.png");
    
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

        /* Draw the background */
        window.draw(background);

        /* Get the location of the player ship and draw it */
        float ship_x = ship -> getPosition().x;
        float ship_y = ship -> getPosition().y;
        ship -> drawEntity(window);

        /* Update ammo text */
        updateAmmoText(ammoRemain, text, gameClock, sf::Color(133, 202, 255, 255));
        window.draw(*text);

        /* Update alien positions */
        updateAlienPosition(&alienRefBlock, &window, &moveLeft, &gameClock, &alienMove1, &alienMove2);

        /* Update alien bullet positions */
        for (size_t column = 0; column < alienRefBlock.size(); column++) 
        { 
            /* Select the alien's bullet for this column */
            bullet = alienAmmo[column];
            Entity* alien = NULL;

            // if (bullet -> getPosition().y != alienRefBlock[column] -> front().getPosition().y + 30)
            // {
            //     bullet -> setSpritePosition(sf::Vector2f(bullet -> getPosition().x, bullet -> getPosition().y + ALIEN_BULLET_SPEED));
            //     bullet -> setHitboxPosition(sf::Vector2f(bullet -> getPosition().x, bullet -> getPosition().y + ALIEN_BULLET_SPEED));

            //     /* Draw the bullet */
            //     bullet -> drawProjectile(window);
            // }

            /* LIVING ALIENS: Select a living alien at the bottom of the column */
            for (std::vector<Entity>::iterator it = alienRefBlock[column] -> begin();
                 it != alienRefBlock[column] -> end();
                 it++)
            {
                /* Save the alien at the bottom of the column that is alive */
                if (it -> getStatus() == ALIVE)
                {
                    alien = &*it;
                } 
            }

            /* All aliens in this column are dead. Go-to the next column*/
            if (alien == NULL) { continue; }

            /* Get the current (x,y)-position of the alien */
            float alien_x = alien -> getPosition().x;
            float alien_y = alien -> getPosition().y;

            /* Get the current (x,y)-position of the bullet */
            float bullet_x = bullet -> getPosition().x;
            float bullet_y = bullet -> getPosition().y;

            /* Determine whether the alien should fire */
            int randNum = rand() % 100;
            if (randNum == 5 && alien -> getFiredStatus() == false) 
            {
                alien -> setFiredWeapon(true);
            }

            /* Bullet not fired or out of bounds: Set position to the alien's position */
            if (bullet -> getPosition().y >= Y_MAX || alien -> getFiredStatus() == false)
            {
                alien -> setFiredWeapon(false);
                bullet -> setSpritePosition(sf::Vector2f(alien_x + 30, alien_y + 30));
                bullet -> setHitboxPosition(sf::Vector2f(alien_x + 30, alien_y + 30));
            }
            
            /* Bullet fired */
            else
            {                
                /* Increment the position of the bullet across the game screen */
                bullet -> setSpritePosition(sf::Vector2f(bullet_x, bullet_y + ALIEN_BULLET_SPEED));
                bullet -> setHitboxPosition(sf::Vector2f(bullet_x, bullet_y + ALIEN_BULLET_SPEED));

                if (bullet -> getHitbox() -> intersects(*ship -> getHitbox()))
                {
                    printf("GAME OVER!\n");
                    explode.play();
                    ship -> getSprite() -> setColor(sf::Color::Transparent);
                }

                /* Draw the bullet */
                bullet -> drawProjectile(window);
            }            
        }

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
                        hit.play();                         // Play a hit sound
                        alienObj -> setStatus(DEAD);        // Update alien status
                        (*bullet) -> setStatus(NOT_FIRED);  // Update bullet status
                    }
                }
            }

            /* Draw the bullet */
            (*bullet) -> drawProjectile(window);
        }

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

        /* Display the frame */
        window.display();
    }

    /* Cleanup */
    free(ship);

    return 0;
}