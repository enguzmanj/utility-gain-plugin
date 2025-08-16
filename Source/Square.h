#pragma once

#include <JuceHeader.h>

class Square : public juce::Component
{
public:
    void paint (juce::Graphics& g) override;
    void resized () override;
};
