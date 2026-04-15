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
class InClassapr14AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    InClassapr14AudioProcessorEditor (InClassapr14AudioProcessor&);
    ~InClassapr14AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    InClassapr14AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InClassapr14AudioProcessorEditor)
};
