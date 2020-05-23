#include "application.h"

class SandboxApp : public Ambient::Application {
 public:
  SandboxApp() {}
  ~SandboxApp() {}
};

int main() {
  SandboxApp* my_app = new SandboxApp();
  my_app->Run();
  delete my_app;
  return 0;
}
