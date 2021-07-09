#pragma once
#include <memory>

class Scene {
public:
  virtual void Init() = 0;
  virtual void CleanUp() = 0;
  virtual void DoFrame(SDL_Event &event) =0; 
  virtual void DoUI() =0;
  virtual std::unique_ptr<Scene> GetNextScene() = 0;
};
