#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Square.h"
#include "MeterComponent.h"

//============================================================
class UtilityGainAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit UtilityGainAudioProcessorEditor (UtilityGainAudioProcessor&);
    ~UtilityGainAudioProcessorEditor() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    Square  square;
    
    UtilityGainAudioProcessor& audioProcessor;
    
    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::ToggleButton bypassButton;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment>(gainAttach);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>(bypassAttach);
    
    std::unique_ptr<MeterComponent>(meters);
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityGainAudioProcessorEditor);
};
