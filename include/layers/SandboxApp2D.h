#pragma once

#include "Ambient.h"
#include "glm/gtc/matrix_transform.hpp"

class SandboxApp2D : public Ambient::Layer
{
public:
    SandboxApp2D();

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Ambient::Timestep ts) override;

    void OnEvent(Ambient::Event& e) override;

private:
    Ambient::OrthographicCameraController m_CameraController;
    Ambient::Ref<Ambient::VertexArray> m_SquareVertexArray;
    Ambient::Ref<Ambient::Shader> m_FlatColorShader;
    Ambient::Ref<Ambient::Texture2D> m_Texture;
    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
