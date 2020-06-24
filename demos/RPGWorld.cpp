//
// Created by Prajwal Prakash on 23/06/20.
//

#include "RPGWorld.h"

static const char* layer1 = "................................................"
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

static const char* layer2 = "                                                "
                            "     T                            T             "
                            "                                                "
                            "                              T     T           "
                            "                                                "
                            "         T                                      "
                            "                                                "
                            "           T                                    "
                            "                                                "
                            "            T                                   "
                            "                                                ";

static std::array<const char*, 2> layers;

RPGWorld::RPGWorld() : Layer("RPGWorld"), m_CameraController(1280.0f / 720.0f)
{
}

void RPGWorld::OnAttach()
{
    auto sprite_sheet = Ambient::Texture2D::Create("./../../assets/textures/RPGpack_sheet_2X.png");
    glm::vec2 cell_size = { 128.0f, 128.0f };

    m_TextureAtlas = std::make_shared<Ambient::TextureAtlas2D>(sprite_sheet, cell_size);

    m_TextureMap['.'] = m_TextureAtlas->GetSubTexture(1, 11, 1, 1);
    m_TextureMap['W'] = m_TextureAtlas->GetSubTexture(8, 11, 1, 1);
    m_TextureMap['D'] = m_TextureAtlas->GetSubTexture(13, 11, 1, 1);
    m_TextureMap['T'] = m_TextureAtlas->GetSubTexture(0, 3, 1, 1);

    layers[0] = layer1;
    layers[1] = layer2;

    m_CameraController.SetZoomLevel(2.25f);
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

    for (auto& layer: layers)
    {
        for (uint32_t y = 0; y < 11; y++)
        {
            for (uint32_t x = 0; x < 48; x++)
            {
                char tile = layer[x + y * 48];

                if (tile == ' ') continue;

                auto texture = m_TextureMap[tile];
                Ambient::Renderer2D::DrawQuad({ x, y, 0.0f }, { 1.0f, 1.0f }, texture);
            }
        }
    }

    // To use uneven sized sprites, use the following API
    // Note the relation between the size of the squad and the size of the sprite (in units)

    // Ambient::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, m_TextureAtlas->GetSubTexture(1, 10, 1, 1));
    // Ambient::Renderer2D::DrawQuad({ 0.5f, 0.0f }, { 1.0f, 2.0f }, m_TextureAtlas->GetSubTexture(1, 10, 2, 4));
    // Ambient::Renderer2D::DrawQuad({ 1.5f, 0.0f }, { 1.5f, 3.0f }, m_TextureAtlas->GetSubTexture(1, 10, 3, 6));

    Ambient::Renderer2D::EndScene();
}

void RPGWorld::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}
