/*
  ==============================================================================

    Delay.cpp
    Created: 20 Apr 2026 4:21:24pm
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
void Delay::setDelayLength(float delayInSamples)
{
    delaySamples = delayInSamples;
}

void Delay::setWetMix(float wetAmount)
{
    mix = wetAmount;
}

float Delay::processSample(float inputSample, int channel) 
{
    float* delayData = delayBuffer.getWritePointer(channel);

    int writeHead = writeHeads[channel];

    int readTail = (writeHead - delaySamples + delayBufferSize) % delayBufferSize;

    //float delayed = (delayData[readTail] * mix) + (inputSample * (1 - mix));\

    float delayed = delayData[readTail];

    delayData[writeHead] = inputSample;

    writeHead = (writeHead + 1) % delayBufferSize;

    writeHeads[channel] = writeHead;

    return delayed;

}