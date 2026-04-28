/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class _2526Activtity11KeyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    _2526Activtity11KeyAudioProcessor();
    ~_2526Activtity11KeyAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_2526Activtity11KeyAudioProcessor)
    
    float samplingRate;
    int numSamplesPerBlock;
    
    float freq;
    float amp;
    float phase;
    
    // envelope length in seconds
    const float envSec = 1.0;
    
    // envelope length in samples;
    int envSamples;
    
    // envelope sample tracker;
    int envTracker;
    
    float lfoFreq;
    float lfoAmp;
    float lfoPhase;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    void genSineWave(juce::AudioBuffer<float>& buffer);
    void applyEnvRamp(juce::AudioBuffer<float>& buffer);
    void applyLFO(juce::AudioBuffer<float>& buffer);

};
