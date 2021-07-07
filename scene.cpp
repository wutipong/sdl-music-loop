#include "scene.hpp"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <magic_enum.hpp>
#include <vector>

#include "io_util.hpp"

void Scene::Init() {
  fileBrowser.SetTitle("Open file");
  fileBrowser.SetTypeFilters({".wav"});
}

void Scene::CleanUp() {}

void Scene::DoFrame(SDL_Event &event) {
  if (IsAudioNeedToQueue()) {
    QueueAudio(source);
  }
}

void Scene::DoUI() {
  ImGui::Begin("Control");
  ImGui::BeginGroup();
  if (ImGui::Button("Open File")) {
    fileBrowser.Open();
  }

  if (isPlaying) {
    if (ImGui::Button("Pause")) {
      Pause();
    }
  } else {
    if (ImGui::Button("Play")) {
      Play();
    }
  }
  fileBrowser.Display();
  if (fileBrowser.HasSelected()) {

    currentPath = fileBrowser.GetSelected();
    fileBrowser.ClearSelected();

    try {
      auto src = WaveSource::Open(currentPath.string());
      source = src;
      Pause();
      ClearAudio();
    } catch (std::exception &err) {
      errorMessage = err.what();
      ImGui::OpenPopup("Error");
    }
  }
  ImGui::EndGroup();
  if (ImGui::CollapsingHeader("info")) {
    ImGui::LabelText("Filename", currentPath.filename().string().c_str());
  }

  if (ImGui::BeginPopupModal("Error")) {

    ImGui::Text("Error: %s", errorMessage.c_str());
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::End();
}

void Scene::Play() {
  isPlaying = true;
  PlayAudio();
}

void Scene::Pause() {
  isPlaying = false;
  PauseAudio();
}
