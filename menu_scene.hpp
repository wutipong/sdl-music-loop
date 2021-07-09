#pragma once

#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include "scene.hpp"

class MenuScene : public Scene {
public:
  virtual void Init() override;
  virtual void CleanUp() override;
  virtual void DoFrame(SDL_Event &event) override;
  virtual void DoUI() override;
  virtual std::unique_ptr<Scene> GetNextScene() { return std::unique_ptr<Scene>(next); }

private:
  Scene* next = nullptr;
};
