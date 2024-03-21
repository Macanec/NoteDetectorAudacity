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
}

void NoteDetectorAudioProcessorEditor::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
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
