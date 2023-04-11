//
//  HexNodeState.hpp
//  HexGame
//
//  Created by Patrick Miron on 2022-02-15.
//
#include <iostream>

#ifndef PlayerType_hpp
#define PlayerType_hpp

#include <stdio.h>
//Enum to mark a square as owned by Blue, Red or Unoccupied.
enum class PlayerType
{
    HUMAN, AI
};
//This next constant expression is to enable printing the enum as text during debugging.
constexpr const char* PlayerTypeToString(PlayerType type) throw()
{
    switch (type)
    {
        case PlayerType::HUMAN: return "Human";
        case PlayerType::AI: return "AI";
        default: throw std::invalid_argument("Unimplemented item");
    }
}
#endif /* PlayerType_hpp */
