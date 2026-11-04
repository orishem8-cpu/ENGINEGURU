# ENGINEGURU
JUCE-based VST3/Standalone sample-to-sound-design prototype for Windows and FL Studio.

## Included in this first foundation
- WAV/AIFF reference loading (up to 8 seconds)
- Bass/808, Kick, Snare, Clap, Closed Hat, Open Hat, Percussion, Other and Auto modes
- Deterministic 16-variation synthesis
- Similarity and Amount controls
- 24-bit WAV export
- Light modular interface foundation

## Status and limitations
This is an initial engineering prototype, not a finished commercial plugin. The analyzer currently uses simple time-domain heuristics, not a full FFT/pitch-envelope analysis. Sound models are starter DSP implementations and require iterative listening tests and tuning. In particular, low-end cleanliness, kick/snare identity, and hat harshness must be benchmarked against real reference samples. No professional sound-quality or successful-build claim is made yet.

## Windows build
1. Install Visual Studio 2022 with **Desktop development with C++** and Windows SDK.
2. Install CMake 3.22+ and Git.
3. In PowerShell, from this folder run:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release --target ENGINEGURU_VST3
```

JUCE 8.0.6 is fetched at configure time. Runtime operation is offline; first configuration needs internet unless JUCE is already cached. The VST3 bundle should appear in `build/ENGINEGURU_artefacts/Release/VST3`.

## Next milestones
1. FFT, pitch-envelope, transient and periodicity analysis with confidence scoring.
2. Dedicated refined models for each drum family; avoid broadband noise in 808 and tame hat resonances.
3. 16 visible audition pads, waveform previews, favorites, drag-to-DAW and Export All.
4. Automated QC for clipping, DC offset, clicks, aliasing and spectral outliers.
5. Build CI and listening tests with a reference-sample benchmark.
