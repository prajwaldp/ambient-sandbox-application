#include "PlatformGame2D.h"
#include "LevelManager.h"

#include <memory>


PlatformGame2D::PlatformGame2D() : Layer("PlatformGame2D"), m_CameraController(1280.0f / 720.0f)
{
    m_LevelManager = std::make_shared<LevelManager>("./../../demos/PlatformGame2D/assets/levels");
    m_Level = m_LevelManager->GetLevel(0);

    m_CameraController.SetZoomLevel(10.0f);
}

void PlatformGame2D::OnAttach()
{
    // Load the world textures
    auto world_sprite_sheet = Ambient::Texture2D::Create(
            "./../../demos/PlatformGame2D/assets/textures/tiles.png");

    m_WorldTextureAtlas.reset(new Ambient::TextureAtlas2D(world_sprite_sheet, { 70.0, 70.0 }));

    // Load the character textures
    auto character_sprite_sheet = Ambient::Texture2D::Create(
            "./../../demos/PlatformGame2D/assets/textures/adventurer_tilesheet.png");

    m_CharacterTextureAtlas.reset(
            new Ambient::TextureAtlas2D(character_sprite_sheet, { 80.0f, 110.0f }));

    m_TextureMap['.'] = m_WorldTextureAtlas->GetSubTexture(1, 6, 1, 1);
    m_TextureMap['#'] = m_WorldTextureAtlas->GetSubTexture(1, 2, 1, 1);

    m_TextureMap['1'] = m_CharacterTextureAtlas->GetSubTexture(0, 2, 1, 1); // standing still
    m_TextureMap['2'] = m_CharacterTextureAtlas->GetSubTexture(0, 1, 1, 1); // walking 1
    m_TextureMap['3'] = m_CharacterTextureAtlas->GetSubTexture(1, 1, 1, 1); // walking 2
    m_TextureMap['4'] = m_CharacterTextureAtlas->GetSubTexture(8, 2, 1, 1); // jump

    m_Player.Position = {
            -10.0, 10
    };

    m_Player.m_ActiveTexture = m_TextureMap['1'];
    m_Player.IsOnGround = true;
}

void PlatformGame2D::OnDetach()
{
}

void PlatformGame2D::OnUpdate(Ambient::TimeStep ts)
{
    const float gravity = 0.2f;

    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    Ambient::Renderer2D::ResetStats();
    Ambient::Renderer2D::BeginScene(m_CameraController.GetCamera());

    RenderTiles();

    if (Ambient::Input::IsKeyPressed(AM_KEY_LEFT))
    {
        m_Player.Velocity.x -= 4.05f;
    }

    else if (Ambient::Input::IsKeyPressed(AM_KEY_RIGHT))
    {
        m_Player.Velocity.x += 4.05f;
    }

    if (Ambient::Input::IsKeyPressed(AM_KEY_UP))
    {
        m_Player.Velocity.y += 4.05f;
    }

    else if (Ambient::Input::IsKeyPressed(AM_KEY_DOWN))
    {
        m_Player.Velocity.y -= 4.05f;
    }

    if (Ambient::Input::IsKeyPressed(AM_KEY_SPACE) && m_Player.Velocity.y == 0.0f)
    {
        m_Player.Velocity.y = 15.0f;
    }

    // Clamp Player Velocities
    m_Player.Velocity.x = std::min(m_Player.Velocity.x, 10.0f);
    m_Player.Velocity.x = std::max(m_Player.Velocity.x, -10.0f);
    m_Player.Velocity.y = std::min(m_Player.Velocity.y, 100.0f);
    m_Player.Velocity.y = std::max(m_Player.Velocity.y, -100.0f);

    // Apply drag
    m_Player.Velocity.x += -10.0f * m_Player.Velocity.x * ts;
    if (fabs(m_Player.Velocity.x) < 0.01f)
        m_Player.Velocity.x = 0.0f;

    // Gravity
    m_Player.Velocity.y -= 20.0f * ts;

    DetectCollisions();

    m_Player.Position.x += m_Player.Velocity.x * ts;
    m_Player.Position.y += m_Player.Velocity.y * ts;

    // Determine pose to draw

    if (abs(m_Player.Velocity.x) >= 4.0f && m_Player.IsOnGround)
    {
        if (m_Player.PreviousWalkPoseSetAt > 0.15)
        {
            m_Player.PreviousWalkPoseSetAt = 0;

            if (m_Player.PreviousWalkPose == '2')
            {
                m_Player.m_ActiveTexture = m_TextureMap['3'];
                m_Player.PreviousWalkPose = '3';
            }
            else
            {
                m_Player.m_ActiveTexture = m_TextureMap['2'];
                m_Player.PreviousWalkPose = '2';
            }
        }
        else
        {
            m_Player.PreviousWalkPoseSetAt += ts;
        }
    }

    else if (!m_Player.IsOnGround)
    {
        m_Player.m_ActiveTexture = m_TextureMap['4'];
    }

    else
    {
        m_Player.m_ActiveTexture = m_TextureMap['1'];
    }

    Ambient::Renderer2D::DrawQuad(m_Player.Position, { 1.5, 1.5 }, m_Player.m_ActiveTexture);
    m_CameraController.GetCamera().SetPosition({ m_Player.Position.x, 0.0f, 0.0f });

    Ambient::Renderer2D::EndScene();
}

