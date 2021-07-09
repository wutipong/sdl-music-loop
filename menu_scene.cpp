#include "menu_scene.hpp"
#include "audio_device_scene.hpp"
#include "mixer_scene.hpp"

void MenuScene::Init() {}
void MenuScene::CleanUp() {}
void MenuScene::DoFrame(SDL_Event &event) {}

void MenuScene::DoUI() {
  ImGui::Begin("Menu");
  
  if (ImGui::Button("Demo - SDL2 Audio Device")) {
    next = new AudioDeviceScene;
  }
  if (ImGui::Button("Demo - SDL2-Mixer")) {
    next = new MixerScene;
  }

  ImGui::End();
}