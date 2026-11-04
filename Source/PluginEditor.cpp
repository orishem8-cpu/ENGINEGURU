#include "PluginEditor.h"

ENGINEGURAudioProcessorEditor::ENGINEGURAudioProcessorEditor(
    ENGINEGURAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(1100, 650);

    title.setText("ENGINEGURU", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(28, juce::Font::bold)));
    title.setColour(
        juce::Label::textColourId,
        juce::Colour(0xff202b35));
    addAndMakeVisible(title);

    subtitle.setText(
        "SAMPLE-BASED SOUND DESIGN ENGINE",
        juce::dontSendNotification);
    subtitle.setColour(
        juce::Label::textColourId,
        juce::Colour(0xff74818a));
    addAndMakeVisible(subtitle);

    status.setText(
        processor.getReferenceName(),
        juce::dontSendNotification);
    status.setColour(
        juce::Label::textColourId,
        juce::Colour(0xff53636e));
    addAndMakeVisible(status);

    for (auto* b : { &loadButton, &generateButton, &exportButton })
    {
        b->addListener(this);
        addAndMakeVisible(b);
    }

    juce::StringArray names{
        "Auto",
        "Bass / 808",
        "Kick",
        "Snare",
        "Clap",
        "Closed Hat",
        "Open Hat",
        "Percussion",
        "Other"
    };

    for (int i = 0; i < names.size(); ++i)
        typeBox.addItem(names[i], i + 1);

    typeBox.setSelectedId(1);
    addAndMakeVisible(typeBox);

    similarity.setSliderStyle(
        juce::Slider::LinearHorizontal);
    similarity.setTextBoxStyle(
        juce::Slider::TextBoxRight, false, 60, 22);
    similarity.setRange(0, 1, 0.01);
    similarity.setValue(0.72);
    addAndMakeVisible(similarity);

    amount.setSliderStyle(
        juce::Slider::LinearHorizontal);
    amount.setTextBoxStyle(
        juce::Slider::TextBoxRight, false, 60, 22);
    amount.setRange(0, 1, 0.01);
    amount.setValue(0.22);
    addAndMakeVisible(amount);
}

void ENGINEGURAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xffedf1f3));

    auto panel = [&](juce::Rectangle<int> r,
                     const juce::String& name,
                     juce::Colour accent)
    {
        g.setColour(juce::Colours::white);
        g.fillRoundedRectangle(r.toFloat(), 14);

        g.setColour(juce::Colour(0x160d2735));
        g.drawRoundedRectangle(r.toFloat(), 14, 1);

        g.setColour(accent);
        g.fillRoundedRectangle(
            r.removeFromTop(5).toFloat(), 2);

        g.setColour(juce::Colour(0xff34434d));
        g.setFont(14);
        g.drawText(
            name,
            r.removeFromTop(34).reduced(14, 0),
            juce::Justification::centredLeft);
    };

    panel({24, 100, 250, 500},
          "01  INPUT", juce::Colour(0xff36b7b0));

    panel({290, 100, 250, 500},
          "02  ANALYZER", juce::Colour(0xffe3ad56));

    panel({556, 100, 250, 500},
          "03  GENERATOR", juce::Colour(0xff5997d5));

    panel({822, 100, 254, 500},
          "04  VARIATIONS", juce::Colour(0xff8b78ca));

    g.setColour(juce::Colour(0xff71808a));
    g.setFont(12);

    g.drawText(
        "REFERENCE  /  ANALYSIS  /  SYNTHESIS  /  EXPORT",
        28, 615, 500, 18,
        juce::Justification::left);

    g.drawText(
        "16 VARIATIONS  •  WAV 24-BIT",
        780, 615, 290, 18,
        juce::Justification::right);
}

void ENGINEGURAudioProcessorEditor::resized()
{
    title.setBounds(28, 18, 300, 42);
    subtitle.setBounds(30, 58, 350, 22);

    loadButton.setBounds(42, 160, 210, 38);
    status.setBounds(42, 208, 210, 48);

    typeBox.setBounds(572, 160, 218, 32);
    similarity.setBounds(572, 225, 218, 42);
    amount.setBounds(572, 292, 218, 42);

    generateButton.setBounds(572, 370, 218, 42);
    exportButton.setBounds(838, 535, 222, 38);
}

void ENGINEGURAudioProcessorEditor::buttonClicked(
    juce::Button* b)
{
    // LOAD SAMPLE
    if (b == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Choose WAV or AIFF sample",
            juce::File{},
            "*.wav;*.aif;*.aiff");

        juce::Component::SafePointer<
            ENGINEGURAudioProcessorEditor> safeThis(this);

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles,

            [safeThis](const juce::FileChooser& chooser)
            {
                if (safeThis == nullptr)
                    return;

                const auto file = chooser.getResult();

                if (file.existsAsFile())
                {
                    if (safeThis->processor.loadReference(file))
                    {
                        safeThis->status.setText(
                            safeThis->processor.getReferenceName(),
                            juce::dontSendNotification);
                    }
                    else
                    {
                        safeThis->status.setText(
                            "Could not read audio file",
                            juce::dontSendNotification);
                    }
                }

                safeThis->fileChooser.reset();
            });
    }

    // GENERATE
    else if (b == &generateButton)
    {
        processor.setType(
            static_cast<eg::SoundType>(
                typeBox.getSelectedId() - 1));

        processor.parameters
            .getParameter("similarity")
            ->setValueNotifyingHost(
                float(similarity.getValue()));

        processor.parameters
            .getParameter("amount")
            ->setValueNotifyingHost(
                float(amount.getValue()));

        processor.generateVariations();

        status.setText(
            "16 variations generated",
            juce::dontSendNotification);
    }

    // EXPORT SELECTED
    else if (b == &exportButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Export selected variation",
            juce::File::getSpecialLocation(
                juce::File::userMusicDirectory)
                .getChildFile("ENGINEGURU_01.wav"),
            "*.wav");

        juce::Component::SafePointer<
            ENGINEGURAudioProcessorEditor> safeThis(this);

        fileChooser->launchAsync(
            juce::FileBrowserComponent::saveMode |
            juce::FileBrowserComponent::canSelectFiles,

            [safeThis](const juce::FileChooser& chooser)
            {
                if (safeThis == nullptr)
                    return;

                const auto file = chooser.getResult();

                if (file != juce::File{}
                    && safeThis->processor.exportVariation(
                        safeThis->selected,
                        file.withFileExtension("wav")))
                {
                    safeThis->status.setText(
                        "Exported variation 01",
                        juce::dontSendNotification);
                }

                safeThis->fileChooser.reset();
            });
    }
}
