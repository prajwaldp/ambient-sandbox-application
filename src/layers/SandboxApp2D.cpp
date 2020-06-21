#include "layers/SandboxApp2D.h"

SandboxApp2D::SandboxApp2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
    // clang-format off
    float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
    // clang-format on

    m_SquareVertexArray.reset(Ambient::VertexArray::Create());
    Ambient::Ref<Ambient::VertexBuffer> squareVertexBuffer;
    squareVertexBuffer.reset(Ambient::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    squareVertexBuffer->SetLayout({{Ambient::ShaderDataType::Float3, "a_Position"}});
    m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    Ambient::Ref<Ambient::IndexBuffer> squareIndexBuffer;
    squareIndexBuffer.reset(Ambient::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

    m_FlatColorShader = Ambient::Shader::Create("assets/shaders/FlatColor3.glsl");
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

    Ambient::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Ambient::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Ambient::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    Ambient::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0), glm::vec3(1.5f)));
    Ambient::Renderer::EndScene();
    Ambient::Renderer::Flush();
}

void SandboxApp2D::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}
