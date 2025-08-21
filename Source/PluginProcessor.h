#pragma once

#include <JuceHeader.h>

//===========================================================

namespace ids
{
    inline constexpr auto gain = "gain";
    inline constexpr auto bypass = "bypass";
}

class UtilityGainAudioProcessor : public juce::AudioProcessor
{
public:
    using APVTS = juce::AudioProcessorValueTreeState;
    
    UtilityGainAudioProcessor();
    ~UtilityGainAudioProcessor() override;
    
    // JUCE-required overrides:
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override { return true;}
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override {return true;}
    
    const juce::String getName() const override { return "UtilityGain";}
    
    bool acceptsMidi() const  override { return false;}
    bool producesMidi() const override { return false;}
    bool isMidiEffect() const override { return false;}
    double getTailLengthSeconds() const override { return 0.0;}
    
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override {return {};}
    void changeProgramName (int index, const juce::String& newName) override {}
    
    // ---- Parameter State ---- //
    static APVTS::ParameterLayout createParameterLayout();
    APVTS apvts { *this, nullptr, "PARAMS", createParameterLayout() };
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
private:
    // Smoothed linear gain used in processBlock
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> gainSmoothed;
    
    // Smoothed bypass
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> bypassXfade; // 0=process, 1=bypass
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityGainAudioProcessor)
};
