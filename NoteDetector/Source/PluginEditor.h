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
    void parameterValueChanged(int parameterIndex, float newValue);
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
