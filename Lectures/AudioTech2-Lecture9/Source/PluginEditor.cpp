/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioTech2Lecture9AudioProcessorEditor::AudioTech2Lecture9AudioProcessorEditor (AudioTech2Lecture9AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);


    // get apvts ref
    auto& apvts = audioProcessor.apvts;

    // quick set up for sliders
    auto setupSlider = [](juce::Slider& s)
        {
            s.setSliderStyle(juce::Slider::Rotary);
            s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        };

    setupSlider(delaySlider);
    setupSlider(mixSlider);
    setupSlider(feedbackSlider);
    setupSlider(rateSlider);
    setupSlider(depthSlider);

    // components
    addAndMakeVisible(delayGroup);
    addAndMakeVisible(modGroup);

    addAndMakeVisible(delaySlider);
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(feedbackSlider);

    addAndMakeVisible(rateSlider);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(modButton);


    // labels 
    auto setupLabel = [](juce::Label& l, const juce::String& text)
        {
            l.setText(text, juce::dontSendNotification);
            l.setJustificationType(juce::Justification::centred);
            l.setColour(juce::Label::textColourId, ColoursTheme::text);
        };

    setupLabel(delayLabel, "Delay");
    setupLabel(mixLabel, "Mix");
    setupLabel(feedbackLabel, "Feedback");
    setupLabel(rateLabel, "Rate");
    setupLabel(depthLabel, "Depth");

    addAndMakeVisible(delayLabel);
    addAndMakeVisible(mixLabel);
    addAndMakeVisible(feedbackLabel);
    addAndMakeVisible(rateLabel);
    addAndMakeVisible(depthLabel);

    // attachments
    delayAttach = std::make_unique<SliderAttachment>(apvts, "delay", delaySlider);
    mixAttach = std::make_unique<SliderAttachment>(apvts, "mix", mixSlider);
    feedbackAttach = std::make_unique<SliderAttachment>(apvts, "feedback", feedbackSlider);

    rateAttach = std::make_unique<SliderAttachment>(apvts, "rate", rateSlider);
    depthAttach = std::make_unique<SliderAttachment>(apvts, "depth", depthSlider);

    modAttach = std::make_unique<ButtonAttachment>(apvts, "modOn", modButton);
}

AudioTech2Lecture9AudioProcessorEditor::~AudioTech2Lecture9AudioProcessorEditor()
{
}

//==============================================================================
void AudioTech2Lecture9AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(ColoursTheme::background);

    auto area = getLocalBounds().reduced(10);

    auto top = area.removeFromTop(area.getHeight() / 2);

    g.setColour(ColoursTheme::panel);
    g.fillRoundedRectangle(top.toFloat(), 10.0f);
    g.fillRoundedRectangle(area.toFloat(), 10.0f);

    delayGroup.setColour(juce::GroupComponent::outlineColourId, ColoursTheme::accent);
    delayGroup.setColour(juce::GroupComponent::textColourId, ColoursTheme::text);

    modGroup.setColour(juce::GroupComponent::outlineColourId, ColoursTheme::accent);
    modGroup.setColour(juce::GroupComponent::textColourId, ColoursTheme::text);

    styleSlider(delaySlider);
    styleSlider(mixSlider);
    styleSlider(feedbackSlider);
    styleSlider(rateSlider);
    styleSlider(depthSlider);

    modButton.setColour(juce::ToggleButton::textColourId, ColoursTheme::text);
    modButton.setColour(juce::ToggleButton::tickColourId, ColoursTheme::accent);
}

void AudioTech2Lecture9AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto area = getLocalBounds().reduced(10);

    auto top = area.removeFromTop(area.getHeight() / 2);

    // Delay group
    delayGroup.setBounds(top);
    auto delayArea = top.reduced(10);

    delaySlider.setBounds(delayArea.removeFromLeft(100));
    mixSlider.setBounds(delayArea.removeFromLeft(100));
    feedbackSlider.setBounds(delayArea.removeFromLeft(100));

    // Mod group
    modGroup.setBounds(area);
    auto modArea = area.reduced(10);

    rateSlider.setBounds(modArea.removeFromLeft(100));
    depthSlider.setBounds(modArea.removeFromLeft(100));
    modButton.setBounds(modArea.removeFromLeft(80).reduced(10));

    // labels
    delayLabel.setBounds(delaySlider.getX(), delaySlider.getBottom(), 100, 20);
    mixLabel.setBounds(mixSlider.getX(), mixSlider.getBottom(), 100, 20);
    feedbackLabel.setBounds(feedbackSlider.getX(), feedbackSlider.getBottom(), 100, 20);

    rateLabel.setBounds(rateSlider.getX(), rateSlider.getBottom(), 100, 20);
    depthLabel.setBounds(depthSlider.getX(), depthSlider.getBottom(), 100, 20);
}

void AudioTech2Lecture9AudioProcessorEditor::styleSlider(juce::Slider& s)
{
    s.setColour(juce::Slider::rotarySliderFillColourId, ColoursTheme::accent);
    s.setColour(juce::Slider::thumbColourId, ColoursTheme::accent);
    s.setColour(juce::Slider::rotarySliderOutlineColourId, ColoursTheme::knob);
    s.setColour(juce::Slider::textBoxTextColourId, ColoursTheme::text);
    s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
}
