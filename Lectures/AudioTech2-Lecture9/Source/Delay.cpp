/*
  ==============================================================================

    Delay.cpp
    Created: 22 Apr 2026 3:54:11pm
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

    // sets smoothing changes to linear smoothing in 10ms
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

    if (phase >= juce::MathConstants<float>::twoPi) {
        phase -= juce::MathConstants<float>::twoPi;
    }
}

float Delay::interpRead(float* delayData, int writeHead, float delaySamples)
{
    float readTail = std::fmod((writeHead - delaySamples + delayBufferSize), delayBufferSize);
    if (readTail < 0)
    {
        readTail += delayBufferSize;
    }

    int before = floor(readTail);
    int after = (before + 1) % delayBufferSize;

    float frac = readTail - before;

    return (delayData[before] * (1 - frac)) + (delayData[after] * frac);
}

float Delay::processSample(float inputSample, int channel)
{
    float* delayData = delayBuffer.getWritePointer(channel);

    int writeHead = writeHeads[channel];

    if (channel == 0)
    {
        nextLfoVal();
        currDelay = smoothDelay.getNextValue();
    }

    float modDelay = currDelay + lfo;

    modDelay = std::clamp<float>(modDelay, 0.001f, (delayBufferSize / sampleRate));

    float delaySamples = modDelay * sampleRate;

    float delayed = interpRead(delayData, writeHead, delaySamples);

    //delayData[writeHead] = inputSample;

    DBG(delayed);

    // with feedback
    delayData[writeHead] = inputSample + (feedback * delayed);

    // update output as mixed signal

    delayed = ((1.0 - mix) * inputSample) + (mix * delayed);

    writeHead = (writeHead + 1) % delayBufferSize;

    writeHeads[channel] = writeHead;

    return delayed;

}