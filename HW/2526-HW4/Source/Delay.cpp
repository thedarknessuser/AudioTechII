/*
  ==============================================================================

    Delay.cpp
 
    This code contains the implementation needed for a simple feedback delay.

  ==============================================================================
*/

#include "Delay.h"


void Delay::prepare(double samplingRate, int maxDelay, int numChannels)
{
    sampleRate = samplingRate;
    delayBufferSize = maxDelay;

    delayBuffer.setSize(numChannels, delayBufferSize);
    delayBuffer.clear();

    // TODO: smoothing stuff? Will proably want to extend to other apvts parameters
    smoothedDelay.reset(sampleRate, 0.01);

    writeHeads.resize(numChannels);
    for (int i = 0; i < numChannels; i++)
    {
        writeHeads[i] = 0;
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

void Delay::setDelayTime(float delaySeconds)
{
    //DBG("delay value in setter is: " << delaySeconds);
    smoothedDelay.setTargetValue(delaySeconds);
}

void Delay::setWetMix(float wetAmount)
{
    smoothedMix.setTargetValue(wetAmount);
}

void Delay::setFeedbackAmt(float feedbackAmt)
{
    smoothedFeedback.setTargetValue(feedbackAmt);
}

// this is called in the ProcessBlock as we iterate over each channel's buffer
float Delay::processSample(float inputSample, int channel)
{
    float* delayData = delayBuffer.getWritePointer(channel);
    int writeHead = writeHeads[channel];
    // nextLfoVal(); Not needed for this assignment but will be useful for final
    float modDelay = smoothedDelay.getNextValue(); // we can ommit the + lfo;
    float mixValue = smoothedMix.getNextValue();
    float feedbackValue = smoothedFeedback.getNextValue();
    //DBG("smoothed delay value is: " << modDelay);
    delaySamples = modDelay * sampleRate;
    int readTail = (writeHead - delaySamples + delayBufferSize) % delayBufferSize;
    float delayed = delayData[readTail];
    delayData[writeHead] = inputSample + delayed * feedbackValue; // Delayed sample is what we are returning now. So we just up our write buffer (what we will get later) with our delayed
    writeHead = (writeHead + 1) % delayBufferSize;
    writeHeads[channel] = writeHead;
    return (delayed * mixValue) + (inputSample * (1-mixValue)); // This could proably be improved using the quadratic easing formula learned in Audio Tech 1. Final project goal mayhaps.
}
