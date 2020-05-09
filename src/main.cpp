#include <SFML/Audio.hpp>
#include "helpers.hpp"
#include <cstdio>

int main(int argc, char* argv[])
{
    /* Set game window size and framerate */
    sf::Clock gameClock;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game");
    window.setFramerateLimit(100);
    window.setKeyRepeatEnabled(false);

    /* Load textures*/
    sf::Texture shipTexture, spaceTexture, bulletTexture, alienTexture, alienTexture2;
    shipTexture.loadFromFile("../Textures/ship.png");
    spaceTexture.loadFromFile("../Textures/spacebg.jpg");
    bulletTexture.loadFromFile("../Textures/bullet.png");
    alienTexture.loadFromFile("../Textures/alien.png");
    alienTexture2.loadFromFile("../Textures/alien.png");

    /* Load background */
    sf::Sprite background;
    background.setTexture(spaceTexture);

    /* Initialize player spaceship */
    Entity ship;
    ship.setSpritePosition(sf::Vector2f(500.0f, 920.0f));
    ship.setHitboxPosition(sf::Vector2f(490.0f, 940.0f));
    ship.setSpriteSize(sf::Vector2f(0.1f, .1f));
    ship.setHitboxSize(sf::Vector2f(90.0f, 50.0f));
    ship.setSpriteTexture(&shipTexture);

    /* Initialize ammo */
    int ammoRemain = MAXAMMO;
    std::deque<Entity> ammo, reload;
    initAmmo(10, &ammo, &bulletTexture);

    /* Initialize aliens */
    bool moveLeft = true;
    std::deque<Entity> aliens, aliens2, aliens3, aliens4;
    initAliens(10, &aliens,  &alienTexture, XMIN, 100, 0.07, 0.07);
    initAliens(10, &aliens2, &alienTexture, XMIN, 175, 0.07, 0.07);
    initAliens(10, &aliens3, &alienTexture, XMIN, 250, 0.07, 0.07);
    initAliens(10, &aliens4, &alienTexture, XMIN, 325, 0.07, 0.07);

    /* Load sounds */
    sf::Sound shot, outOfAmmo, hit;
    sf::SoundBuffer laserBuf, clickBuf, hitBuf;
    laserBuf.loadFromFile("../Sounds/shoot.wav");
    clickBuf.loadFromFile("../Sounds/click.wav");
    hitBuf.loadFromFile("../Sounds/showdown.wav");
    outOfAmmo.setBuffer(clickBuf); shot.setBuffer(laserBuf); hit.setBuffer(hitBuf);
    outOfAmmo.setVolume(VOLUME); shot.setVolume(VOLUME); hit.setVolume(VOLUME);

    /* Load fonts and texts */
    sf::Font arial;
    sf::Text text;
    arial.loadFromFile("../Fonts/arial.ttf");
    text.setFont(arial);
    text.setScale(sf::Vector2f(0.75, 0.75));
    text.setCharacterSize(50);
    
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
                            bullet = ammo.front();
                            ammo.pop_front();
                            ammoRemain--;

                            // Set the bullet's initial position to the ship's
                            bullet.getSprite() -> setPosition(ship.getPosition());

                            // Add the bullet to the reload queue
                            reload.push_back(bullet);
                        }
                        else // Out of ammo
                        {
                            outOfAmmo.play();
                            std::cout << "Reloading!\n" << std::endl;
                        }
                        break;
                    
                    case sf::Keyboard::R:
                        /* TODO: Respawn aliens for testing purposes */     
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
        updateAmmoText(ammoRemain, &text, gameClock);
        window.draw(text);

        /* Update alien positions */
        if (aliens.size() > 0) {
            spawnAliens(&aliens,  &window, &moveLeft, &gameClock);
            spawnAliens(&aliens2, &window, &moveLeft, &gameClock);
            spawnAliens(&aliens3, &window, &moveLeft, &gameClock);
            spawnAliens(&aliens4, &window, &moveLeft, &gameClock);     
        }

        /* Update bullet positions */
        for (std::deque<Entity>::iterator bulletObj = reload.begin(); bulletObj != reload.end(); bulletObj++) 
        {
            /* Push offscreen bullets back to the ammo vector */
            if (bulletObj -> getSprite() -> getPosition().y < YMIN)
            {
                ammo.push_back(*bulletObj);
                reload.pop_front();
            }
            /* Draw the bullets currently fired */
            else
            {
                /* Get the current position of the bullet object */
                float x = bulletObj -> getSprite() -> getPosition().x;
                float y = bulletObj -> getSprite() -> getPosition().y;

                /* Update the position of the bullet sprite and hitbox */
                bulletObj -> setSpritePosition(sf::Vector2f(x, y - BULLETSPEED));
                bulletObj -> setHitboxPosition(sf::Vector2f(x, y - BULLETSPEED));

                /* Detect a bullet hitting the aliens */
                checkHit(&aliens, bulletObj, &hit);
                checkHit(&aliens2, bulletObj, &hit);
                checkHit(&aliens3, bulletObj, &hit);
                checkHit(&aliens4, bulletObj, &hit);

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
            ship.setHitboxPosition(sf::Vector2f(ship.getPosition().x - 5.0f, ship.getPosition().y));
        }

        /* Right arrow key: Move the ship right */
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && ship.getPosition().x < XMAX) 
        {
            ship.setSpritePosition(sf::Vector2f(ship.getPosition().x + 5.0f, ship.getPosition().y));
            ship.setHitboxPosition(sf::Vector2f(ship.getPosition().x + 5.0f, ship.getPosition().y));
        }

        /* Display the frame */
        window.display();
    }

    return 0;
}