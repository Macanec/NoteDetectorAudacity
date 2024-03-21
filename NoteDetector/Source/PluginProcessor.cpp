/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoteDetectorAudioProcessor::NoteDetectorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    detectedPitchParameter = new juce::AudioParameterFloat("DetectedPitch", "Detected Pitch", 0.0f, 20000.0f, 0.0f);
    addParameter(detectedPitchParameter);
}

NoteDetectorAudioProcessor::~NoteDetectorAudioProcessor()
{
}

//==============================================================================
const juce::String NoteDetectorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoteDetectorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoteDetectorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NoteDetectorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NoteDetectorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoteDetectorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoteDetectorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoteDetectorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NoteDetectorAudioProcessor::getProgramName (int index)
{
    return {};
}

void NoteDetectorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NoteDetectorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NoteDetectorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoteDetectorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// Autocorrelation-based pitch detection function
double detectPitch(const std::vector<float>& audioBuffer, double sampleRate) {
    const int numSamples = audioBuffer.size();

    // Autocorrelation variables
    double maxCorrelation = 0.0;
    int maxShift = 0;

    // Autocorrelation computation
    for (int shift = 1; shift < numSamples; ++shift) {
        double correlation = 0.0;
        for (int i = 0; i < numSamples - shift; ++i) {
            correlation += audioBuffer[i] * audioBuffer[i + shift];
        }
        if (correlation > maxCorrelation) {
            maxCorrelation = correlation;
            maxShift = shift;
        }
    }

    // Compute pitch from the autocorrelation peak
    double fundamentalPeriod = maxShift / sampleRate;
    double fundamentalFrequency = 1.0 / fundamentalPeriod;

    return fundamentalFrequency;
}

void NoteDetectorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // ScopedNoDenormals is used to avoid denormal numbers, which can cause performance issues
    juce::ScopedNoDenormals noDenormals;

    // Get the total number of input channels
    auto totalNumInputChannels = getTotalNumInputChannels();

    // Iterate over each input channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel); // Get pointer to the input channel data

        // Convert channelData to std::vector<float>
        std::vector<float> audioBuffer(channelData, channelData + buffer.getNumSamples());

        // Detect pitch using autocorrelation-based method
        double pitch = detectPitch(audioBuffer, getSampleRate());

        // Update detected pitch parameter
        detectedPitchParameter->setValueNotifyingHost((float)pitch);
    }
}

//==============================================================================
bool NoteDetectorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NoteDetectorAudioProcessor::createEditor()
{
    return new NoteDetectorAudioProcessorEditor (*this);
}

//==============================================================================
void NoteDetectorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NoteDetectorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoteDetectorAudioProcessor();
}
