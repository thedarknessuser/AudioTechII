/*
  ==============================================================================

    Delay.cpp
    Created: 23 Apr 2026 9:38:44am
    Author:  Jocelyn

  ==============================================================================
*/

#include "Delay.h"

void Delay::prepare(double samplingRate, int maxDelay, int numChannels)
{
    sampleRate = samplingRate;
    
    delayBufferSize = maxDelay;
    
    delayBuffer.setSize(numChannels, delayBufferSize);
    delayBuffer.clear();
    
    // set smoothing changes to linear smoothing in 10ms
    smoothDelay.reset(sampleRate, 0.01);
    
    writeHeads.resize(numChannels);
    for (int c = 0; c < numChannels; ++c)
    {
        writeHeads[c] = 0;
    }
}

void Delay::setMaxDelayInSamples(int maxDelay)
{
    maxDelayInSamples = maxDelay;
}

int Delay::getMaxDelayInSamples()
{
    return maxDelayInSamples;
}

void Delay::setDelayTime(float delayInSeconds)
{
    smoothDelay.setTargetValue(delayInSeconds);
}

void Delay::setWetMix(float wetAmount)
{
    mix = wetAmount;
}

void Delay::nextLfoVal()
{
    lfo = amp * sinf(phase);
    phase += juce::MathConstants<float>::twoPi * freq / sampleRate;
    
    if (phase >= juce::MathConstants<float>::twoPi)
    {
        phase -= juce::MathConstants<float>::twoPi;
    }
}

float Delay::processSample(float inputSample, int channel)
{
    float* delayData = delayBuffer.getWritePointer(channel);
    
    int writeHead = writeHeads[channel];
    
    nextLfoVal();
    
    float modDelay = smoothDelay.getNextValue() + lfo;
    
    delaySamples = modDelay * sampleRate;
    
    int readTail = (writeHead - delaySamples + delayBufferSize) % delayBufferSize;
    
    float delayed = delayData[readTail];
    
    delayData[writeHead] = inputSample;
    
    writeHead = (writeHead + 1) % delayBufferSize;
    
    writeHeads[channel] = writeHead;
    
    return delayed;
}
