//
//  HexNodeState.hpp
//  HexGame
//
//  Created by Patrick Miron on 2022-02-15.
//

#ifndef HexNodeState_hpp
#define HexNodeState_hpp

#include <stdio.h>
//Enum to mark a square as owned by Blue, Red or Unoccupied.
enum class HexNodeState
{
    UNOCCUPIED, RED, BLUE
};
//This next constant expression is to enable printing the enum as text during debugging.
constexpr const char* HexNodeStateToString(HexNodeState state) throw()
{
    switch (state)
    {
        case HexNodeState::UNOCCUPIED: return "Unoccupied";
        case HexNodeState::RED: return "Red";
        case HexNodeState::BLUE: return "Blue";
        default: throw std::invalid_argument("Unimplemented item");
    }
}

#endif /* HexNodeState_hpp */
