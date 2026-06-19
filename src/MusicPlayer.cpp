#include "MusicPlayer.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3_mixer/SDL_mixer.h>
#ifdef LUMEN_LOG
#  include <spdlog/spdlog.h>
#endif

namespace LumenCappuccino {

MusicPlayer::MusicPlayer()
    : valid(true), mixer(nullptr), backgroundTrack(nullptr),
      foregroundTrack(nullptr), loadingScreenAudio(nullptr),
      successAudio(nullptr), failureAudio(nullptr),
      coffeeMachineAudio(nullptr) {}

MusicPlayer::~MusicPlayer() {
  MIX_DestroyTrack(backgroundTrack);
  MIX_DestroyTrack(foregroundTrack);
  MIX_DestroyMixer(mixer);
  MIX_Quit();
}

bool MusicPlayer::setup() {
  if (!MIX_Init()) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::warn("SDL_mixer init failed: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
  if (!mixer) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::warn("Mixer creation failed: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  backgroundTrack = MIX_CreateTrack(mixer);
  if (!backgroundTrack) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::warn("Failed creating background track : {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  foregroundTrack = MIX_CreateTrack(mixer);
  if (!foregroundTrack) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::warn("Failed creating foreground track : {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!(loadAudio(LOADING_AUDIO_FILENAME, &loadingScreenAudio) &&
        loadAudio(COFFEE_MACHINE_AUDIO_FILENAME, &coffeeMachineAudio) &&
        loadAudio(SUCCESS_AUDIO_FILENAME, &successAudio) &&
        loadAudio(FAILURE_AUDIO_FILENAME, &failureAudio))) [[unlikely]] {
    return valid; // valid was already set false
  }
  // do some other things maybe
  return valid;
}

bool MusicPlayer::loadAudio(const char* fileName, MIX_Audio** destination) {
  *destination = MIX_LoadAudio(mixer, fileName, true);
  if (!destination || !(*destination)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::warn("Loading audio failed: {}", SDL_GetError());
#endif
    valid = false;
  }
  return valid;
}

bool MusicPlayer::isValid() const { return valid; }

bool MusicPlayer::playAudio(MIX_Track* track, MIX_Audio* audio) {
#ifdef LUMEN_STRICT_CHECKS
  if (!valid || !mixer || !track || !audio) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Invalid music player state {}", SDL_GetError());
#  endif
    return false;
  }
  if (!MIX_StopTrack(track, 0)) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Could not stop audio track before playing {}",
                  SDL_GetError());
#  endif
    return false;
  }
  if (!MIX_SetTrackAudio(track, audio)) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Could not set audio track {}", SDL_GetError());
#  endif
    return false;
  }
  if (!MIX_PlayTrack(track, 0)) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Could play audio track {}", SDL_GetError());
#  endif
    return false;
  }
  return true;
#else
  return MIX_SetTrackAudio(track, audio) && MIX_PlayTrack(track, 0);
#endif
}

bool MusicPlayer::playLoadingScreenAudio() {
  return playAudio(backgroundTrack, loadingScreenAudio);
}

bool MusicPlayer::playCoffeeMachineAudio() {
  return playAudio(backgroundTrack, coffeeMachineAudio);
}
bool MusicPlayer::playFailureSound() { return true; }
bool MusicPlayer::playSuccessSound() { return true; }

} // namespace LumenCappuccino
