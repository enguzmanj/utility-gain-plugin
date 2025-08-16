#include "PluginProcessor.h"
#include "PluginEditor.h"

UtilityGainAudioProcessorEditor::UtilityGainAudioProcessorEditor(UtilityGainAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
}

UtilityGainAudioProcessorEditor::~UtilityGainAudioProcessorEditor () {}

void UtilityGainAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void UtilityGainAudioProcessorEditor::resized() {}


