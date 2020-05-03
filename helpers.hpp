#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <deque>

/* 
 *  cObject Class:
 *     Creates 'collision objects' where each object is a shape with
 *     an attached hitbox. The hitbox can detect collisions by checking
 *     if an intersection with another collision object occurred.
 */
class cObject
{
    private:
        sf::RectangleShape shape;
        sf::FloatRect hitbox;

    public:
        /* Constructor: Create the collision rectangle --------- */
        cObject() 
        {
            // Default constructor
        }
        cObject(sf::Vector2f coord, sf::Vector2f objSize, 
                sf::Vector2f hitboxSize, sf::Color color)
        {
            /* Initialize hitbox */
            sf::FloatRect objHitbox; 
            objHitbox.left = coord.x; 
            objHitbox.top = coord.y; 
            objHitbox.width = hitboxSize.x; 
            objHitbox.height = hitboxSize.y;

            /* Initialize object shape */
            sf::RectangleShape objShape;
            objShape.setSize(objSize);
            objShape.setPosition(coord);
            objShape.setFillColor(color);

            /* Save the hitbox and shape addresses */
            hitbox = objHitbox;
            shape = objShape;
        }

        /* Member functions --------------- */
        sf::RectangleShape* Shape()
        {
            return &shape;  // Return shape address
        }

        sf::FloatRect* Hitbox()
        {
            return &hitbox;  // Return hitbox address
        }
        
        /* Destructor --------------- */
        ~cObject()
        {
            std::cout << "Bullet fizzled out\n" << std::endl;
        }
};

/* 
 *  Function prototypes
 */
void initAmmo(int numBullets, std::deque<sf::Sprite>* bulletVector, sf::Texture* bulletTexture);
void initAliens(int numAliens, std::deque<sf::Sprite>* alienVector, sf::Texture* alienTexture);
void updateAmmoText(int ammoRemaining, sf::Text* displayedText, sf::Clock gameClock);