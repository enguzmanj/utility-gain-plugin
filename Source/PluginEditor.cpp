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
    gainAttach = std::make_unique<Attachment>(audioProcessor.apvts, ids::gain, gainSlider);
}

UtilityGainAudioProcessorEditor::~UtilityGainAudioProcessorEditor () {}

void UtilityGainAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

}

void UtilityGainAudioProcessorEditor::resized()
{
    square.setBounds(50, 5, 300, 50);
    auto r = getLocalBounds().reduced(55);
    gainLabel.setBounds(r.removeFromTop(24));
    gainSlider.setBounds(r.withSizeKeepingCentre(r.getHeight() * 0.75, 200));
}


