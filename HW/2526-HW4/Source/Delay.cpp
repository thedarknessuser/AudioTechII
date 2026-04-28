/*
  ==============================================================================

    Delay.cpp
 
    This code contains the implementation needed for a simple feedback delay.

  ==============================================================================
*/

#include "Delay.h"


void Delay::prepare(double samplingRate, int maxDelay, int numChannels)
{

}

void Delay::setMaxDelayInSamples(int maxDelay)
{
    
}

int Delay::getMaxDelayInSamples()
{
    return 0;
}

void Delay::setDelayTime(float delaySeconds)
{
    
}

void Delay::setWetMix(float wetAmount)
{
    
}

// this is called in the ProcessBlock as we iterate over each channel's buffer
float Delay::processSample(float inputSample, int channel)
{
    return 0.;
}
