/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HarmonizationmachineAudioProcessor::HarmonizationmachineAudioProcessor()
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
}

HarmonizationmachineAudioProcessor::~HarmonizationmachineAudioProcessor()
{
}

void HarmonizationmachineAudioProcessor::magentaSystemCall(juce::String inputPrimerMidiNotes)
{
    // Create script and magenta directories to execute everything
    system("mkdir $HOME/HarmonizationMachine");
    system("mkdir $HOME/HarmonizationMachine/input");
    system("mkdir $HOME/HarmonizationMachine/outputs");
    system("> $HOME/HarmonizationMachine/input/script.sh");
    system("chmod +x $HOME/HarmonizationMachine/input/script.sh");
    system("echo '#!/bin/bash' >> $HOME/HarmonizationMachine/input/script.sh");

    // Conversion from juce::String to const char*
    std::string inputPrimerMidiNotesString = inputPrimerMidiNotes.toStdString();

    // Building the magenta model command
    std::string polyphony_rnn_string_call_first_part = "polyphony_rnn_generate --bundle_file=${BUNDLE_PATH} --output_dir=$HOME/HarmonizationMachine/outputs --num_outputs=1 --num_steps=64 --primer_melody=\"";
    std::string polyphony_rnn_string_call_second_part = "\" --condition_on_primer=false --inject_primer_during_generation=true";
    std::string full_polyphony_rnn_command = polyphony_rnn_string_call_first_part+ inputPrimerMidiNotesString +polyphony_rnn_string_call_second_part;

    // Insert activate magenta enviroment command into the script
    system("echo 'eval \"$(conda shell.bash hook)\"' >> $HOME/HarmonizationMachine/input/script.sh");
    system("echo 'conda activate magenta' >> $HOME/HarmonizationMachine/input/script.sh");

    // Write the bundle path into the script (.mag file has to be downloaded from https://github.com/magenta/magenta/tree/master/magenta/models/polyphony_rnn)
    system("echo 'BUNDLE_PATH=$HOME/Descargas/polyphony_rnn.mag' >> $HOME/HarmonizationMachine/input/script.sh");

    std::string bash = "echo '";
    std::string redirect = "' >> $HOME/HarmonizationMachine/input/script.sh";
    std::string command = bash+full_polyphony_rnn_command+redirect;
    const char* finalModelCommand = command.c_str();
    system(finalModelCommand);

    // Execute script.sh
    system("$HOME/HarmonizationMachine/input/script.sh");

}

//==============================================================================
const juce::String HarmonizationmachineAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmonizationmachineAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HarmonizationmachineAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HarmonizationmachineAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HarmonizationmachineAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmonizationmachineAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HarmonizationmachineAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmonizationmachineAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HarmonizationmachineAudioProcessor::getProgramName (int index)
{
    return {};
}

void HarmonizationmachineAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HarmonizationmachineAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void HarmonizationmachineAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmonizationmachineAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void HarmonizationmachineAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool HarmonizationmachineAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HarmonizationmachineAudioProcessor::createEditor()
{
    return new HarmonizationmachineAudioProcessorEditor (*this);
}

//==============================================================================
void HarmonizationmachineAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HarmonizationmachineAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonizationmachineAudioProcessor();
}
