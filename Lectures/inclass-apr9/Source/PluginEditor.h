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
class Inclassapr9AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Inclassapr9AudioProcessorEditor (Inclassapr9AudioProcessor&);
    ~Inclassapr9AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Inclassapr9AudioProcessor& audioProcessor;
    
    juce::Slider gainSlider;
    
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Inclassapr9AudioProcessorEditor)
};
