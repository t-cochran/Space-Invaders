#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <unistd.h>
#include <deque>
#include <stdio.h>
#include <math.h>

#include "helpers.cpp"


#define XMIN        0.0f
#define XMAX        920.0f
#define YMIN        -50.0f
#define YMAX        920.0f
#define TOPMAX      -1000.0f
#define BULLETSPEED 15.0f
#define MAXAMMO     50
#define VOLUME      10

int main(int argc, char* argv[])
{
    /* Set game window size and framerate */
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game");
    window.setFramerateLimit(100);
    window.setKeyRepeatEnabled(false);

    /* Load textures*/
    sf::Texture shipTexture, spaceTexture, bulletTexture, alienTexture;
    shipTexture.loadFromFile("Textures/ship.png");
    spaceTexture.loadFromFile("Textures/spacebg.jpg");
    bulletTexture.loadFromFile("Textures/bullet2.png");
    alienTexture.loadFromFile("Textures/alien.png");

    /* Load sprites */
    sf::Sprite background, ship, alien;
    background.setTexture(spaceTexture);
    ship.setTexture(shipTexture);
    ship.scale(sf::Vector2f(0.1f, .1f));
    ship.setPosition(500.0f, 920.0f);

    /* Initialize ammo and aliens vectors */
    cObject bullet;
    std::deque<cObject> ammo, reload;
    std::deque<sf::Sprite> aliens;
    initAmmo(10, &ammo, &bulletTexture);
    initAliens(4, &aliens, &alienTexture);

    /* Load sounds */
    sf::Sound shot, outOfAmmo;
    sf::SoundBuffer laserBuf, clickBuf;
    laserBuf.loadFromFile("Sounds/laser.wav");
    clickBuf.loadFromFile("Sounds/click.wav");
    outOfAmmo.setBuffer(clickBuf);
    shot.setBuffer(laserBuf);
    shot.setVolume(VOLUME);
    outOfAmmo.setVolume(VOLUME);

    /* Load fonts */
    sf::Font arial;
    arial.loadFromFile("Fonts/arial.ttf");

    /* Load texts */
    sf::Text text; 
    text.setFont(arial);
    text.setScale(sf::Vector2f(0.75, 0.75));
    text.setCharacterSize(50);

    /* Bools to control alien movement */
    bool goLeft = true, goRight = false;

    /* Track running total ammo */
    int ammoRemain = MAXAMMO;

    /* Initialize game clock */
    sf::Clock clock;

    /* Initialize target dummy boxes */
    cObject dummy(sf::Vector2f(325.0f, 300.0f), 
                  sf::Vector2f(50.0f, 50.0f), 
                  sf::Vector2f(50.0f, 50.0f), 
                  sf::Color::Green);
    cObject dummy2(sf::Vector2f(450.0f, 386.0f), 
                   sf::Vector2f(50.0f, 50.0f), 
                   sf::Vector2f(50.0f, 50.0f), 
                   sf::Color::Green);
    cObject dummy3(sf::Vector2f(200.0f, 400.0f), 
                   sf::Vector2f(50.0f, 50.0f), 
                   sf::Vector2f(50.0f, 50.0f), 
                   sf::Color::Green);
    cObject dummy4(sf::Vector2f(500.0f, 500.0f), 
                   sf::Vector2f(50.0f, 50.0f), 
                   sf::Vector2f(50.0f, 50.0f), 
                   sf::Color::Green);

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
                            bullet = ammo.front();
                            ammo.pop_front();
                            ammoRemain--;

                            // Set the bullet's initial position to the ship's
                            bullet.Shape() -> setPosition(ship.getPosition());

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
                        dummy.Shape() -> setFillColor(sf::Color::Green);
                        break;

                    default :
                        break;
                }
            }
        }

        /* Game Frames ----------------------------------------------------------------------- */
        window.clear();

        /* Draw the background, ship, and aliens */
        window.draw(background);
        window.draw(ship);
        window.draw(alien);

        /* Draw green target dummies */
        window.draw(*dummy.Shape());
        window.draw(*dummy2.Shape());
        window.draw(*dummy3.Shape());
        window.draw(*dummy4.Shape());
        
        /* Update ammo text */
        updateAmmoText(ammoRemain, &text, clock);
        window.draw(text);

        /* Update aliens position */
        if (aliens.size() > 0) {

            // Move all aliens LEFT
            if (aliens.front().getPosition().x >= 0.0f && goLeft) 
            {
                for (std::deque<sf::Sprite>::iterator it = aliens.begin(); it != aliens.end(); it++) 
                {
                    // move aliens left
                    it -> setPosition(it -> getPosition().x - 2, it -> getPosition().y);
                    window.draw(*it);  // Draw the aliens at the current position
                }
            }
            else 
            {   // Left boundary reached: move aliens right
                goRight = true;
                goLeft = false;
            }

            // Move all aliens RIGHT
            if (aliens.front().getPosition().x <= 540.0f && goRight) 
            {
                for (std::deque<sf::Sprite>::iterator it = aliens.begin(); it != aliens.end(); it++) 
                {
                    // move aliens right
                    it -> setPosition(it -> getPosition().x + 2, it -> getPosition().y);
                    window.draw(*it);  // Draw the aliens at the current position
                }
            }
            else 
            {   // Right boundary reached: move aliens left
                goLeft = true;
                goRight = false;
            }
        }

        /* Update bullet position */
        for (std::deque<cObject>::iterator it = reload.begin(); it != reload.end(); it++) 
        {
            /* Offscreen bullets go back to the ammo vector */
            if (it -> Shape() -> getPosition().y < YMIN)
            {
                it -> Shape() -> setPosition(ship.getPosition());
                ammo.push_back(*it);
                reload.pop_front();
            }
            /* Draw the bullets currently fired */
            else 
            {
                /* Update the position of the bullet */
                float x = it -> Shape() -> getPosition().x;
                float y = it -> Shape() -> getPosition().y - BULLETSPEED;
                it -> Shape() -> setPosition(sf::Vector2f(x, y));

                /* Update the position of the bullet's hitbox */
                it -> Hitbox() -> left = x;
                it -> Hitbox() -> top = y;

                /* Detect a collisions with target dummies */
                if (it -> Hitbox() -> intersects(*dummy.Hitbox())) 
                {
                    std::cout << "HIT!" << std::endl;
                    dummy.Shape() -> setFillColor(sf::Color::Transparent);
                } 
                else if (it -> Hitbox() -> intersects(*dummy2.Hitbox())) 
                {
                    std::cout << "HIT!" << std::endl;
                    dummy2.Shape() -> setFillColor(sf::Color::Transparent);
                } 
                else if (it -> Hitbox() -> intersects(*dummy3.Hitbox())) 
                {
                    std::cout << "HIT!" << std::endl;
                    dummy3.Shape() -> setFillColor(sf::Color::Transparent);
                } 
                else if (it -> Hitbox() -> intersects(*dummy4.Hitbox())) 
                {
                    std::cout << "HIT!" << std::endl;
                    dummy4.Shape() -> setFillColor(sf::Color::Transparent);
                }
                window.draw(*it -> Shape());   
            }
        }

        /* Move the ship left */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && ship.getPosition().x > XMIN) 
        {
            ship.setPosition(ship.getPosition().x - 5.0f, ship.getPosition().y);
        }

        /* Move the ship right */
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && ship.getPosition().x < XMAX) 
        {
            ship.setPosition(ship.getPosition().x + 5.0f, ship.getPosition().y);
        }

        /* Display the frame */
        window.display();
    }

    return 0;
}