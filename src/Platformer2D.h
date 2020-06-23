//
// Created by Prajwal Prakash on 23/06/20.
//

#ifndef SANDBOX_PLATFORMER2D_H
#define SANDBOX_PLATFORMER2D_H

#include "Ambient.h"
#include "glm/gtc/matrix_transform.hpp"

class Platformer2D : public Ambient::Layer
{
public:
    Platformer2D();

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Ambient::Timestep ts) override;

    void OnEvent(Ambient::Event& e) override;

private:
    Ambient::OrthographicCameraController m_CameraController;

    const char* m_Level;
    uint32_t m_LevelWidth = 64;
    uint32_t m_LevelHeight = 16;

    float m_CameraPositionX = 0.0f;
    float m_CameraPositionY = 0.0f;

    uint32_t s_TileWidth = 16;
    uint32_t s_TileHeight = 16;

    uint32_t m_VisibleTilesX = 1280 / s_TileWidth;
    uint32_t m_VisibleTilesY = 720 / s_TileHeight;

    // Calculate the top-leftmost visible tile
    float m_OffsetX = m_CameraPositionX - (float)m_VisibleTilesX / 2.0f;
    float m_OffsetY = m_CameraPositionY - (float)m_VisibleTilesY / 2.0f;

    float m_PlayerPositionX = 0;
    float m_PlayerPositionY = 0;

    float m_PlayerVelocityX = 0;
    float m_PlayerVelocityY = 0;

    bool m_IsPlayerOnGround = false;
};

#endif // SANDBOX_PLATFORMER2D_H
