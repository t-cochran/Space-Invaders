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
#define MAXAMMO     10
#define VOLUME      10

sf::Vector2f globalShipPos;

int main(int argc, char* argv[])
{
    /* Set game window size and framerate */
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game");
    window.setFramerateLimit(100);

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

    /* Load sounds */
    sf::Sound shot, outOfAmmo;
    sf::SoundBuffer laserBuf, clickBuf;
    laserBuf.loadFromFile("Sounds/laser.wav");
    clickBuf.loadFromFile("Sounds/click.wav");
    outOfAmmo.setBuffer(clickBuf);
    shot.setBuffer(laserBuf);
    shot.setVolume(VOLUME);
    outOfAmmo.setVolume(VOLUME);

    /* Initialize ammo and aliens vectors */
    std::vector<sf::Sprite> ammo, reload, aliens;
    initAmmo(MAXAMMO, &ammo, &bulletTexture);
    initAliens(4, &aliens, &alienTexture);

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
                        if (ammo.size() > 0) 
                        {
                            // Play a fire sound
                            shot.play();

                            // Remove a bullet from total ammo
                            bullet = ammo.back();
                            ammo.pop_back();

                            // Set the bullet's position equal to the ship's position
                            bullet.setPosition(ship.getPosition());

                            // Set the bullet as reloading
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

        /* Draw the background and ship */
        window.draw(background);
        window.draw(ship);

        /* Draw the alien */
        window.draw(alien);
        
        /* Update the ammo text */
        std::string textString;
        text.setFillColor(ammo.size() > 0 ? sf::Color::Cyan : sf::Color::Red);
        if (ammo.size() > 0) // Print current ammo left
        {
            textString = "Ammo: " + std::to_string(ammo.size());
        }
        else // Print out of ammo
        {
            if ((int)std::round(clock.getElapsedTime().asMilliseconds()) % 2 == 0) {
                text.setFillColor(sf::Color::Transparent);
            }
            text.setScale(sf::Vector2f(1.5, 1.5));
            text.setPosition(225, 500);
            textString = "OUT OF AMMO!";
        }

        /* Draw the ammo text */
        text.setString(textString);
        window.draw(text);

        /* Draw the aliens */
        if (aliens.size() > 0) {

            // Move all aliens LEFT
            if (aliens.front().getPosition().x >= 0.0f && goLeft) 
            {
                for (std::vector<sf::Sprite>::iterator it = aliens.begin(); it != aliens.end(); it++) {
                    
                    std::cout << "GOING LEFT: x position: " << it -> getPosition().x << std::endl;

                    // move aliens left
                    it -> setPosition(it -> getPosition().x - 2, it -> getPosition().y);
                    window.draw(*it);  // Draw the aliens at the current position
                }
            }
            else {
                goRight = true;
                goLeft = false;
            }

            // Move all aliens RIGHT
            if (aliens.front().getPosition().x <= 540.0f && goRight) 
            {
                for (std::vector<sf::Sprite>::iterator it = aliens.begin(); it != aliens.end(); it++) {
                    std::cout << "GOING RIGHT: x position: " << it -> getPosition().x << std::endl;

                    // move aliens right
                    it -> setPosition(it -> getPosition().x + 2, it -> getPosition().y);
                    window.draw(*it);  // Draw the aliens at the current position
                }
            }
            else {
                goLeft = true;
                goRight = false;
            }
        }

        /* Draw the bullets that were fired */
        if (reload.size() > 0) 
        {
            // Update the positions of the bullets that were fired
            for (std::vector<sf::Sprite>::iterator it = reload.begin(); it != reload.end(); it++) {
                it -> setPosition(it -> getPosition().x, it -> getPosition().y - BULLETSPEED);
                window.draw(*it);
            }
        }

        /* Move the ship left */
        sf::Vector2f shipPos = ship.getPosition();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && shipPos.x > LEFTMAX) 
        {
            ship.setPosition(shipPos.x - 5.0f, 0.0f);
        }

        /* Move the ship right */
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && shipPos.x < RIGHTMAX) 
        {
            ship.setPosition(shipPos.x + 5.0f, 0.0f);
        }

        /* Display the frame */
        window.display();
    }

    return 0;
}