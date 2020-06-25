//
// Created by Prajwal Prakash on 24/06/20.
//

#ifndef SANDBOX_PLAYER_H
#define SANDBOX_PLAYER_H

#include <glm/glm.hpp>

class Player
{
public:
    Player() {}

    ~Player() = default;

    glm::vec2 Position = { 0.0f, 0.0f };
    glm::vec2 Velocity = { 0.0f, 0.0f };
};


#endif //SANDBOX_PLAYER_H
