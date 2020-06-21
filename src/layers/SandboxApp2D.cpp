#include "layers/SandboxApp2D.h"

SandboxApp2D::SandboxApp2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void SandboxApp2D::OnAttach()
{
}

void SandboxApp2D::OnDetach()
{
}

void SandboxApp2D::OnUpdate(Ambient::Timestep ts)
{
    float time = ts;
    m_CameraController.OnUpdate(ts);

    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    Ambient::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Ambient::Renderer2D::DrawQuad(
            { 0.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.8f, 0.2f, 0.3f, 1.0f }
    );

    Ambient::Renderer2D::EndScene();
}

void SandboxApp2D::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}
