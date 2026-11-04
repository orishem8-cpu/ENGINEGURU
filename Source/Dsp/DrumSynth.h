#pragma once
#include <JuceHeader.h>
#include <cstdint>
namespace eg {
enum class SoundType { Auto, Bass808, Kick, Snare, Clap, ClosedHat, OpenHat, Percussion, Other };
struct Analysis { SoundType suggested=SoundType::Other; float duration=0.25f, rms=0.0f, centroid=0.0f, lowRatio=0.0f, transient=0.0f, pitchDrop=0.0f; };
class DrumSynth { public: static void render(juce::AudioBuffer<float>&, double, SoundType, uint32_t, float, float, float); private: static float noise(uint32_t&) noexcept; };
}
