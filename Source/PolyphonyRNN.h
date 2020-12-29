/*
  ==============================================================================

    PolyphonyRNN.h
    Created: 29 Dec 2020 12:04:22pm
    Author:  enrique

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PolyphonyRNN
{
private:
public:
  void generateHarmonization(juce::String inputPrimerMidiNotes, double temperature, double qpm)
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
    std::string inputTemperature = " --temperature=" + strs1.str();
    std::string inputQpm = " --qpm=" + strs2.str();
    //num-outputs
    //num-steps

    // Building the magenta model command
    std::string polyphony_rnn_string_call_first_part = "polyphony_rnn_generate --bundle_file=${BUNDLE_PATH} --output_dir=$HOME/HarmonizationMachine/outputs --num_outputs=3 --num_steps=64 --primer_melody=\"";
    std::string polyphony_rnn_string_call_second_part = "\" --condition_on_primer=false --inject_primer_during_generation=true";
    std::string full_polyphony_rnn_command = polyphony_rnn_string_call_first_part + inputPrimerMidiNotesString + polyphony_rnn_string_call_second_part + inputTemperature + inputQpm;

    // Insert activate magenta enviroment command into the script
    system("echo 'eval \"$(conda shell.bash hook)\"' >> $HOME/HarmonizationMachine/input/script.sh");
    system("echo 'conda activate magenta' >> $HOME/HarmonizationMachine/input/script.sh");

    // Write the bundle path into the script (.mag file has to be downloaded from https://github.com/magenta/magenta/tree/master/magenta/models/polyphony_rnn)
    system("echo 'BUNDLE_PATH=$HOME/Descargas/polyphony_rnn.mag' >> $HOME/HarmonizationMachine/input/script.sh");

    std::string bash = "echo '";
    std::string redirect = "' >> $HOME/HarmonizationMachine/input/script.sh";
    std::string command = bash + full_polyphony_rnn_command + redirect;
    const char *finalModelCommand = command.c_str();
    system(finalModelCommand);

    // Execute script.sh
    system("$HOME/HarmonizationMachine/input/script.sh");
  }
};