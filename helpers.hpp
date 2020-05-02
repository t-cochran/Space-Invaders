#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <deque>

/*  Data hiding: 
 *    Can only access 'Private' data with member functions 
 * 
 *  Public -- Private -- Protected
 *    Access modifiers in C++ set accessibility of class members
 *    so cannot access directly outside of included functions.
 * 
 */
class Aliens
{
    private:
        sf::Sprite* self;

    public:
        /* Constructor --------------- */
        Aliens()
        {
            std::cout << "Default constructor called:\n" << std::endl;
        }

        /* Member functions --------------- */
        void setSprite(sf::Sprite* ptr)
        {
            self = ptr;
        }

        sf::Sprite* getSprite()
        {
            return self;
        }

        /* Destructor --------------- */
        ~Aliens()
        {
            std::cout << "Bullet fizzled out\n" << std::endl;
        }
};

void initAmmo(int numBullets, std::deque<sf::Sprite>* bulletVector, sf::Texture* bulletTexture);
void initAliens(int numAliens, std::deque<sf::Sprite>* alienVector, sf::Texture* alienTexture);