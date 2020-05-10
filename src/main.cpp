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
    window.setKeyRepeatEnabled(false);

    /* Load background */
    sf::Sprite background; sf::Texture space;
    space.loadFromFile("../Textures/spacebg.jpg");
    background.setTexture(space);

    /* Load sounds */
    Sounds shot("../Sounds/shoot.wav");
    Sounds outOfAmmo("../Sounds/click.wav");
    Sounds hit("../Sounds/shotdown.wav");

    /* Load fonts */
    sf::Font arial;
    sf::Text* text = loadFont("../Fonts/arial.ttf", &arial, sf::Vector2f(0.75, 0.75), 50);

    /* Initialize player spaceship */
    Entity ship;
    ship.setSpritePosition(sf::Vector2f(500.0f, 920.0f));
    ship.setHitboxPosition(sf::Vector2f(493.0f, 940.0f));
    ship.setSpriteSize(sf::Vector2f(0.1f, .1f));
    ship.setHitboxSize(sf::Vector2f(90.0f, 50.0f));
    ship.setTexture("../Textures/ship.png");

    /* Initialize aliens */
    bool moveLeft = true;
    std::vector<Entity> alienRow1, alienRow2, alienRow3, alienRow4;
    initAliens(10, &alienRow1, "../Textures/alien.png", "../Textures/alienBullet.png", XMIN, 100, 0.07, 0.07);  // Create a row of 10 aliens
    initAliens(10, &alienRow2, "../Textures/alien.png", "../Textures/alienBullet.png", XMIN, 175, 0.07, 0.07);  // Create a row of 10 aliens
    initAliens(10, &alienRow3, "../Textures/alien.png", "../Textures/alienBullet.png", XMIN, 250, 0.07, 0.07);  // Create a row of 10 aliens
    initAliens(10, &alienRow4, "../Textures/alien.png", "../Textures/alienBullet.png", XMIN, 325, 0.07, 0.07);  // Create a row of 10 aliens

    /* Initialize player ammo */
    int ammoRemain = MAXAMMO;
    initAmmo(10, &ship, "../Textures/playerBullet.png", 
             sf::Vector2f(-500.0f, -920.0f),   // object position
             sf::Vector2f(1.0f, 1.0f),         // sprite size
             sf::Vector2f(5.0f, 5.0f));        // hitbox size
    
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

                            // Remove a bullet from the ammo queue
                            Entity bullet;
                            bullet = ship.getAmmoQueue() -> front();
                            ship.getAmmoQueue() -> pop_front();
                            ammoRemain--;

                            // Set the bullet's initial position to the ship's center
                            float x = ship.getPosition().x + 33.0f;
                            float y = ship.getPosition().y;
                            bullet.getSprite() -> setPosition(sf::Vector2f(x, y));

                            // Add the bullet to the reload queue
                            ship.setBulletReload(bullet);
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

        /* Draw the player ship */
        window.draw(*ship.getSprite());
        #if SEE_HITBOX
            window.draw(ship.getHitboxShape());
        #endif

        /* Update ammo text */
        updateAmmoText(ammoRemain, text, gameClock);
        window.draw(*text);

        /* Update alien positions */
        spawnAliens(&alienRow1, &window, &moveLeft, &gameClock);
        spawnAliens(&alienRow2, &window, &moveLeft, &gameClock);
        spawnAliens(&alienRow3, &window, &moveLeft, &gameClock);
        spawnAliens(&alienRow4, &window, &moveLeft, &gameClock);

        /* Make row 4 aliens fire */
        for (std::vector<Entity>::iterator it = alienRow4.begin(); it != alienRow4.end(); it++)
        {
            /* Fire a bullet if randNumb == 5 */
            int randNum = rand() % 100;
            if (randNum == 5) 
            {
                it -> setFiredWeapon(true);
            }

            /* Update the status of bullets that were not fired */
            if (it -> getFiredStatus() == false)
            {
                /* Get the current (x,y)-position of the alien */
                float x = it -> getSprite() -> getPosition().x;
                float y = it -> getSprite() -> getPosition().y;

                /* Update the unfired bullet sprite and hitbox positions */
                it -> getAmmoQueue() -> front().setSpritePosition(sf::Vector2f(x + 30, y + 30));
                it -> getAmmoQueue() -> front().setHitboxPosition(sf::Vector2f(x + 30, y + 30));
            }

            /* Update the status of bullets that were fired */
            if (it -> getFiredStatus() == true)
            {
                /* Reset the state of the alien after its bullet is out of bounds */
                if (it -> getAmmoQueue() -> front().getPosition().y > YMAX)
                {
                    it -> setFiredWeapon(false);
                }
                /* Draw the bullets fired that are in bounds */
                else
                {
                    /* Get the current (x,y)-position of the bullet */
                    float x = it -> getAmmoQueue() -> front().getPosition().x;
                    float y = it -> getAmmoQueue() -> front().getPosition().y;
                    
                    /* Increment the position of the bullet */
                    it -> getAmmoQueue() -> front().setSpritePosition(sf::Vector2f(x, y + ALIENBULLETSPEED));
                    it -> getAmmoQueue() -> front().setHitboxPosition(sf::Vector2f(x, y + ALIENBULLETSPEED));

                    /* Draw the bullet */
                    window.draw(*it -> getAmmoQueue() -> front().getSprite()); 
                    #if SEE_HITBOX
                        window.draw(*it -> getAmmoQueue() -> front().getHitboxShape());
                    #endif 
                }
            } 
        }

        /* Update player bullet positions */
        for (std::deque<Entity>::iterator bulletObj = ship.getReloadQueue() -> begin(); 
             bulletObj != ship.getReloadQueue() -> end(); 
             bulletObj++) 
        {
            /* Push offscreen bullets back to the player ammo vector */
            if (bulletObj -> getSprite() -> getPosition().y < YMIN)
            {
                ship.setBullet(*bulletObj);
                ship.getReloadQueue() -> pop_front();
            }
            /* Draw the bullets fired by the player */
            else
            {
                /* Get the current position of the bullet object */
                float x = bulletObj -> getSprite() -> getPosition().x;
                float y = bulletObj -> getSprite() -> getPosition().y;

                /* Update the position of the bullet sprite and hitbox */
                bulletObj -> setSpritePosition(sf::Vector2f(x, y - SHIPBULLETSPEED));
                bulletObj -> setHitboxPosition(sf::Vector2f(x, y - SHIPBULLETSPEED));

                /* Detect a bullet hitting the aliens */
                checkHit(&alienRow1, bulletObj, &hit);  // Check alien entities at row 1
                checkHit(&alienRow2, bulletObj, &hit);  // Check alien entities at row 2
                checkHit(&alienRow3, bulletObj, &hit);  // Check alien entities at row 3
                checkHit(&alienRow4, bulletObj, &hit);  // Check alien entities at row 4

                /* Draw the bullet */
                window.draw(*bulletObj -> getSprite()); 
                #if SEE_HITBOX
                    window.draw(bulletObj -> getHitboxShape());
                #endif  
            }
        }

        /* Left arrow key: move the ship left */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && ship.getPosition().x > XMIN) 
        {
            ship.setSpritePosition(sf::Vector2f(ship.getPosition().x - 5.0f, ship.getPosition().y));
            ship.setHitboxPosition(sf::Vector2f(ship.getPosition().x - 5.0f, ship.getPosition().y + 20));
        }

        /* Right arrow key: Move the ship right */
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && ship.getPosition().x < XMAX) 
        {
            ship.setSpritePosition(sf::Vector2f(ship.getPosition().x + 5.0f, ship.getPosition().y));
            ship.setHitboxPosition(sf::Vector2f(ship.getPosition().x - 3.0f, ship.getPosition().y + 20));
        }

        /* Display the frame */
        window.display();
    }

    return 0;
}