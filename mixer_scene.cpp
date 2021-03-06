#include "mixer_scene.hpp"

#include <SDL2/SDL_mixer.h>
#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <magic_enum.hpp>
#include <vector>

#include "io_util.hpp"

void MixerScene::Init() {
  fileBrowser.SetTitle("Open file");
  fileBrowser.SetTypeFilters(Music::SupportedFormats());

  Mix_Init(0);
  Mix_OpenAudio(SamplingRate, Format, Channels, BufferFrameCount);

  Mix_HookMusic(
      [](void *udata, Uint8 *stream, int len) {
        auto *scene = reinterpret_cast<MixerScene *>(udata);
        SampleBuffer buffer(len / sizeof(Frame));
        scene->FillBuffer(buffer);

        std::copy_n((Uint8 *)buffer.SampleData(), buffer.ByteSize(), stream);
      },
      this);

  Mix_PauseMusic();
}

void MixerScene::CleanUp() {
  Mix_CloseAudio();
  Mix_Quit();
}

void MixerScene::DoFrame(SDL_Event &event) {}

void MixerScene::DoUI() {
  ImGui::Begin("SDL2_mixer");
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

  if (ImGui::Button("Rewind")) {
    music.current = 0;
  }

  ImGui::ProgressBar((float)music.current / music.loopEnd);

  fileBrowser.Display();
  if (fileBrowser.HasSelected()) {

    currentPath = fileBrowser.GetSelected();
    fileBrowser.ClearSelected();

    try {
      music = std::move(Music::Open(currentPath));
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

void MixerScene::Play() { isPlaying = true; }

void MixerScene::Pause() { isPlaying = false; }

void MixerScene::FillBuffer(SampleBuffer &buffer) {
  if (isPlaying) {
    music.FillBuffer(buffer);
  } else {
    std::fill_n(buffer.FrameData(), buffer.FrameCount(), 0);
  }
}

std::unique_ptr<Scene> MixerScene::GetNextScene() { return nullptr; }
