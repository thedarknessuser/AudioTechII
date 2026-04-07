/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_2526Activity10AudioProcessor::_2526Activity10AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

_2526Activity10AudioProcessor::~_2526Activity10AudioProcessor()
{
}

//==============================================================================
const juce::String _2526Activity10AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _2526Activity10AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _2526Activity10AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _2526Activity10AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _2526Activity10AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _2526Activity10AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _2526Activity10AudioProcessor::getCurrentProgram()
{
    return 0;
}

void _2526Activity10AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _2526Activity10AudioProcessor::getProgramName (int index)
{
    return {};
}

void _2526Activity10AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _2526Activity10AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // You need to initialize your variables here!
    
}

void _2526Activity10AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _2526Activity10AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _2526Activity10AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    genSineWave(buffer);
    applyEnvRamp(buffer);
}

void _2526Activity10AudioProcessor::genSineWave(juce::AudioBuffer<float>& buffer)
{
    // Fill the buffer (in place) with a sinusoid
    // your code goes here!
    
}


void _2526Activity10AudioProcessor::applyEnvRamp(juce::AudioBuffer<float>& buffer)
{
    // Apply an amplitude envelope to the buffer (in place)
    // Multiply each sample by an envelope value (0 → 1 → 0)
    // your code goes here!
}

//==============================================================================
bool _2526Activity10AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _2526Activity10AudioProcessor::createEditor()
{
    return new _2526Activity10AudioProcessorEditor (*this);
}

//==============================================================================
void _2526Activity10AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _2526Activity10AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _2526Activity10AudioProcessor();
}
