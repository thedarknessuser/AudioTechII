/*
  ==============================================================================

    Delay.h
 
 This code contains the implementation needed for a simple feedback delay.

  ==============================================================================
*/
#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class Delay
{

public:
    void prepare(double samplingRate, int maxDelayInSamples, int numChannels);

    float processSample(float inputSample, int channel);

    void setMaxDelayInSamples(int maxDelayInSamples);

    int getMaxDelayInSamples();

    void setDelayLength(float delayInSamples);

    void setWetMix(float wetAmt);
    
    void setDelayTime(float delaySeconds);
    
    void setFeedbackAmt(float feedbackAmt);


private:

    juce::AudioBuffer<float> delayBuffer;

    std::vector<int> writeHeads;

    int delaySamples = 0;
    float delaySeconds = 0;
    int maxDelayInSamples = 0;
    int delayBufferSize = 0;

    double sampleRate;
    float mix = 0.5;
    float feedback = 0.2;
    
    juce::SmoothedValue<float> smoothedDelay;
   
};
