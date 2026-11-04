#include "SampleAnalyzer.h"
#include <cmath>
namespace eg {
Analysis SampleAnalyzer::analyze(const juce::AudioBuffer<float>& audio, double sr) {
 Analysis a; const int n=audio.getNumSamples(); if(n<=0||audio.getNumChannels()<=0||sr<=0.0) return a;
 a.duration=static_cast<float>(n/sr); double energy=0.0, crossings=0.0; float first=0.0f, late=0.0f;
 for(int i=0;i<n;++i){ float x=0.0f; for(int c=0;c<audio.getNumChannels();++c)x+=audio.getSample(c,i); x/=static_cast<float>(audio.getNumChannels()); energy+=double(x)*x;
  if(i<n/5)first=juce::jmax(first,std::abs(x)); if(i>n/2)late=juce::jmax(late,std::abs(x));
  if(i>0){const float p=audio.getSample(0,i-1); if(p*x<0.0f)crossings+=1.0; a.transient=juce::jmax(a.transient,juce::jlimit(0.0f,1.0f,std::abs(x)-std::abs(p)));}}
 a.rms=static_cast<float>(std::sqrt(energy/n)); a.centroid=static_cast<float>(crossings/n); a.pitchDrop=first>0?juce::jlimit(0.0f,1.0f,(first-late)/first):0.0f;
 if(a.duration>0.35f&&a.centroid>0.18f)a.suggested=SoundType::OpenHat;
 else if(a.centroid>0.20f&&a.duration<0.18f)a.suggested=SoundType::ClosedHat;
 else if(a.duration<0.22f&&a.transient>0.12f&&a.centroid>0.08f)a.suggested=SoundType::Snare;
 else if(a.duration<0.5f&&a.pitchDrop>0.45f)a.suggested=SoundType::Kick;
 else if(a.duration>=0.5f&&a.centroid<0.12f)a.suggested=SoundType::Bass808; else a.suggested=SoundType::Percussion;
 return a;
}
}
