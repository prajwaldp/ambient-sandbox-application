#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>


std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;


ParticleSystem::ParticleSystem() : Layer("PlatformGame2D"), m_CameraController(1280.0f / 720.0f)
{
    m_ParticlePool.resize(1000);

    m_ParticleProperties.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_ParticleProperties.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };

    m_ParticleProperties.SizeBegin = 0.5f;
    m_ParticleProperties.SizeVariation = 0.3f;
    m_ParticleProperties.SizeEnd = 0.0f;

    m_ParticleProperties.Lifetime = 1.0f;
    m_ParticleProperties.Velocity = { 0.0f, 0.0f };
    m_ParticleProperties.VelocityVariation = { 3.0f, 1.0f };
    m_ParticleProperties.Position = { 0.0f, 0.0f };

    for (int i = 0; i < ParticleSystem::PARTICLE_COUNT; i++)
    {
        Particle particle;
        m_ParticlePool.push_back(particle);
    }
}

void ParticleSystem::OnAttach()
{
    Particle& particle = m_ParticlePool[m_PoolIndex];
    particle.Active = true;
    particle.Position = m_ParticleProperties.Position;
    particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

    // Velocity
    particle.Velocity = m_ParticleProperties.Velocity;
    particle.Velocity.x += m_ParticleProperties.VelocityVariation.x * (Random::Float() - 0.5f);
    particle.Velocity.y += m_ParticleProperties.VelocityVariation.y * (Random::Float() - 0.5f);

    // Color
    particle.ColorBegin = m_ParticleProperties.ColorBegin;
    particle.ColorEnd = m_ParticleProperties.ColorEnd;

    particle.LifeTime = m_ParticleProperties.Lifetime;
    particle.LifeRemaining = m_ParticleProperties.Lifetime;
    particle.SizeBegin = m_ParticleProperties.SizeBegin + m_ParticleProperties.SizeVariation * (Random::Float() - 0.5f);
    particle.SizeEnd = m_ParticleProperties.SizeEnd;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();

    Emit(m_ParticleProperties);
}

void ParticleSystem::OnDetach()
{

}

void ParticleSystem::OnEvent(Ambient::Event& e)
{
}

void ParticleSystem::OnUpdate(Ambient::Timestep ts)
{
    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    Ambient::Renderer2D::ResetStats();
    Ambient::Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (auto& particle : m_ParticlePool)
    {
        if (!particle.Active)
            continue;

        if (particle.LifeRemaining <= 0.0f)
        {
            particle.Active = false;
            continue;
        }

        particle.LifeRemaining -= ts;
        particle.Position += particle.Velocity * (float)ts;
        particle.Rotation += 0.01f * ts;

        float life = particle.LifeRemaining / particle.LifeTime;
        glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

        float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

        Ambient::Renderer2D::DrawRotatedQuad(
                particle.Position,
                glm::vec2(size),
                particle.Rotation,
                color
        );
    }

    Ambient::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProperties& properties)
{
    Particle& particle = m_ParticlePool[m_PoolIndex];
    particle.Active = true;
    particle.Position = properties.Position;
    particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

    // Velocity
    particle.Velocity = properties.Velocity;
    particle.Velocity.x += properties.VelocityVariation.x * (Random::Float() - 0.5f);
    particle.Velocity.y += properties.VelocityVariation.y * (Random::Float() - 0.5f);

    // Color
    particle.ColorBegin = properties.ColorBegin;
    particle.ColorEnd = properties.ColorEnd;

    particle.LifeTime = properties.Lifetime;
    particle.LifeRemaining = properties.Lifetime;
    particle.SizeBegin =
            properties.SizeBegin + properties.SizeVariation * (Random::Float() - 0.5f);
    particle.SizeEnd = properties.SizeEnd;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
