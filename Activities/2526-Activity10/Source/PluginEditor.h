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
class _2526Activity10AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _2526Activity10AudioProcessorEditor (_2526Activity10AudioProcessor&);
    ~_2526Activity10AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _2526Activity10AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_2526Activity10AudioProcessorEditor)
};
