#include <iostream>  // std::cout
#include <iterator>  // std::advance, std::iterator
#include <list>      // std::list
#include <vector>    // vector class
#include <array>     // array class

#include "bullet.hpp"

/* 
 *  '::' scope resolution operator
 *       Access global when local of same name
 *       Used to separate names, e.g. class from constructor
 * 
 *       Shows which namespace or class a symbol belongs
 * 
 * TODO:
 *    vectors
 *    iterators
 *    array class -> safety features of other std library containers
 *                   resembles std::vector
 *    const auto keyword in loops
 *    enum
 */

int main(int argc, char* argv[]) {

    /* Declare vector */
    std::vector<int> arr = {1, 2, 3, 4, 5};

    /* for-loop */
    std::cout << "\nWith a for-loop:" << std::endl;
    for (long unsigned int i=0; i < arr.size(); i++) {
        std::cout << arr[i] << std::endl;
    }

    /* Iterator */
    std::cout << "\nWith an iterator:" << std::endl;
    std::vector<int>::iterator it;
    for(it = arr.begin(); it != arr.end(); it++) {
        std::cout << *it << std::endl;
    }

    /* Array of strings */
    std::array<std::string, 3> stuff = {"apple", "banana", "pineapple"};
    for (const auto &item : stuff) {
        std::cout << item << std::endl;
    }

    /* Test class stuff */

}