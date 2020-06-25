//
// Created by Prajwal Prakash on 24/06/20.
//

#ifndef SANDBOX_PLAYER_H
#define SANDBOX_PLAYER_H

#include "Ambient.h"
#include <glm/glm.hpp>

class Player
{
public:
    Player() {}

    ~Player() = default;

    glm::vec2 Position = { 0.0f, 0.0f };
    glm::vec2 Velocity = { 0.0f, 0.0f };

    char PreviousWalkPose = '2';
    float PreviousWalkPoseSetAt = 0.0f;
    bool IsOnGround = true;

    Ambient::Ref<Ambient::SubTexture2D> m_ActiveTexture;
};


#endif //SANDBOX_PLAYER_H
