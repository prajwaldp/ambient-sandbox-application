//
// Created by Prajwal Prakash on 24/06/20.
//

#ifndef SANDBOX_PARTICLESYSTEM_H
#define SANDBOX_PARTICLESYSTEM_H

#include "Ambient.h"

#include <random>


struct ParticleProperties
{
    glm::vec2 Position;
    glm::vec2 Velocity, VelocityVariation;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float Lifetime = 1.0f;
};


class Random
{
public:
    static void Init()
    {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }

private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};


class ParticleSystem : public Ambient::Layer
{
public:
    ParticleSystem();

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Ambient::Timestep ts) override;

    void OnEvent(Ambient::Event& e) override;

    void Emit(const ParticleProperties& properties);


private:
    struct Particle
    {
        glm::vec2 Position, Velocity;
        glm::vec4 ColorBegin, ColorEnd;

        float Rotation = 0.0f;
        float SizeBegin, SizeEnd;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;

        bool Active = false;
    };

    const static uint32_t PARTICLE_COUNT = 1000;

    Ambient::OrthographicCameraController m_CameraController;

    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex = 999;

    ParticleProperties m_ParticleProperties;
};


#endif //SANDBOX_PARTICLESYSTEM_H
