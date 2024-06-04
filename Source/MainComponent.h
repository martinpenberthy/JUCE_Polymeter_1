#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Timer, public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void timerCallback() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    //==============================================================================
    // Your private member variables go here...
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    double currentFrequency = 500.0, targetFrequency = 500.0; // [5]
    
    bool isBeep;
    
    juce::Slider sliderTime;
    int timeInterval = 1000;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
