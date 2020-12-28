/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define DEBUGPLUGIN

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
//#ifdef DEBUGPLUGIN
    m_flogger = std::unique_ptr<juce::FileLogger>(juce::FileLogger::createDateStampedLogger("/home/enrique", "mylog", ".txt", "Welcome to plugin"));
//#endif
}

HarmonizationmachineAudioProcessor::~HarmonizationmachineAudioProcessor()
{
}

void HarmonizationmachineAudioProcessor::magentaSystemCall(juce::String inputPrimerMidiNotes, double temperature, double qpm)
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
    // Conversion from double to std::string
    std::ostringstream strs1, strs2;
    strs1 << temperature;
    strs2 << qpm;
    std::string inputTemperature = " --temperature="+strs1.str();
    std::string inputQpm = " --qpm="+strs2.str();
    //num-outputs
    //num-steps

    // Building the magenta model command
    std::string polyphony_rnn_string_call_first_part = "polyphony_rnn_generate --bundle_file=${BUNDLE_PATH} --output_dir=$HOME/HarmonizationMachine/outputs --num_outputs=3 --num_steps=64 --primer_melody=\"";
    std::string polyphony_rnn_string_call_second_part = "\" --condition_on_primer=false --inject_primer_during_generation=true";
    std::string full_polyphony_rnn_command = polyphony_rnn_string_call_first_part+ inputPrimerMidiNotesString +polyphony_rnn_string_call_second_part+inputTemperature+inputQpm;

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
    //buffer.clear();

    //#ifdef DEBUGPLUGIN
    if(m_flogger)
    {
        //m_flogger->logMessage(std::to_string(midiMessages.getNumEvents()));
        for (const juce::MidiMessageMetadata metadata : midiMessages)
        {
            //if (metadata.numBytes == 3)
            m_flogger->logMessage(metadata.getMessage().getDescription());        
        }
    }
    //#endif
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
