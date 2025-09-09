//---------------------------------------------------------------------------------------------------------------------
// c++.h
//
// <Explanation of the program ...>
// <... May have multiple lines.>
//
//---------------------------------------------------------------------------------------------------------------------
//
#include <iostream>
#include <thread>
/**
 * @file player.h
 * @brief Player class declaration.
 */

#pragma once
#include <string>
#include <vector>
/**
 * @class Player
 * @brief Represents a player in the game.
 */
class Player {
public:
    /**
     * @brief Construct a new Player.
     * @param name The player's name.
     */
    Player(const std::string& name);

    /**
     * @brief Make the player jump.
     */
    void jump();

private:
    std::string name_; ///< Player's name
};
