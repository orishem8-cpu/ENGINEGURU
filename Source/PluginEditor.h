#pragma once

#include <JuceHeader.h>
#include <memory>
#include "PluginProcessor.h"

class ENGINEGURAudioProcessorEditor final
    : public juce::AudioProcessorEditor,
      private juce::Button::Listener
{
public:
    explicit ENGINEGURAudioProcessorEditor(ENGINEGURAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void buttonClicked(juce::Button*) override;

    ENGINEGURAudioProcessor& processor;

    juce::TextButton loadButton{"LOAD SAMPLE"};
    juce::TextButton generateButton{"GENERATE 16"};
    juce::TextButton exportButton{"EXPORT SELECTED"};

    juce::ComboBox typeBox;
    juce::Slider similarity, amount;
    juce::Label title, subtitle, status;

    int selected = 0;

    // Храним FileChooser до завершения асинхронного выбора файла.
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        ENGINEGURAudioProcessorEditor)
};
