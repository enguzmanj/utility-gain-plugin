#include "PluginProcessor.h"
#include "PluginEditor.h"

UtilityGainAudioProcessorEditor::UtilityGainAudioProcessorEditor(UtilityGainAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(& square);
    
    setSize (400, 300);
}

UtilityGainAudioProcessorEditor::~UtilityGainAudioProcessorEditor () {}

void UtilityGainAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    
    g.setColour(juce::Colours::white);
    g.setFont(50.0f);
    g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void UtilityGainAudioProcessorEditor::resized()
{
    square.setBounds(50, 50, 300, 50);
}


