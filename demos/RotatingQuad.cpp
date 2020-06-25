#include "RotatingQuad.h"

RotatingQuad::RotatingQuad() : Layer("RotatingQuad"), m_CameraController(1280.0f / 720.0f)
{
}

void RotatingQuad::OnAttach()
{
    m_Texture = Ambient::Texture2D::Create("./../../assets/textures/batthern.png");
}

void RotatingQuad::OnDetach()
{
}

void RotatingQuad::OnUpdate(Ambient::TimeStep ts)
{
    m_CameraController.OnUpdate(ts);

    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    static float rotation = 0.0f;
    rotation -= float(ts) * 180.0f;

    Ambient::Renderer2D::ResetStats();
    Ambient::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Ambient::Renderer2D::DrawQuad(
            { 0.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.2f, 0.8f, 0.3f, 1.0f }
    );

    Ambient::Renderer2D::DrawRotatedQuad(
            { 0.0f, 0.0f },
            { 1.0f, 1.0f },
            rotation,
            { 0.8f, 0.2f, 0.3f, 1.0f }
    );

    Ambient::Renderer2D::DrawRotatedQuad(
            { 0.0f, 0.0f, 0.5f },
            { 1.0f, 1.0f },
            rotation,
            m_Texture
    );

    Ambient::Renderer2D::EndScene();
}

void RotatingQuad::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}
