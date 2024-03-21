/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NoteDetectorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NoteDetectorAudioProcessorEditor (NoteDetectorAudioProcessor&);
    ~NoteDetectorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
    {
        // Process incoming audio data in bufferToFill.buffer
        const float* inputData = bufferToFill.buffer->getReadPointer(0); // Assuming mono audio
        const int numSamples = bufferToFill.numSamples;

        // Autocorrelation variables
        double maxCorrelation = 0.0;
        int maxShift = 0;

        // Autocorrelation computation
        for (int shift = 1; shift < numSamples; ++shift) {
            double correlation = 0.0;
            for (int i = 0; i < numSamples - shift; ++i) {
                correlation += inputData[i] * inputData[i + shift];
            }
            if (correlation > maxCorrelation) {
                maxCorrelation = correlation;
                maxShift = shift;
            }
        }

        // Get the sample rate from the audio device manager
        double sampleRate = audioDeviceManager.getCurrentAudioDevice()->getCurrentSampleRate();

        // Compute pitch from the autocorrelation peak using sample rate
        double fundamentalFrequency = sampleRate / maxShift;
        double midiNote = 69 + 12 * log2(fundamentalFrequency / 440.0);
        int noteIndex = (int)round(midiNote) % 12;
        juce::String noteName = getNoteName(noteIndex);

        // Display detected note
        displayDetectedNote(noteName);
    }
    juce::StringArray notes = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NoteDetectorAudioProcessor& audioProcessor;
    juce::AudioDeviceManager audioDeviceManager;
    juce::Label noteLabel;

    juce::String getRandomNote() {
        return notes[std::rand() % notes.size()];
    }

    void displayDetectedNote(const juce::String& noteName) {
        // Update the label with the detected note
        noteLabel.setText("Current Note: " + noteName, juce::dontSendNotification);
    }

    juce::String getNoteName(int index) {
        return notes[index % 12];
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteDetectorAudioProcessorEditor)
};
