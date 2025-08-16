#include "PluginProcessor.h"
#include "PluginEditor.h"

UtilityGainAudioProcessor::UtilityGainAudioProcessor() {}

UtilityGainAudioProcessor::~UtilityGainAudioProcessor() {}

juce::AudioProcessorEditor* UtilityGainAudioProcessor::createEditor()
{
    return new UtilityGainAudioProcessorEditor (*this);
}
