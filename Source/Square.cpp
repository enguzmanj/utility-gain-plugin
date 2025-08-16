#include "Square.h"

void Square::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::blue);
    
    g.setColour(juce::Colours::red);
    g.setFont(30.0f);
    g.drawFittedText("Nico learning JUCE!", getLocalBounds(), juce::Justification::centred, 1);
}

void Square::resized () {};
