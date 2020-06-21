#include "Ambient.h"
#include "layers/SquareAndTriangle.h"

#include "EntryPoint.h"

class SandboxApp : public Ambient::Application
{
  public:
    SandboxApp()
    {
        PushLayer(new SquareAndTriangleLayer());
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
