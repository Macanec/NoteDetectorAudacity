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
    void getNextAudioBlock();
    ~NoteDetectorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NoteDetectorAudioProcessor& audioProcessor;
    juce::AudioDeviceManager audioDeviceManager;
    juce::Label noteLabel;

    juce::String getRandomNote() {
        juce::StringArray notes = { "A", "B", "C", "D", "E", "F", "G" };
        return notes[std::rand() % notes.size()];
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteDetectorAudioProcessorEditor)
};
