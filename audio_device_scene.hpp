#pragma once

#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <imgui.h>

#include "imgui-filebrowser/imfilebrowser.h"
#include "music.hpp"
#include "scene.hpp"

class AudioDeviceScene : public Scene {
public:
  virtual void Init() override;
  virtual void CleanUp() override;
  virtual void DoFrame(SDL_Event &event) override;
  virtual void DoUI() override;
  virtual std::unique_ptr<Scene> GetNextScene() override { return nullptr; };

private:
  ImGui::FileBrowser fileBrowser{};
  bool isPlaying{false};
  std::filesystem::path currentPath;
  std::string errorMessage;

  Music music;

  void Play();
  void Pause();
};
