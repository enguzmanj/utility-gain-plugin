#include "PluginProcessor.h"
#include "PluginEditor.h"

UtilityGainAudioProcessor::UtilityGainAudioProcessor() {}

UtilityGainAudioProcessor::~UtilityGainAudioProcessor() {}

void UtilityGainAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock){};
void UtilityGainAudioProcessor::releaseResources() {};
void UtilityGainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // For now: just clear the buffer (silence)
    buffer.clear();
}

juce::AudioProcessorEditor* UtilityGainAudioProcessor::createEditor()
{
    return new UtilityGainAudioProcessorEditor (*this);
}


//===========================================
// Factory
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UtilityGainAudioProcessor();
}
