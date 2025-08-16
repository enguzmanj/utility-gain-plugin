#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//============================================================
class UtilityGainAudioProcessorEditor : juce::AudioProcessorEditor
{
public:
    explicit UtilityGainAudioProcessorEditor (UtilityGainAudioProcessor&);
    ~UtilityGainAudioProcessorEditor() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    UtilityGainAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityGainAudioProcessorEditor);
};
