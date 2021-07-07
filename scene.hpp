#pragma once

#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <imgui.h>

#include "buffered_source.hpp"
#include "imgui-filebrowser/imfilebrowser.h"

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

  BufferedSource source;

  void Play();
  void Pause();
};
