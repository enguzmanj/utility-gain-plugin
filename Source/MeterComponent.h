//
//  MeterComponent.h
//  UtilityGain
//
//  Created by Nicolás Guzmán on 8/21/25.
//

#pragma once

#include <JuceHeader.h>

class MeterComponent : public juce::Component, private juce::Timer
{
public:
    MeterComponent (std::function<float()> inPeakFn,
                    std::function<float()> outPeakFn) : readInPeak(std::move(inPeakFn)), readOutPeak(std::move(outPeakFn))
    {
        inSmooth.reset (60.0, 0.15);
        outSmooth.reset (60.0, 0.15);
        startTimerHz(30);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto r = getLocalBounds();
        auto left = r.removeFromLeft(r.getWidth()/2).reduced(4);
        auto right = r.reduced(4);
        
        drawBar(g, left, inSmooth.getCurrentValue(), "IN");
        drawBar(g, right, outSmooth.getCurrentValue(), "OUT");
    }
    
private:
    juce::SmoothedValue<float> inSmooth, outSmooth;
    std::function<float()> readInPeak, readOutPeak;
    
    void timerCallback() override
    {
        const float in = readInPeak();
        const float out = readOutPeak();
        
        inSmooth.setTargetValue(in);
        outSmooth.setTargetValue(out);
        
        inSmooth.skip(1); // advance on "frame" worth of smoothing
        outSmooth.skip(1);
        
        repaint();
    }
    
    static void drawBar (juce::Graphics& g, juce::Rectangle<int> area, float lin, juce::String label)
    {
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(area);
        
        auto filled = area.removeFromBottom(int(area.getHeight() * juce::jlimit(0.0f, 1.0f, lin)));
        
        g.setColour(juce::Colours::chartreuse);
        g.fillRect(filled);
        
        g.setColour(juce::Colours::white.withAlpha(0.8f));
        g.drawFittedText(label, area.withY(2), juce::Justification::centredTop, 1);
    }
    
};
