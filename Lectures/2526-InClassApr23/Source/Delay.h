/*
  ==============================================================================

    Delay.h
    Created: 23 Apr 2026 9:38:44am
    Author:  Jocelyn

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class Delay
{
    public:
    void prepare(double samplingRate, int maxDelay, int numChannels);
    
    float processSample(float inputSample, int channel);
    
    void setMaxDelayInSamples(int maxDelay);
    
    int getMaxDelayInSamples();
    
    void setDelayTime(float delayInSeconds);
    
    void setWetMix(float wetAmount);
    
    private:
    
    juce::AudioBuffer<float> delayBuffer;
    
    std::vector<int> writeHeads;
    
    juce::SmoothedValue<float> smoothDelay;
    float currDelay;
    
    int delaySamples = 0;
    int maxDelayInSamples = 48000;
    int delayBufferSize = 0;
    
    double sampleRate = 48000;
    float mix = 0.5;
    
    float feedback = 0;
    
    void nextLfoVal();
    
    float phase = 0;
    float freq = 0.5;
    float amp = 0.1;
    float lfo = 0;
    
};
