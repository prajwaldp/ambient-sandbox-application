//
// Created by Prajwal Prakash on 23/06/20.
//

#include "RPGWorld.h"

static const char* map_tiles = "................................................"
                               "................................................"
                               "...................WWWWWWW......................"
                               "...................WWWWWWW......................"
                               "...................WWWWWWW......................"
                               "...................WWWWWWW......................"
                               ".................................DDDDDDDD......."
                               ".................................DDDDDDDD......."
                               ".................................DDDDDDDD......."
                               ".................................DDDDDDDD......."
                               "................................................";

RPGWorld::RPGWorld() : Layer("RPGWorld"), m_CameraController(1280.0f / 720.0f)
{
}

void RPGWorld::OnAttach()
{
    m_SpriteSheet = Ambient::Texture2D::Create("./../../assets/textures/RPGpack_sheet_2X.png");

    m_TextureMap['.'] = Ambient::SubTexture2D::CreateFromCoords(m_SpriteSheet, 12, 15, 128.0f, 128.0f);
    m_TextureMap['W'] = Ambient::SubTexture2D::CreateFromCoords(m_SpriteSheet, 1, 1, 128.0f, 128.0f);
    m_TextureMap['D'] = Ambient::SubTexture2D::CreateFromCoords(m_SpriteSheet, 1, 1, 128.0f, 128.0f);
}

void RPGWorld::OnDetach()
{
}

void RPGWorld::OnUpdate(Ambient::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    Ambient::Renderer2D::ResetStats();
    Ambient::Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (uint32_t y = 0; y < 11; y++)
    {
        for (uint32_t x = 0; x < 24; x++)
        {
            char tile = map_tiles[x + y * 24];
            auto texture = m_TextureMap[tile];

            Ambient::Renderer2D::DrawQuad({ x, y, 0.5f }, {1.0f, 1.0f}, texture);
        }
    }

    Ambient::Renderer2D::EndScene();
}

void RPGWorld::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}
