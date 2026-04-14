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
class AudioTech2Lecture2pluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AudioTech2Lecture2pluginAudioProcessorEditor (AudioTech2Lecture2pluginAudioProcessor&);
    ~AudioTech2Lecture2pluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioTech2Lecture2pluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioTech2Lecture2pluginAudioProcessorEditor)
};
