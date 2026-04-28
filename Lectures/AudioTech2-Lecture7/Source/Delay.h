/*
  ==============================================================================

    Delay.h
    Created: 20 Apr 2026 4:21:24pm
    Author:  Jocelyn

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

    void setWetMix(float wetAmount);


private:

    juce::AudioBuffer<float> delayBuffer;

    std::vector<int> writeHeads;

    int delaySamples = 0;
    int maxDelayInSamples = 0;
    int delayBufferSize = 0;

    double sampleRate;
    float mix = 0.5;
   
};