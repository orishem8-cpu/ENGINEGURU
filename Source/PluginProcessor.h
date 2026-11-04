#pragma once
#include <JuceHeader.h>
#include "Dsp/DrumSynth.h"
#include "Dsp/SampleAnalyzer.h"
class ENGINEGURAudioProcessor final:public juce::AudioProcessor{
public:
 ENGINEGURAudioProcessor(); ~ENGINEGURAudioProcessor() override=default;
 void prepareToPlay(double,int)override; void releaseResources()override{}; bool isBusesLayoutSupported(const BusesLayout&)const override;
 void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&)override; juce::AudioProcessorEditor* createEditor()override; bool hasEditor()const override{return true;}
 const juce::String getName()const override{return JucePlugin_Name;} bool acceptsMidi()const override{return false;} bool producesMidi()const override{return false;} bool isMidiEffect()const override{return false;} double getTailLengthSeconds()const override{return 0.0;}
 int getNumPrograms()override{return 1;} int getCurrentProgram()override{return 0;} void setCurrentProgram(int)override{} const juce::String getProgramName(int)override{return{};} void changeProgramName(int,const juce::String&)override{}
 void getStateInformation(juce::MemoryBlock&)override; void setStateInformation(const void*,int)override;
 bool loadReference(const juce::File&); void generateVariations(); bool exportVariation(int,const juce::File&); int getVariationCount()const{return int(variations.size());}
 juce::String getReferenceName()const{return referenceName;} void setType(eg::SoundType t){type=t;} juce::AudioProcessorValueTreeState parameters;
private: double sampleRate=48000.0; eg::Analysis analysis; eg::SoundType type=eg::SoundType::Auto; juce::String referenceName="No sample loaded"; std::vector<juce::AudioBuffer<float>> variations; juce::AudioBuffer<float> reference;
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ENGINEGURAudioProcessor)
};
