/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoteDetectorAudioProcessorEditor::NoteDetectorAudioProcessorEditor (NoteDetectorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    audioDeviceManager.initialise(0, 2, nullptr, true);
    addAndMakeVisible(noteLabel);
    getNextAudioBlock();
}

void NoteDetectorAudioProcessorEditor::getNextAudioBlock ()
{
    // Implement your pitch detection algorithm here
        // Detect the pitch and convert it to a note name
        // For this example, we'll just display a random note
    juce::String note = getRandomNote();
    noteLabel.setText("Current Note: " + note, juce::dontSendNotification);
}

NoteDetectorAudioProcessorEditor::~NoteDetectorAudioProcessorEditor()
{
}

//==============================================================================
void NoteDetectorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void NoteDetectorAudioProcessorEditor::resized()
{
    noteLabel.setBounds(getLocalBounds().reduced(20));
}
