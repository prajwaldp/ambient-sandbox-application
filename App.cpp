#include "Ambient.h"


#include "demos/Platformer2D.h"
#include "demos/RotatingQuad.h"
#include "demos/RPGWorld.h"
#include "demos/SimpleSquareAndTriangle.h"



#include "EntryPoint.h"

class SandboxApp : public Ambient::Application
{
public:
    SandboxApp()
    {
        // PushLayer(new Platformer2D());
        // PushLayer(new RotatingQuad());
        PushLayer(new RPGWorld());
        // PushLayer(new SimpleSquareAndTriangle());
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
