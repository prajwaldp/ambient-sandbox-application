//
// Created by Prajwal Prakash on 23/06/20.
//

#ifndef SANDBOX_SIMPLESQUAREANDTRIANGLE_H
#define SANDBOX_SIMPLESQUAREANDTRIANGLE_H

#include "Ambient.h"
#include "glm/gtc/matrix_transform.hpp"

class SimpleSquareAndTriangle : public Ambient::Layer
{
public:
    SimpleSquareAndTriangle();

    void OnUpdate(Ambient::TimeStep ts) override;

    void OnEvent(Ambient::Event& e) override;

private:
    // Ambient::ShaderLibrary m_ShaderLibrary;
    Ambient::Ref<Ambient::Shader> m_FlatColor1Shader, m_FlatColor2Shader, m_TextureShader;
    Ambient::Ref<Ambient::Texture2D> m_Texture;

    Ambient::Ref<Ambient::VertexArray> m_TriangleVertexArray;
    Ambient::Ref<Ambient::VertexArray> m_SquareVertexArray;

    glm::vec3 m_TrianglePosition;
    float m_TriangleSpeed = 3.0f;

    Ambient::OrthographicCameraController m_CameraController;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 0.0f;

    float m_CameraSpeed = 5.0f;          // 5 units per second
    float m_CameraRotationSpeed = 90.0f; // 90 degrees per second
};

#endif //SANDBOX_SIMPLESQUAREANDTRIANGLE_H
