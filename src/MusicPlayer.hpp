#ifndef MUSIC_PLAYER__HPP
#define MUSIC_PLAYER__HPP
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string>

#define LOADING_AUDIO_FILENAME "patapim.mp3"
//#define LOADING_AUDIO_FILENAME "takefive.mp3"
#define COFFEE_MACHINE_AUDIO_FILENAME "coffee_machine_background_noise.mp3"
#define FAILURE_AUDIO_FILENAME "failure_sound.mp3"
#define SUCCESS_AUDIO_FILENAME "success_sound.mp3"

namespace LumenCappuccino{
class MusicPlayer{
private:
    bool valid;
    MIX_Mixer* mixer;
    MIX_Track* backgroundTrack;
    MIX_Track* foregroundTrack;
    MIX_Audio* loadingScreenAudio;
    MIX_Audio* successAudio;
    MIX_Audio* failureAudio;
    MIX_Audio* coffeeMachineAudio;
    bool playAudio(MIX_Track* track, MIX_Audio* audio);
    bool loadAudio(const char *fileName, MIX_Audio** destination);
public:
    MusicPlayer();

    /**
     *  call only once
     */
    bool setup();
    void startPlaying(const std::string& soundName);
    void stopAllPlaying(const std::string& soundName);
    bool playLoadingScreenAudio();
    bool playCoffeeMachineAudio();
    bool playFailureSound();
    bool playSuccessSound();
    bool isValid() const;
    ~MusicPlayer();
};

} // namespace LumenCappuccino


#endif
