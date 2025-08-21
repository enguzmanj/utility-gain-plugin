#include "PluginProcessor.h"
#include "PluginEditor.h"

UtilityGainAudioProcessorEditor::UtilityGainAudioProcessorEditor(UtilityGainAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(& square);
    
    setSize (400, 300);
    
    // Slider UI
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(gainSlider);
    
    gainLabel.setText("Gain (dB)", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);
    
    // Attach to parameter
    gainAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, ids::gain, gainSlider);
    
    // Bypass button
    bypassButton.setButtonText("Bypass");
    addAndMakeVisible(bypassButton);
    bypassAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, ids::bypass, bypassButton);
}

UtilityGainAudioProcessorEditor::~UtilityGainAudioProcessorEditor () {}

void UtilityGainAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

}

void UtilityGainAudioProcessorEditor::resized()
{
    square.setBounds(50, 5, 300, 50);
    
    auto r = getLocalBounds().reduced (20);
    bypassButton.setBounds (r.removeFromBottom(r.getHeight() * 0.10).removeFromLeft (r.getWidth() * 0.5));
    
    r = getLocalBounds().reduced(55);
    gainLabel.setBounds(r.removeFromTop(24));
    gainSlider.setBounds(r.withSizeKeepingCentre(r.getHeight() * 0.75, 200));
}


