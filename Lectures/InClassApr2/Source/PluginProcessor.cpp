/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
InClassApr2AudioProcessor::InClassApr2AudioProcessor()
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

InClassApr2AudioProcessor::~InClassApr2AudioProcessor()
{
}

//==============================================================================
const juce::String InClassApr2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool InClassApr2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool InClassApr2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool InClassApr2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double InClassApr2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int InClassApr2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int InClassApr2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void InClassApr2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String InClassApr2AudioProcessor::getProgramName (int index)
{
    return {};
}

void InClassApr2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void InClassApr2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    samplingRate = sampleRate;
    blockSize = samplesPerBlock;
    
    freq = 440;
    phase = 0;
    amp = 0.5;
}

void InClassApr2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool InClassApr2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void InClassApr2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
    genSineWave3(buffer);
}


void InClassApr2AudioProcessor::genSineWave1(juce::AudioBuffer<float> &buffer) {
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int i = 0; i < blockSize; i++) {
            channelData[i] = amp * sinf(juce::MathConstants<float>::twoPi * freq / samplingRate * i + phase);
        }
    }
}


void InClassApr2AudioProcessor::genSineWave2(juce::AudioBuffer<float> &buffer)  {
    
    int trackSamplesStart = trackSamples;
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        
        trackSamples = trackSamplesStart;
        
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int i = 0; i < blockSize; i++) {
            channelData[i] = amp * sinf(juce::MathConstants<float>::twoPi * freq / samplingRate * trackSamples + phase);
            
            trackSamples++;
        }
    }
    
}


void InClassApr2AudioProcessor::genSineWave3(juce::AudioBuffer<float> &buffer)  {

    float phaseStart = phase;
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        
        auto* channelData = buffer.getWritePointer (channel);
        phase = phaseStart;
        
        for (int i = 0; i < blockSize; i++) {
            
            channelData[i] = amp * sinf(phase);
            phase += juce::MathConstants<float>::twoPi * freq / samplingRate;
            
            if (phase >= juce::MathConstants<float>:: twoPi) {
                phase -= juce::MathConstants<float>:: twoPi;
            }
            
        }
    }
    
}
//==============================================================================
bool InClassApr2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* InClassApr2AudioProcessor::createEditor()
{
    return new InClassApr2AudioProcessorEditor (*this);
}

//==============================================================================
void InClassApr2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void InClassApr2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new InClassApr2AudioProcessor();
}
