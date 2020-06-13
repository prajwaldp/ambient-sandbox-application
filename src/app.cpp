#include "ambient.h"

class MyLayer : public Ambient::Layer {
 public:
  MyLayer() : Layer("My Layer") {}

  void OnUpdate() override { AM_INFO("Called SampleLayer::OnUpdate"); }

  void OnEvent(Ambient::Event::Event& event) override {
    AM_TRACE("{0}", event.ToString());
  }
};

class SandboxApp : public Ambient::Application {
 public:
  SandboxApp() { PushLayer(new MyLayer()); }
  ~SandboxApp() {}
};

Ambient::Application* Ambient::CreateApplication() { return new SandboxApp; }
