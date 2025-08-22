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
                                                                                                                       [] (float v, int) {
                                                                                                                           if (v <= -59.95) return juce::String("-inf dB");
                                                                                                                           if (std::abs(v) < 0.05) return juce::String("0.0 dB");
                                                                                                                           return juce::String(v, 1) + "dB";} )));
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ ids::bypass, 1 }, "Bypass", false));
    
    return { params.begin(), params.end()};
}

void UtilityGainAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    gainSmoothed.reset(sampleRate, 0.02);
    gainSmoothed.setCurrentAndTargetValue(1.0f);
    
    bypassXfade.reset(sampleRate, 0.005);
    bypassXfade.setCurrentAndTargetValue(0.0f);
};

void UtilityGainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    
    // --- Input Meters (before processing)
    
    float inPeakBlock = 0.0f;
    double inRMSAcc = 0.0;
    
    for (int ch = 0; ch < numChannels; ++ch)
    {
        const float* rd = buffer.getReadPointer(ch);
        for (int n = 0; n < numSamples; ++n)
        {
            const float x = rd[n];
            inPeakBlock = std::max(inPeakBlock, std::abs(x));
            inRMSAcc += double(x) * double(x);
        }
    }
    
    const float inRMSBlock = std::sqrt(inRMSAcc/std::max(1, numSamples * numChannels));
    inPeak.store(inPeakBlock);
    inRMS.store(inRMSBlock);
    
    auto bypassParam = apvts.getRawParameterValue(ids::bypass);
    const bool isBypassed = bypassParam->load();
    
    // Set crossfade target
    bypassXfade.setTargetValue(isBypassed ? 1.0f : 0.0f);
    
    // Keep a copy of buffer for crossfade
    juce::AudioBuffer<float> dry (buffer.getNumChannels(), buffer.getNumSamples());
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        dry.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples());
    
    // 1) Read parameter atomically (dB)
    auto* gainParam = apvts.getRawParameterValue(ids::gain);
    const float gainDB = gainParam->load();
    
    // 2) Convert to linear
    const float targetLinear = juce::Decibels::decibelsToGain(gainDB);
    
    // 3) Update smoother target
    gainSmoothed.setTargetValue(targetLinear);
    
    // 4) Apply per-sample ramp
    
    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        for (int n = 0; n < numSamples; ++n)
        {
            data[n] *= gainSmoothed.getNextValue();
        }
    }
    
    for (int ch = 0; ch < numChannels; ++ch)
    {
        auto* wet = buffer.getWritePointer(ch);
        auto* d = dry.getReadPointer(ch);
        for (int n = 0; n < numSamples; ++n)
        {
            const float x = bypassXfade.getNextValue();
            wet[n] = wet[n] * (1 - x) + d[n] * x;
        }
    }
    
    // --- Output meters (after processing)
    float outPeakBlock = 0.0f;
    double outRMSAcc = 0.0;
    
    for (int ch = 0; ch < numChannels; ++ch)
    {
        const float* rd = buffer.getReadPointer(ch);
        for (int n = 0; n < numSamples; ++n)
        {
            const float y = rd[n];
            outPeakBlock = std::max(outPeakBlock, std::abs(y));
            outRMSAcc += double(y) * double(y);
        }
    }
    
    const float outRMSBlock = std::sqrt(outRMSAcc/std::max(1, numSamples * numChannels));
    outPeak.store(outPeakBlock);
    outRMS.store(outRMSBlock);
    
    
}

float UtilityGainAudioProcessor::getGainDB() const
{
    return apvts.getRawParameterValue( ids::gain )->load();
}
float UtilityGainAudioProcessor::getGainNorm() const
{
    auto* p = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(ids::gain));
    jassert(p != nullptr);
    const float db = getGainDB();
    return p->getNormalisableRange().convertTo0to1(db);
}

void UtilityGainAudioProcessor::setGainDB(float db)
{
    auto* p = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(ids::gain));
    jassert(p != nullptr);
    const auto& range = p->getNormalisableRange();
    const float norm = range.convertTo0to1(db);
    p->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, norm));
}
void UtilityGainAudioProcessor::setGainNorm(float norm)
{
    auto* p = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(ids::gain));
    jassert(p != nullptr);
    p->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, norm));
}

void UtilityGainAudioProcessor::releaseResources() {};


juce::AudioProcessorEditor* UtilityGainAudioProcessor::createEditor()
{
    return new UtilityGainAudioProcessorEditor (*this);
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

juce::AudioProcessorParameter* UtilityGainAudioProcessor::getBypassParameter() const
{
    return apvts.getParameter( ids::bypass );
}


//===========================================
// Factory
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UtilityGainAudioProcessor();
}
