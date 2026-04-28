/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioTech2Lecture7AudioProcessor::AudioTech2Lecture7AudioProcessor()
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

AudioTech2Lecture7AudioProcessor::~AudioTech2Lecture7AudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioTech2Lecture7AudioProcessor::createParams()
{
    return {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"delay", 1}, "Delay length", 0.0, 5, 0.25)
    };
}

//==============================================================================
const juce::String AudioTech2Lecture7AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioTech2Lecture7AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioTech2Lecture7AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioTech2Lecture7AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioTech2Lecture7AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioTech2Lecture7AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioTech2Lecture7AudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioTech2Lecture7AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioTech2Lecture7AudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioTech2Lecture7AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioTech2Lecture7AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    samplingRate = sampleRate;

    bufferSize = samplesPerBlock;

    int maxDelay = maxDelaySec * samplingRate;

    int numChannels = getTotalNumOutputChannels();

    delay.prepare(samplingRate, maxDelay, numChannels);
}

void AudioTech2Lecture7AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioTech2Lecture7AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioTech2Lecture7AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();

    // get params

    auto* delayParam = apvts.getRawParameterValue("delay");
    auto delayLengthSec = delayParam->load();

    int delayInSamples = (int)(delayLengthSec * samplingRate);

    delay.setDelayLength(delayInSamples);

    delay.setWetMix(0.5);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int i = 0; i < numSamples; ++i)
        {
            channelData[i] = delay.processSample(channelData[i], channel);
        }
    }
}

//==============================================================================
bool AudioTech2Lecture7AudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioTech2Lecture7AudioProcessor::createEditor()
{
    return new AudioTech2Lecture7AudioProcessorEditor (*this);
}

//==============================================================================
void AudioTech2Lecture7AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioTech2Lecture7AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioTech2Lecture7AudioProcessor();
}
