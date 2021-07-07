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
    QueueAudio(music);
  }
}

void Scene::DoUI() {
  ImGui::Begin("Control");
  ImGui::BeginGroup();
  if (ImGui::Button("Open File")) {
    fileBrowser.Open();
  }

  uint64_t loopStart = music.loopStart;
  uint64_t loopEnd = music.loopEnd;
  if (ImGui::InputScalar("Loop Start", ImGuiDataType_U64, &loopStart)) {
    if (loopStart < loopEnd && loopStart >= 0) {
      music.loopStart = loopStart;
    }
  }
  if (ImGui::InputScalar("Loop End", ImGuiDataType_U64, &loopEnd)) {
    if (loopEnd > loopStart && loopEnd <= music.Duration()) {
      music.loopEnd = loopEnd;
    }
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
      auto source = BufferedSource::OpenWAV(currentPath.string());
      music = Music(std::move(source));

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
