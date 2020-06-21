#include "Ambient.h"
// #include "layers/SquareAndTriangle.h"
#include "layers/SandboxApp2D.h"

#include "EntryPoint.h"

class SandboxApp : public Ambient::Application
{
public:
    SandboxApp()
    {
        // PushLayer(new SquareAndTriangleLayer());
        PushLayer(new SandboxApp2D());
    }

    ~SandboxApp()
    {
    }
};

Ambient::Application* Ambient::CreateApplication()
{
    auto app = new SandboxApp();
    return app;
}
