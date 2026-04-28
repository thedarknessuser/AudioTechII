/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
InClassApr16AudioProcessor::InClassApr16AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
apvts(*this, nullptr, "Parameters", createParams())
{
}

InClassApr16AudioProcessor::~InClassApr16AudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout InClassApr16AudioProcessor::createParams()
{
    return {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"delay", 1}, "Delay length", 0.0, 5, 0.25)
    };
}

//==============================================================================
const juce::String InClassApr16AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool InClassApr16AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool InClassApr16AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool InClassApr16AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double InClassApr16AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int InClassApr16AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int InClassApr16AudioProcessor::getCurrentProgram()
{
    return 0;
}

void InClassApr16AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String InClassApr16AudioProcessor::getProgramName (int index)
{
    return {};
}

void InClassApr16AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void InClassApr16AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    samplingRate = sampleRate;
    bufferSize = samplesPerBlock;
    
    delayBufferSize = (int) (sampleRate * maxDelayLength);
    
    int numChannels = getTotalNumOutputChannels();
    delayBuffer.setSize(numChannels, delayBufferSize);
    delayBuffer.clear();
    
    writeTail = 0;
}

void InClassApr16AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool InClassApr16AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void InClassApr16AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    delay(buffer);
    
}

void InClassApr16AudioProcessor::delay(juce::AudioBuffer<float> &buffer)
{
    int numChannels = buffer.getNumChannels();
    
    auto* delayParam = apvts.getRawParameterValue("delay");
    auto delayLengthSec = delayParam->load();
    
    int delayInSamples = (int) (delayLengthSec * samplingRate);
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        float* delayData = delayBuffer.getWritePointer(channel);
        
        for (int i = 0; i < bufferSize; ++i)
        {
            int writeIndex = (writeTail + i) % delayBufferSize;
            int readIndex = (writeIndex - delayInSamples + delayBufferSize) % delayBufferSize;
            
            float input = channelData[i];
            
            float delayed = delayData[readIndex];
            
            delayData[writeIndex] = input;
            
            channelData[i] = delayed;
        }
    }
    
    writeTail = (writeTail + bufferSize) % delayBufferSize;
    
}

//==============================================================================
bool InClassApr16AudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* InClassApr16AudioProcessor::createEditor()
{
    return new InClassApr16AudioProcessorEditor (*this);
}

//==============================================================================
void InClassApr16AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void InClassApr16AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new InClassApr16AudioProcessor();
}
