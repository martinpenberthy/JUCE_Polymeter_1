/*
  ==============================================================================

    TimeComponent.h
    Created: 4 Jun 2024 3:51:53pm
    Author:  Martin Penberthy

  ==============================================================================
*/

#pragma once

class TimeComponent
{
    public:
        TimeComponent()
        {
            
        }
        void prepareToPlay(double sampleRate)
        {
            currentSampleRate = sampleRate;
            
            sampsPerBeat = currentSampleRate / 4;
            sampsCounterBeep = sampsPerBeat / 4;
            sampsCounterSilence = sampsPerBeat / 4;
            
            auto cyclesPerSample = currentFrequency / currentSampleRate;
            angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
            
        }
    
        void processBlock(const juce::AudioSourceChannelInfo& bufferToFill)
        {
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
                    leftBuffer[sample]  += currentSample * level;
                    rightBuffer[sample] += currentSample * level;
                    sampsCounterBeep--;
                }
                else if(sampsCounterSilence > 0)
                {
                    leftBuffer[sample]  += 0.0f;
                    rightBuffer[sample] += 0.0f;
                    sampsCounterSilence--;
                }
                
                if(sampsCounterBeep == 0 && sampsCounterSilence == 0)
                {
                    sampsCounterBeep = sampsPerBeat;
                    
                }
                else if(sampsCounterSilence == 0)
                {
                    sampsCounterSilence = sampsPerBeat;
                }
            }
        }
    
        void setTempo(int tempo)
        {
            sampsPerBeat = currentSampleRate / (tempo / 30);
            sampsCounterBeep = sampsPerBeat / 4;
            sampsCounterSilence = sampsPerBeat / 4;
        }
        void setFrequency(double newFreq)
        {
            currentFrequency = newFreq;
            
            auto cyclesPerSample = currentFrequency / currentSampleRate;
            angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
        }
        
    private:
        double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
        double currentFrequency = 500.0;
        
        int sampsPerBeat = currentSampleRate;
        int sampsCounterBeep = sampsPerBeat;
        int sampsCounterSilence = sampsPerBeat;
};
