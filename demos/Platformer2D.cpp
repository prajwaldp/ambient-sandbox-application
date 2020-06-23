#include "Platformer2D.h"

Platformer2D::Platformer2D() : Layer("Platformer2D"), m_CameraController(1280.0f / 720.0f)
{
    m_Level = "................................................................"
              "................................................................"
              "................................................................"
              "................................................................"
              "................................................................"
              "..............................##................................"
              ".............................#.................................."
              "..........................###..........................##......."
              "......................####..........................###........."
              ".....................#............................##............"
              "######################################.#############............"
              "####################.................#.#####################...."
              "####################.................#.#####################...."
              "####################.......###########.#####################...."
              "####################.......#...........#####################...."
              "####################.......#################################....";


    m_LevelWidth = 64;
    m_LevelHeight = 16;
}

void Platformer2D::OnAttach()
{
}

void Platformer2D::OnDetach()
{
}

void Platformer2D::OnUpdate(Ambient::Timestep ts)
{
    auto GetTile = [&](int x, int y)
    {
        if (x >= 0 && x < m_LevelWidth && y >= 0 && y < m_LevelHeight)
            return m_Level[y * m_LevelWidth + x];
        else
            return ' ';
    };

    // float drag_force = m_IsPlayerOnGround ? 10.0f : 0.0f;

    if (Ambient::Input::IsKeyPressed(AM_KEY_LEFT))
        m_PlayerVelocityX = -15.0f;

    else if (Ambient::Input::IsKeyPressed(AM_KEY_RIGHT))
        m_PlayerVelocityX = 15.0f;

    else
        m_PlayerVelocityX= 0.0f;

    if (Ambient::Input::IsKeyPressed(AM_KEY_SPACE))
    {
        if (m_PlayerVelocityY == 0)
        {
            m_PlayerVelocityY = -15.0f;
        }
    }
    else
    {
        m_PlayerVelocityY = 0;
    }

    // Gravity
    m_PlayerVelocityY += 20.f * ts;

    // Drag
//    if (m_IsPlayerOnGround)
//    {
//        m_PlayerVelocityX += -3.0 * m_PlayerVelocityX * ts;
//        if (fabs(m_PlayerVelocityX) < 0.01f)
//            m_PlayerVelocityX = 0;
//    }

    // Clamp Velocities
    if (m_PlayerVelocityX > 10.0f)
        m_PlayerVelocityX = 10.0f;

    if (m_PlayerVelocityX < -10.0f)
        m_PlayerVelocityX = -10.0f;

    if (m_PlayerVelocityY > 100.0f)
        m_PlayerVelocityY = 100.0f;

    if (m_PlayerVelocityY < -100.0f)
        m_PlayerVelocityY = -100.0f;

    float NewPlayerPositionX = m_PlayerPositionX + m_PlayerVelocityX * ts;
    float NewPlayerPositionY = m_PlayerPositionY + m_PlayerVelocityY * ts;

    if (m_PlayerVelocityX <= 0) // Moving Left
    {
        if (GetTile(NewPlayerPositionX + 0.0f, m_PlayerPositionY + 0.0f) != L'.' ||
            GetTile(NewPlayerPositionX + 0.0f, m_PlayerPositionY + 0.9f) != L'.')
        {
            NewPlayerPositionX = (int)NewPlayerPositionX + 1;
            m_PlayerVelocityX = 0;
        }
    }
    else // Moving Right
    {
        if (GetTile(NewPlayerPositionX + 1.0f, m_PlayerPositionY + 0.0f) != L'.' ||
            GetTile(NewPlayerPositionX + 1.0f, m_PlayerPositionY + 0.9f) != L'.')
        {
            NewPlayerPositionX = (int)NewPlayerPositionX;
            m_PlayerVelocityX = 0;

        }
    }

    m_IsPlayerOnGround = false;

    if (m_PlayerVelocityY <= 0) // Moving Up
    {
        if (GetTile(NewPlayerPositionX + 0.0f, NewPlayerPositionY) != L'.' ||
            GetTile(NewPlayerPositionX + 0.9f, NewPlayerPositionY) != L'.')
        {
            NewPlayerPositionY = (int)NewPlayerPositionY + 1;
            m_PlayerVelocityY = 0;
        }
    }
    else // Moving Down
    {
        if (GetTile(NewPlayerPositionX + 0.0f, NewPlayerPositionY + 1.0f) != L'.' ||
            GetTile(NewPlayerPositionX + 0.9f, NewPlayerPositionY + 1.0f) != L'.')
        {
            NewPlayerPositionY = (int)NewPlayerPositionY;
            m_PlayerVelocityY = 0;
            m_IsPlayerOnGround = true;
        }
    }

    // Apply new position
    m_PlayerPositionX = NewPlayerPositionX;
    m_PlayerPositionY = NewPlayerPositionY;

    m_CameraController.GetCamera().SetPosition({ m_PlayerPositionX, m_LevelHeight - m_PlayerPositionY, 0.0f });

    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    Ambient::Renderer2D::ResetStats();
    Ambient::Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (int y = 0; y < m_LevelHeight; y++)
    {
        for (int x = 0; x < m_LevelWidth; x++)
        {
            auto tile_id = GetTile(x, y);

            switch (tile_id)
            {
            case '#':
                Ambient::Renderer2D::DrawQuad(
                        { x, m_LevelHeight - y },
                        { 1.0f, 1.0f },
                        { 0.7f, 0.2f, 0.3f, 1.0f });
                break;

            case '.':
                Ambient::Renderer2D::DrawQuad(
                        { x, m_LevelHeight - y },
                        { 1.0f, 1.0f },
                        { 0.1f, 0.2f, 0.9f, 1.0f });
                break;

            default:
                Ambient::Renderer2D::DrawQuad({ x, y }, { 1.0f, 1.0f }, glm::vec4(1.0f));
            }
        }
    }

    Ambient::Renderer2D::DrawQuad(
            { m_PlayerPositionX, m_LevelHeight - m_PlayerPositionY, 0.5 },
            { 1.0f, 1.0f },
            { 0.1f, 0.9f, 0.0f, 1.0f });

    Ambient::Renderer2D::EndScene();
}

void Platformer2D::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}

