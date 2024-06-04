#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    startTimer(timeInterval);
    
    addAndMakeVisible(&sliderTime);
    sliderTime.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderTime.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 300, 50);
    sliderTime.setRange(50.0f, 300.0f);
    sliderTime.addListener(this);
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}
void MainComponent::sliderValueChanged (juce::Slider* slider)
{
    if(slider == &sliderTime)
    {
        stopTimer();
        
        timeInterval = (int)sliderTime.getValue() / 60;
        startTimer(timeInterval);
    }
}

void MainComponent::timerCallback()
{
    if(getTimerInterval() == timeInterval)
    {
        isBeep = !isBeep;
        
        DBG("isBeep flipped");
    }
    return;
}
//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    currentSampleRate = sampleRate;
    
    auto cyclesPerSample = currentFrequency / currentSampleRate;         // [2]
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
    
    isBeep = true;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
    
    auto level = 0.0f;
    
    auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        if(sampsCounterBeep > 0)
        {
            level = 0.125f;
            
            auto currentSample = (float) std::sin (currentAngle);
            currentAngle += angleDelta;
            leftBuffer[sample]  = currentSample * level;
            rightBuffer[sample] = currentSample * level;
            sampsCounterBeep--;
        }
        else if(sampsCounterSilence > 0)
        {
            leftBuffer[sample]  = 0.0f;
            rightBuffer[sample] = 0.0f;
            sampsCounterSilence--;
        }
        else if(sampsCounterBeep == 0)
        {
            
                sampsCounterSilence = sampsPerBeat;
            
        }
        else if(sampsCounterSilence == 0)
        {
            sampsCounterBeep = sampsPerBeat;
        }
    }
    
    /*
    if(isBeep)
    {
        level = 0.125f;
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            auto currentSample = (float) std::sin (currentAngle);
            currentAngle += angleDelta;
            leftBuffer[sample]  = currentSample * level;
            rightBuffer[sample] = currentSample * level;
        }
        DBG("Beep on");
    }
    else
    {
        level = 0.0f;
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            auto currentSample = (float) std::sin (currentAngle);
            currentAngle += angleDelta;
            leftBuffer[sample]  = currentSample * level;
            rightBuffer[sample] = currentSample * level;
        }
        DBG("Beep off");
    }*/
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    sliderTime.setBounds(100, 100, 100, 300);
}
