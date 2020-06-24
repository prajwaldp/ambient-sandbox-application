//
// Created by Prajwal Prakash on 23/06/20.
//

#ifndef SANDBOX_RPGWORLD_H
#define SANDBOX_RPGWORLD_H

#include "Ambient.h"
#include "glm/gtc/matrix_transform.hpp"

class RPGWorld : public Ambient::Layer
{
public:
    RPGWorld();

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Ambient::Timestep ts) override;

    void OnEvent(Ambient::Event& e) override;

private:
    Ambient::OrthographicCameraController m_CameraController;
    Ambient::Ref<Ambient::TextureAtlas2D> m_TextureAtlas;

    std::unordered_map<char, Ambient::Ref<Ambient::SubTexture2D>> m_TextureMap;
};

#endif //SANDBOX_RPGWORLD_H
