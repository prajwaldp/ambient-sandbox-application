//
// Created by Prajwal Prakash on 23/06/20.
//

#ifndef SANDBOX_PLATFORMGAME2D_H
#define SANDBOX_PLATFORMGAME2D_H

#include "Ambient.h"
#include "LevelManager.h"
#include "Player.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class PlatformGame2D : public Ambient::Layer
{
public:
    PlatformGame2D();

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Ambient::TimeStep ts) override;

    void OnEvent(Ambient::Event& e) override;

    // Helpers
    void RenderTiles();

    void DetectCollisions();

private:
    Ambient::OrthographicCameraController m_CameraController;

    Ambient::Ref<LevelManager> m_LevelManager;
    Ambient::Ref<Level> m_Level; // Current level data

    Player m_Player;

    Ambient::Ref<Ambient::TextureAtlas2D> m_CharacterTextureAtlas, m_WorldTextureAtlas;
    std::unordered_map<char, Ambient::Ref<Ambient::SubTexture2D>> m_TextureMap;
};

#endif // SANDBOX_PLATFORMGAME2D_H
