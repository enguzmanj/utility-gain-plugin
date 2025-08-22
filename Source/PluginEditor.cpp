#include "PluginProcessor.h"
#include "PluginEditor.h"

UtilityGainAudioProcessorEditor::UtilityGainAudioProcessorEditor(UtilityGainAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(& square);
    
    // Slider UI
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    
    gainSlider.textFromValueFunction = [] (double v)
    {
        constexpr double floorDB = -60.0;
        constexpr double eps = 0.05;
        
        if (v <= floorDB + 0.05) return juce::String("-inf dB");
        if (std::abs(v) < eps) return juce::String("0.0 dB");
        return juce::String(v, 1) + " dB";
    };
    
    // Parsing (text -> value)
    gainSlider.valueFromTextFunction = [] (const juce::String& text)
    {
        constexpr double floorDb = -60.0;
        constexpr double ceilDb  = 0.0;
        
        auto t = text.trim().toLowerCase();
        
        if (t == "-inf" || t == "-inf dB") return floorDb;
        
        auto num = t.upToFirstOccurrenceOf("dB", false, false);
        return juce::jlimit(floorDb, ceilDb, num.getDoubleValue());
    };
    
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
    
    
    // Create meters, wiring them to processor atomics via lambdas
    meters = std::make_unique<MeterComponent> (
                                                   [&]{ return audioProcessor.inPeak.load();},
                                                   [&]{ return audioProcessor.outPeak.load();}
                                                   );
    addAndMakeVisible(*meters);
    
    setSize (400, 300);
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
    gainSlider.setBounds(r.withSizeKeepingCentre(r.getHeight() * 0.75, 200).removeFromLeft(150));
    
    // meters on the right, gain on the left/center
    auto right = r.removeFromRight(80);
    meters->setBounds(right);
}


