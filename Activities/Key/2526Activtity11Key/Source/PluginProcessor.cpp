/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_2526Activtity11KeyAudioProcessor::_2526Activtity11KeyAudioProcessor()
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

_2526Activtity11KeyAudioProcessor::~_2526Activtity11KeyAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout _2526Activtity11KeyAudioProcessor::createParams()
{
    return {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"gain", 1}, "Gain", 0, 1, 0.5),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"freq", 1}, "Frequency", 50, 5000, 440)
    };
}

//==============================================================================
const juce::String _2526Activtity11KeyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _2526Activtity11KeyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _2526Activtity11KeyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _2526Activtity11KeyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _2526Activtity11KeyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _2526Activtity11KeyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _2526Activtity11KeyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _2526Activtity11KeyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _2526Activtity11KeyAudioProcessor::getProgramName (int index)
{
    return {};
}

void _2526Activtity11KeyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _2526Activtity11KeyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // You need to initialize your variables here!
    samplingRate = sampleRate;
    numSamplesPerBlock = samplesPerBlock;
    
//    freq = 440;
//    amp = 1;
    phase = 0;
        
    // envelope length in samples;
    envSamples = samplingRate * int(envSec);
    
    // envelope sample tracker;
    envTracker = 0;
    
    lfoFreq = 5;
    lfoAmp = 0.5;
    lfoPhase = 0;
}

void _2526Activtity11KeyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _2526Activtity11KeyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _2526Activtity11KeyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
//    applyLFO(buffer);

}

void _2526Activtity11KeyAudioProcessor::genSineWave(juce::AudioBuffer<float>& buffer)
{
    // Fill the buffer (in place) with a sinusoid
    float phaseStart = phase;
    
    auto* gainParam = apvts.getRawParameterValue("gain");
    amp = gainParam->load();
    
    auto* freqParam = apvts.getRawParameterValue("freq");
    freq = freqParam->load();
    
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        phase = phaseStart;
        auto* channelData = buffer.getWritePointer (channel);
        for (int i = 0; i < numSamplesPerBlock; i++)
        {
            channelData[i] = amp * sinf(phase);
            phase += juce::MathConstants<float>::twoPi * freq / samplingRate;
            if (phase >= juce::MathConstants<float>::twoPi)
            {
                phase -= juce::MathConstants<float>::twoPi;
            }
        }
    }
}

void _2526Activtity11KeyAudioProcessor::applyEnvRamp(juce::AudioBuffer<float>& buffer)
{
    // Apply an amplitude envelope to the buffer (in place)
    // Multiply each sample by an envelope value (0 → 1 → 0)
    int envStart = envTracker;
    float envVal;
    float halfEnvLen = float(envSamples) / 2;
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        envTracker = envStart;
        auto* channelData = buffer.getWritePointer(channel);

        for (int i = 0; i < numSamplesPerBlock; i++)
        {

            if (envTracker < envSamples / 2)
                envVal = envTracker / halfEnvLen;
            else
                envVal = 1.0f - (envTracker - halfEnvLen) / halfEnvLen;

            channelData[i] *= envVal;

            envTracker++;
            if (envTracker >= envSamples)
                envTracker = 0;
        }
    }
}

void _2526Activtity11KeyAudioProcessor::applyLFO(juce::AudioBuffer<float>& buffer) {
    float phaseStart = lfoPhase;
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        lfoPhase = phaseStart;
        auto* channelData = buffer.getWritePointer (channel);
        for (int i = 0; i < numSamplesPerBlock; i++)
        {
            channelData[i] *= lfoAmp * sinf(lfoPhase);
            lfoPhase += juce::MathConstants<float>::twoPi * lfoFreq / samplingRate;
            if (lfoPhase >= juce::MathConstants<float>::twoPi)
            {
                lfoPhase -= juce::MathConstants<float>::twoPi;
            }
        }
    }
}


//==============================================================================
bool _2526Activtity11KeyAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _2526Activtity11KeyAudioProcessor::createEditor()
{
    return new _2526Activtity11KeyAudioProcessorEditor (*this);
}

//==============================================================================
void _2526Activtity11KeyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _2526Activtity11KeyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _2526Activtity11KeyAudioProcessor();
}
