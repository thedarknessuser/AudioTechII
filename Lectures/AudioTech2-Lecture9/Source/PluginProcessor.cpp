/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioTech2Lecture9AudioProcessor::AudioTech2Lecture9AudioProcessor()
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

AudioTech2Lecture9AudioProcessor::~AudioTech2Lecture9AudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioTech2Lecture9AudioProcessor::createParams()
{
    return {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"delay", 1}, "Delay length", 0.0, 2, 0.25),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"mix", 1}, "Mix", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"feedback", 1}, "Feedback", 0.0f, 0.95f, 0.3f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"rate", 1}, "Rate", 0.05f, 5.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"depth", 1}, "Depth", 0.0f, 0.5f, 0.1f), // seconds
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"modOn", 1}, "Modulation On", true)
    };
}

//==============================================================================
const juce::String AudioTech2Lecture9AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioTech2Lecture9AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioTech2Lecture9AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioTech2Lecture9AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioTech2Lecture9AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioTech2Lecture9AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioTech2Lecture9AudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioTech2Lecture9AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioTech2Lecture9AudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioTech2Lecture9AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioTech2Lecture9AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    samplingRate = sampleRate;

    bufferSize = samplesPerBlock;

    int maxDelay = maxDelaySec * samplingRate;

    int numChannels = getTotalNumOutputChannels();

    delay.prepare(samplingRate, maxDelay, numChannels);

}

void AudioTech2Lecture9AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioTech2Lecture9AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioTech2Lecture9AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    DBG("process block called");

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


    int numSamples = buffer.getNumSamples();

    // get params

    auto* delayParam = apvts.getRawParameterValue("delay");
    auto delayLengthSec = delayParam->load();

    delay.setDelayTime(delayLengthSec);

    delay.setWetMix(0.5);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int i = 0; i < numSamples; ++i)
        {
            channelData[i] = delay.processSample(channelData[i], channel);
        }
    }
}

//==============================================================================
bool AudioTech2Lecture9AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioTech2Lecture9AudioProcessor::createEditor()
{
    return new AudioTech2Lecture9AudioProcessorEditor (*this);
}

//==============================================================================
void AudioTech2Lecture9AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioTech2Lecture9AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioTech2Lecture9AudioProcessor();
}
