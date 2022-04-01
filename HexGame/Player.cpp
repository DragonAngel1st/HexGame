//
//  Player.cpp
//  CPP_testing
//
//  Created by Patrick Miron on 2022-02-23.
//

#include "Player.hpp"
//Default constructor creates an empty player.
Player::Player(){};
//Specialized constructor use by the HexGame program.
Player::Player(int ID, HexNodeState color, PlayerType type): ID(ID), color(color), type(type) {};
//To enable sorting the player. Used for debugging.
bool Player::operator==(const Player& other) {
    return (this->ID == other.ID);
};
