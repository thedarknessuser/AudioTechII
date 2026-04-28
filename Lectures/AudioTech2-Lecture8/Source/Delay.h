/*
  ==============================================================================

    Delay.h
    Created: 22 Apr 2026 3:54:11pm
    Author:  JOcelyn

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

    void setDelayTime(float delayInSeconds);

    void setWetMix(float wetAmount);


private:

    juce::AudioBuffer<float> delayBuffer;

    std::vector<int> writeHeads;
    float interpRead(float* delayData, int writeHead, float delaySamples);

    int delaySamples = 0;
    int maxDelayInSamples = 48000;
    int delayBufferSize = 0;

    juce::SmoothedValue<float> smoothDelay;
    float currDelay;

    double sampleRate = 48000;
    float mix = 0.5;

    float feedback = 0;

    void nextLfoVal();

    float phase = 0;
    float freq = 0.5;
    float amp = 0.1;
    float lfo = 0;

};