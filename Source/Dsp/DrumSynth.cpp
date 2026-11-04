#include "DrumSynth.h"
#include <cmath>
namespace eg {
float DrumSynth::noise(uint32_t& s) noexcept { s^=s<<13; s^=s>>17; s^=s<<5; return float(s&0x00ffffffu)/8388607.5f-1.0f; }
void DrumSynth::render(juce::AudioBuffer<float>& out,double sr,SoundType type,uint32_t seed,float similarity,float amount,float duration){
 const int n=out.getNumSamples(); out.clear(); if(n<=0||sr<=0.0)return; if(type==SoundType::Auto)type=SoundType::Kick;
 const float chaos=juce::jlimit(0.0f,1.0f,amount), match=juce::jlimit(0.0f,1.0f,similarity);
 const float seconds=juce::jmin(juce::jlimit(0.045f,1.8f,duration>0?duration:0.25f),float(n/sr));
 const float variation=(noise(seed)*0.5f+0.5f)*chaos, base=42.0f+variation*(1.0f-match)*16.0f;
 for(int i=0;i<n;++i){const float t=float(i/sr); if(t>=seconds)continue; const float x=t/seconds; float y=0.0f, env=0.0f;
  switch(type){
   case SoundType::Bass808:{env=std::exp(-t*(2.2f+variation)); const float f=base+1.8f*base*std::exp(-t*24.0f); const float phase=juce::MathConstants<float>::twoPi*(base*t+(f-base)*(1.0f-std::exp(-t*24.0f))/24.0f); y=std::tanh((std::sin(phase)+0.12f*std::sin(2*phase))*(1+2*variation))*env; break;}
   case SoundType::Kick:{env=std::exp(-t*(12+8*variation)); const float f=48+145*std::exp(-t*32); const float phase=juce::MathConstants<float>::twoPi*(48*t+145*(1-std::exp(-t*32))/32); y=std::tanh(1.35f*std::sin(phase))*env+0.06f*std::exp(-t*105)*std::sin(juce::MathConstants<float>::twoPi*1800*t); (void)f; break;}
   case SoundType::Snare: case SoundType::Clap:{const float decay=type==SoundType::Clap?18.0f:25.0f; env=std::exp(-t*decay); const float tone=0.28f*std::sin(juce::MathConstants<float>::twoPi*(185+35*variation)*t)*env; const float noiseEnv=type==SoundType::Clap?(std::exp(-t*45)+((t>0.035f)?0.45f*std::exp(-(t-0.035f)*55):0.0f)):env; y=tone+noise(seed)*noiseEnv*(type==SoundType::Clap?0.16f:0.20f); break;}
   case SoundType::ClosedHat: case SoundType::OpenHat:{env=std::exp(-t*(type==SoundType::ClosedHat?65.0f:13.0f)); const float nse=noise(seed); const float metal=0.35f*std::sin(juce::MathConstants<float>::twoPi*6200*t)+0.15f*std::sin(juce::MathConstants<float>::twoPi*8300*t); y=(0.08f*nse+0.08f*metal)*env; break;}
   case SoundType::Percussion: case SoundType::Other: case SoundType::Auto:{env=std::exp(-t*22); y=(0.55f*std::sin(juce::MathConstants<float>::twoPi*(240+180*variation)*t)+0.05f*noise(seed))*env; break;}
  }
  const float fade=juce::jmin(1.0f,x*90)*juce::jmin(1.0f,(1-x)*35); const float sample=juce::jlimit(-0.95f,0.95f,y*0.78f*fade);
  for(int c=0;c<out.getNumChannels();++c)out.setSample(c,i,sample);
 }
}
}
