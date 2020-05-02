#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <unistd.h>
#include <deque>
#include <stdio.h>
#include <math.h>

#include "helpers.cpp"

#define LEFTMAX    -452
#define RIGHTMAX    470
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
    sf::Sprite background(spaceTexture), ship(shipTexture), bullet, alien;
    ship.scale(sf::Vector2f(0.1f, .1f));
    ship.setOrigin(sf::Vector2f(-4500.f, -9300.f));

    /* Initialize ammo and aliens vectors */
    std::deque<sf::Sprite> ammo, reload, aliens;
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

    /* Main game loop */
    while (window.isOpen())
    {
        /* Event Loop --------------------------------------------------------------------------- */
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
                            bullet.setPosition(ship.getPosition());

                            // Add the bullet to the reload queue
                            reload.push_back(bullet);
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

        /* Game Frames -------------------------------------------------------------------------- */
        window.clear();

        /* Draw the background, ship, and aliens */
        window.draw(background);
        window.draw(ship);
        window.draw(alien);
        
        /* Update ammo text */
        std::string textString;
        text.setFillColor(ammoRemain > 0 ? sf::Color::Cyan : sf::Color::Red);
        if (ammoRemain > 0)
        {   // Print current ammo left
            textString = "Ammo: " + std::to_string(ammoRemain);
        }
        else
        {   // Print out of ammo
            if ((int)std::round(clock.getElapsedTime().asMilliseconds()) % 2 == 0) 
            {
                text.setFillColor(sf::Color::Transparent);
            }
            text.setScale(sf::Vector2f(1.5, 1.5));
            text.setPosition(225, 500);
            textString = "OUT OF AMMO!";
        }

        /* Draw ammo text */
        text.setString(textString);
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
        for (std::deque<sf::Sprite>::iterator it = reload.begin(); it != reload.end(); it++) 
        {
            if (it -> getPosition().y < TOPMAX) // Add offscreen bullets back to ammo
            {
                it -> setPosition(ship.getPosition());
                ammo.push_back(*it);
                reload.pop_front();
            }
            else  // Draw the bullet that was fired
            {
                it -> setPosition(it -> getPosition().x, it -> getPosition().y - BULLETSPEED);
                window.draw(*it);   
            }
        }

        /* Move the ship left */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && ship.getPosition().x > LEFTMAX) 
        {
            ship.setPosition(ship.getPosition().x - 5.0f, 0.0f);
        }

        /* Move the ship right */
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && ship.getPosition().x < RIGHTMAX) 
        {
            ship.setPosition(ship.getPosition().x + 5.0f, 0.0f);
        }

        /* Display the frame */
        window.display();
    }

    return 0;
}