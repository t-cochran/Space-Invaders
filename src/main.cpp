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
        updateAmmoText(ammoRemain, text, gameClock, sf::Color(133, 202, 255, 255));
        window.draw(*text);

        /* Update alien positions */
        updateAlienPosition(&alienRefBlock, &window, &moveLeft, &gameClock, 
                            &alienMove1, &alienMove2);

        /* Update alien bullet positions */
        updateAlienBullets(bullet, alienAmmo, alienRefBlock, ship, &explode, window);

        /* Update player bullet positions */
        updatePlayerBullets(bullet, shipAmmo, alienRefBlock, ship, &hit, window);

        /* Update ship position given keyboard input */
        moveShip(ship);

        /* Display the frame in the game window */
        window.display();
    }

    /* Cleanup */
    free(ship);

    return 0;
}