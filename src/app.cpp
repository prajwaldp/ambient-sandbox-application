#include "ambient.h"

class SandboxApp : public Ambient::Application {
 public:
  SandboxApp() {}
  ~SandboxApp() {}
};

Ambient::Application* Ambient::CreateApplication() { return new SandboxApp; }
