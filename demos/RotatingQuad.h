//
// Created by Prajwal Prakash on 23/06/20.
//

#ifndef SANDBOX_ROTATINGQUAD_H
#define SANDBOX_ROTATINGQUAD_H

#include "Ambient.h"
#include "glm/gtc/matrix_transform.hpp"

class RotatingQuad : public Ambient::Layer
{
public:
    RotatingQuad();

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Ambient::Timestep ts) override;

    void OnEvent(Ambient::Event& e) override;

private:
    Ambient::OrthographicCameraController m_CameraController;
    Ambient::Ref<Ambient::Texture2D> m_Texture;
};

#endif // SANDBOX_ROTATINGQUAD_H
