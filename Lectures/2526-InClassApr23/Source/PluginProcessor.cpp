/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_2526InClassApr23AudioProcessor::_2526InClassApr23AudioProcessor()
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

_2526InClassApr23AudioProcessor::~_2526InClassApr23AudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout _2526InClassApr23AudioProcessor::createParams()
{
    return
    {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"delay", 1}, "Delay Length", 0.0, 2, 0.25)
    };
}

//==============================================================================
const juce::String _2526InClassApr23AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _2526InClassApr23AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _2526InClassApr23AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _2526InClassApr23AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _2526InClassApr23AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _2526InClassApr23AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _2526InClassApr23AudioProcessor::getCurrentProgram()
{
    return 0;
}

void _2526InClassApr23AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _2526InClassApr23AudioProcessor::getProgramName (int index)
{
    return {};
}

void _2526InClassApr23AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _2526InClassApr23AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    samplingRate = sampleRate;
    
    bufferSize = samplesPerBlock;
    
    int maxDelay = maxDelaySec * samplingRate;
    
    int numChannels = getTotalNumOutputChannels();
    
    delay.prepare(samplingRate, maxDelay, numChannels);
}

void _2526InClassApr23AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _2526InClassApr23AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _2526InClassApr23AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto* delayParam = apvts.getRawParameterValue("delay");
    auto delayLengthSec = delayParam->load();
    
    
    delay.setDelayTime(delayLengthSec);
    
    delay.setWetMix(0.5);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < bufferSize; ++i)
        {
            channelData[i] = delay.processSample(channelData[i], channel);
        }
    }
}

//==============================================================================
bool _2526InClassApr23AudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _2526InClassApr23AudioProcessor::createEditor()
{
    return new _2526InClassApr23AudioProcessorEditor (*this);
}

//==============================================================================
void _2526InClassApr23AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _2526InClassApr23AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _2526InClassApr23AudioProcessor();
}
