#include "Ambient.h"

#include <iostream>

class MyLayer : public Ambient::Layer
{
  public:
    MyLayer() : Layer("My Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        /**
         * Draw a triangle
         * => Create a VertexArray
         * => Create a VertexBuffer and IndexBuffer
         * => Add the VertexBuffer and IndexBuffer to the VertexArray
         **/

        // clang-format off
        float triangleVertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        };
        // clang-format on

        m_VertexArray.reset(Ambient::VertexArray::Create());

        std::shared_ptr<Ambient::VertexBuffer> triangleVertexBuffer;
        triangleVertexBuffer.reset(Ambient::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));

        Ambient::BufferLayout layout = {{Ambient::ShaderDataType::Float3, "a_Position"},
                                        {Ambient::ShaderDataType::Float4, "a_Color"}};
        triangleVertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(triangleVertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Ambient::IndexBuffer> triangleIndexBuffer;
        triangleIndexBuffer.reset(Ambient::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(triangleIndexBuffer);

        /**
         * Draw a Square
         **/

        // clang-format off
        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };
        // clang-format on

        m_SquareVertexArray.reset(Ambient::VertexArray::Create());

        std::shared_ptr<Ambient::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(Ambient::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        Ambient::BufferLayout squareLayout = {{Ambient::ShaderDataType::Float3, "a_Position"}};
        squareVertexBuffer->SetLayout(squareLayout);
        m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Ambient::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(Ambient::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

        /*
        Shaders
        =======

        -> Vertex shader
            -> Runs on the GPU for every vertex (point in 3D space)
            -> Normalize vertices to the [-1, 1] space
            -> Runs 3 times for the traingle example

        -> Fragment shader
            -> Also called Pixel shader in other frameworks
            -> Runs per pixel
            -> Fills in the triangle
        */

        std::string vertexShaderSrc = R"(
            #version 410 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;

            void main() {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentShaderSrc = R"(
            #version 410 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main() {
                color = v_Color;
            }
        )";

        m_Shader.reset(new Ambient::Shader(vertexShaderSrc, fragmentShaderSrc));
        // instead of m_Shader = std::make_unique<Ambient::Shader>()

        std::string squareVertexShaderSrc = R"(
            #version 410 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;

            void main() {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string squareFragmentShaderSrc = R"(
            #version 410 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main() {
                color = vec4(0.2, 0.1, 0.98, 1.0);
            }
        )";

        m_SquareShader.reset(new Ambient::Shader(squareVertexShaderSrc, squareFragmentShaderSrc));
    }

    void OnUpdate() override
    {
        Ambient::RenderCommand::SetClearColor();
        Ambient::RenderCommand::Clear();

        m_Camera.SetPosition({0.5f, 0.5f, 0.0f});
        m_Camera.SetRotation(45.0f);

        Ambient::Renderer::BeginScene(m_Camera);
        Ambient::Renderer::Submit(m_SquareShader, m_SquareVertexArray);
        Ambient::Renderer::Submit(m_Shader, m_VertexArray);
        Ambient::Renderer::EndScene();

        Ambient::Renderer::Flush();
    }

    void OnEvent(Ambient::Event::Event &event) override
    {
        AM_TRACE("{0}", event.ToString());
    }

  private:
    std::shared_ptr<Ambient::Shader> m_Shader;
    std::shared_ptr<Ambient::VertexArray> m_VertexArray;

    std::shared_ptr<Ambient::Shader> m_SquareShader;
    std::shared_ptr<Ambient::VertexArray> m_SquareVertexArray;

    Ambient::OrthographicCamera m_Camera;
};

class SandboxApp : public Ambient::Application
{
  public:
    SandboxApp()
    {
        PushLayer(new MyLayer());
    }

    ~SandboxApp()
    {
    }
};

Ambient::Application *Ambient::CreateApplication()
{
    auto app = new SandboxApp();
    return app;
}
