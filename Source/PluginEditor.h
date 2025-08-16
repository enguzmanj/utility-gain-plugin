#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Square.h"

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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityGainAudioProcessorEditor);
};
