#pragma once

#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <imgui.h>

#include "imgui-filebrowser/imfilebrowser.h"
#include "wave_source.hpp"

class Scene {
public:
  void Init();
  void CleanUp();
  void DoFrame(SDL_Event &event);
  void DoUI();

private:
  ImGui::FileBrowser fileBrowser{};
  bool isPlaying{false};
  std::filesystem::path currentPath;
  std::string errorMessage;

  WaveSource source;

  void Play();
  void Pause();
};
