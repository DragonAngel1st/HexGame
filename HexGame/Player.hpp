//
//  Player.hpp
//  CPP_testing
//
//  Created by Patrick Miron on 2022-02-23.
//

#ifndef Player_hpp
#define Player_hpp
#include "PlayerType.hpp"
#include "HexNodeState.hpp"

class Player
{
public:
    //Default constructor creates an empty player.
    Player();
    //Specialized constructor use by the HexGame program.
    Player(int ID, HexNodeState color, PlayerType type);
    //To enable other object to know who the current player is.
    int ID;
    //The player is either a HUMAN or an AI. See PlayerType.hpp for the enum class of player type.
    PlayerType type;
    //The color of the square the player chose or is assigned.
    HexNodeState color;
    //To enable sorting the player. Used for debugging.
    bool operator==(const Player& other);
};

#include <stdio.h>

#endif /* Player_hpp */
