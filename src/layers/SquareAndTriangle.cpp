#include "layers/SquareAndTriangle.h"

SquareAndTriangleLayer::SquareAndTriangleLayer() : Layer("My Layer"), m_CameraController(1280.0f / 720.0f)
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
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    // clang-format on

    m_TriangleVertexArray = Ambient::VertexArray::Create();

    Ambient::Ref<Ambient::VertexBuffer> triangleVertexBuffer;
    triangleVertexBuffer = Ambient::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));

    Ambient::BufferLayout layout = {{ Ambient::ShaderDataType::Float3, "a_Position" },
                                    { Ambient::ShaderDataType::Float4, "a_Color" }};
    triangleVertexBuffer->SetLayout(layout);
    m_TriangleVertexArray->AddVertexBuffer(triangleVertexBuffer);

    uint32_t indices[3] = { 0, 1, 2 };
    Ambient::Ref<Ambient::IndexBuffer> triangleIndexBuffer;
    triangleIndexBuffer = Ambient::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

    /**
     * Draw a Square
     **/

    // clang-format off
    float squareVertices[5 * 4] = {
            -0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
            0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
            0.75f, 0.75f, 0.0f, 1.0f, 1.0f,
            -0.75f, 0.75f, 0.0f, 0.0f, 1.0f,
    };
    // clang-format on

    m_SquareVertexArray = Ambient::VertexArray::Create();

    Ambient::Ref<Ambient::VertexBuffer> squareVertexBuffer;
    squareVertexBuffer = Ambient::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

    Ambient::BufferLayout squareLayout = {{ Ambient::ShaderDataType::Float3, "a_Position" },
                                          { Ambient::ShaderDataType::Float2, "a_TexCoord" }};
    squareVertexBuffer->SetLayout(squareLayout);
    m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Ambient::Ref<Ambient::IndexBuffer> squareIndexBuffer;
    squareIndexBuffer = Ambient::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

    m_FlatColor1Shader = Ambient::Shader::Create("./assets/shaders/FlatColor1.glsl");
    m_FlatColor2Shader = Ambient::Shader::Create("./assets/shaders/FlatColor2.glsl");
    m_TextureShader = Ambient::OpenGLShader::Create("./assets/shaders/Texture.glsl");

    // FlatColor1.glsl takes in no uniforms. Just Bind
    // This also works without binding. Because Renderer::Submit calls shader->Bind()
    // std::dynamic_pointer_cast<Ambient::OpenGLShader>(m_FlatColor1Shader)->Bind();

    // Texture.glsl takes in a uniform u_Texture
    // This also works without binding.
    // TODO Figure out when binding is necessary
    std::dynamic_pointer_cast<Ambient::OpenGLShader>(m_TextureShader)->Bind();
    std::dynamic_pointer_cast<Ambient::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

    std::string filepath = "./assets/batthern.png";
    m_Texture = Ambient::Texture2D::Create(filepath);
}

void SquareAndTriangleLayer::OnUpdate(Ambient::Timestep ts)
{
    float time = ts;

    m_CameraController.OnUpdate(ts);

    /**
     * Square Position Control
     **/

    if (Ambient::Input::IsKeyPressed(AM_KEY_LEFT))
    {
        m_TrianglePosition.x -= m_TriangleSpeed * time;
    }
    else if (Ambient::Input::IsKeyPressed(AM_KEY_RIGHT))
    {
        m_TrianglePosition.x += m_TriangleSpeed * time;
    }

    if (Ambient::Input::IsKeyPressed(AM_KEY_UP))
    {
        m_TrianglePosition.y += m_TriangleSpeed * time;
    }
    else if (Ambient::Input::IsKeyPressed(AM_KEY_DOWN))
    {
        m_TrianglePosition.y -= m_TriangleSpeed * time;
    }

    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    Ambient::Renderer::BeginScene(m_CameraController.GetCamera());

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));

    // glm::vec4 red(0.8f, 0.2f, 0.2f, 1.0f);
    // glm::vec4 blue(0.2f, 0.2f, 0.8f, 1.0f);
    glm::vec4 green(0.2f, 0.8f, 0.2f, 1.0f);

    std::dynamic_pointer_cast<Ambient::OpenGLShader>(m_FlatColor2Shader)->Bind();
    std::dynamic_pointer_cast<Ambient::OpenGLShader>(m_FlatColor2Shader)->UploadUniformFloat4("u_Color", green);

    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0), pos) * scale;
            Ambient::Renderer::Submit(m_FlatColor2Shader, m_SquareVertexArray, transform);
        }
    }

    // Triangle
    Ambient::Renderer::Submit(m_FlatColor1Shader, m_TriangleVertexArray,
            glm::translate(glm::mat4(1.0), m_TrianglePosition));
    Ambient::Renderer::EndScene();

    // Texture Square
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    m_Texture->Bind();
    Ambient::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::mat4(1.0) * scale);

    Ambient::Renderer::Flush();
}

void SquareAndTriangleLayer::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}
