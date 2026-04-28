/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudioTech2Lecture9AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AudioTech2Lecture9AudioProcessorEditor (AudioTech2Lecture9AudioProcessor&);
    ~AudioTech2Lecture9AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioTech2Lecture9AudioProcessor& audioProcessor;

    juce::GroupComponent delayGroup{ "delayGroup", "Delay" };
    juce::GroupComponent modGroup{ "modGroup",   "Modulation" };

    // sliders
    juce::Slider delaySlider, mixSlider, feedbackSlider;
    juce::Slider rateSlider, depthSlider;

    void styleSlider(juce::Slider& s);

    // button
    juce::ToggleButton modButton{ "Mod On" };

    // attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> delayAttach, mixAttach, feedbackAttach;
    std::unique_ptr<SliderAttachment> rateAttach, depthAttach;
    std::unique_ptr<ButtonAttachment> modAttach;

    // labels
    juce::Label delayLabel, mixLabel, feedbackLabel;
    juce::Label rateLabel, depthLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioTech2Lecture9AudioProcessorEditor)
};

namespace ColoursTheme
{
    const juce::Colour background = juce::Colour(0xff121212);
    const juce::Colour panel = juce::Colour(0xff1e1e1e);
    const juce::Colour accent = juce::Colour(0xff4dd0e1);
    const juce::Colour text = juce::Colours::white;
    const juce::Colour knob = juce::Colour(0xff2a2a2a);
}