void PlatformGame2D::RenderTiles()
{
    for (uint32_t y = 0; y < m_Level->Height; y++)
    {
        for (uint32_t x = 0; x < m_Level->Width; x++)
        {
            auto tile_id = m_Level->GetTile(x, y);

            if (m_TextureMap.find(tile_id) == m_TextureMap.end())
            {
                AM_ERROR("Cannot find the texture with key {}", tile_id);
                break;
            }

            Ambient::Renderer2D::DrawQuad(
                    { x - m_Level->Width / 2.0f, m_Level->Height - y - m_Level->Height / 2.0f },
                    { 1.0f, 1.0f },
                    m_TextureMap[tile_id]
            );
        }
    }
}

void PlatformGame2D::DetectCollisions()
{
    // Player position in world coordinate system
    glm::vec2 position = {
            m_Player.Position.x + m_Level->Width / 2.0f,
            m_Level->Height - m_Player.Position.y - m_Level->Height / 2.0f
    };

    m_Player.IsOnGround = false;

    if (m_Player.Velocity.x <= 0)
    {
        if (m_Level->GetTile(position.x + 0.0f, position.y + 0.0f) != '.' ||
            m_Level->GetTile(position.x + 0.0f, position.y + 0.9f) != '.')
        {
            m_Player.Position.x = m_Player.Position.x;
            m_Player.Velocity.x = 0;
        }
    }
    else  // moving right
    {
        if (m_Level->GetTile(position.x + 1.0f, position.y + 0.0f) != '.' ||
            m_Level->GetTile(position.x + 1.0f, position.y + 0.9f) != '.')
        {
            m_Player.Position.x = m_Player.Position.x;
            m_Player.Velocity.x = 0;
        }
    }

    if (m_Player.Velocity.y <= 0)
    {
        if (m_Level->GetTile(position.x + 0.0f, position.y + 1.0f) != '.' ||
            m_Level->GetTile(position.x + 0.9f, position.y + 1.0f) != '.')
        {
            m_Player.Position.y = ceil(m_Player.Position.y);
            m_Player.Velocity.y = 0;
            m_Player.IsOnGround = true;
        }
    }
    else // moving up
    {
        if (m_Level->GetTile(position.x + 0.0f, position.y + 0.0f) != '.' ||
            m_Level->GetTile(position.x + 0.9f, position.y + 0.0f) != '.')
        {
            m_Player.Position.y = floor(m_Player.Position.y);
            m_Player.Velocity.y = 0;
        }
    }
}

void PlatformGame2D::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}

