#include "PluginProcessor.h"
#include "PluginEditor.h"


UtilityGainAudioProcessor::UtilityGainAudioProcessor() {}
UtilityGainAudioProcessor::~UtilityGainAudioProcessor() {}

UtilityGainAudioProcessor::APVTS::ParameterLayout
UtilityGainAudioProcessor::createParameterLayout()
{
    using namespace juce;
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    // dB range: -60.0 dB, with skew for finer control near 0 dB.
    NormalisableRange<float> dBRange (-60.0f, 0.0f, 0.01f, 0.5f);
    
    params.push_back (std::make_unique<AudioParameterFloat>(
                                                            ParameterID { ids::gain, 1 },
                                                            "Gain",
                                                            dBRange,
                                                            0.0f, //default 0 dB
                                                            AudioParameterFloatAttributes().withStringFromValueFunction(
                                                                                                                       [] (float v, int) { return juce::String(v, 1) + "dB";} )));
    return { params.begin(), params.end()};
}

void UtilityGainAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary(*xml, destData);
}

void UtilityGainAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

void UtilityGainAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    gainSmoothed.reset(sampleRate, 0.02);
    gainSmoothed.setCurrentAndTargetValue(1.0f);
};

void UtilityGainAudioProcessor::releaseResources() {};

void UtilityGainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // 1) Read parameter atomically (dB)
    auto* gainParam = apvts.getRawParameterValue("gain");
    const float gainDB = gainParam->load();
    
    // 2) Convert to linear
    const float targetLinear = juce::Decibels::decibelsToGain(gainDB);
    
    // 3) Update smoother target
    gainSmoothed.setTargetValue(targetLinear);
    
    // 4) Apply per-sample ramp
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    
    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        for (int n = 0; n < numSamples; ++n)
        {
            data[n] *= gainSmoothed.getNextValue();
        }
    }
    
    
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
