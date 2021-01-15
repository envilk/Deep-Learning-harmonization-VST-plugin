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
  std::string inputPath;

public:
  void setInputPath(std::string p_inputPath)
  {
    inputPath = p_inputPath;
  }

  std::string selectModel(int id)
  {
    std::string model;
    switch (id)
    {
    case 2: model = "jazz_polyphony_rnn.mag"; break;
    default: model = "polyphony_rnn.mag";break;
    }
    return "echo 'BUNDLE_PATH=$HOME/Descargas/BUNDLES/"+model+"' >> $HOME/HarmonizationMachine/input/script.sh";
  }

  std::string selectKey(int id, bool primerKeyToggle)
  {
    std::string key;
    switch (id)
    {
    //Tonic chords (in order to estabilish a certain key for the harmonized melody)
    case 2: key = "[69,72,76]"; break;//La m
    case 3: key = "[67,70,74]"; break;//Sol m
    case 4: key = "[70,74,77]"; break;//Sib M
    case 5: key = "[68,71,75]"; break;//Lab m
    case 6: key = "[66,70,73]"; break;//Fa# M
    case 7: key = "[71,74,78]"; break;//Si m
    default: key = "[60,64,67]"; break; //Do M
    }
    if(primerKeyToggle) return " --primer_pitches=\""+key+"\" --condition_on_primer=true";
    else return " --primer_pitches=\""+key+"\" --condition_on_primer=false"; 
  }

  std::string numSteps(double steps)
  {
    return " --num_steps="+std::to_string(steps);
  }

  std::string numOutputs(double outputs)
  {
    return " --num_outputs="+std::to_string(outputs);
  }

  void generateHarmonization(double p_temperature, int p_modelId, int p_keyId, bool p_primerKeyToggle,
   bool p_harmonizeToggle, double p_numSteps, double p_numOutpus)
  {
    // Create script and magenta directories to execute everything
    system("mkdir $HOME/HarmonizationMachine");
    system("mkdir $HOME/HarmonizationMachine/input");
    system("mkdir $HOME/HarmonizationMachine/outputs");
    system("> $HOME/HarmonizationMachine/input/script.sh");
    system("chmod +x $HOME/HarmonizationMachine/input/script.sh");
    system("echo '#!/bin/bash' >> $HOME/HarmonizationMachine/input/script.sh");

    // Conversion from double to std::string
    std::ostringstream strs1;
    strs1 << p_temperature;
    std::string inputTemperature = " --temperature=" + strs1.str();
    //num-outputs

    std::string primerKeyString = selectKey(p_keyId, p_primerKeyToggle);

    std::string harmonizeString;
    if(p_harmonizeToggle) harmonizeString = " --inject_primer_during_generation=true";
    else p_harmonizeToggle = " --inject_primer_during_generation=false";

    std::string numStepsString = numSteps(p_numSteps);

    std::string numOutputsString = numSteps(p_numOutpus);

    // Building the magenta model command
    std::string polyphony_rnn_string_call_first_part = "polyphony_rnn_generate --bundle_file=${BUNDLE_PATH} --output_dir=$HOME/HarmonizationMachine/outputs --primer_midi=\"";
    std::string polyphony_rnn_string_call_second_part = "\""+harmonizeString+primerKeyString+numStepsString+numOutputsString;
    std::string full_polyphony_rnn_command = polyphony_rnn_string_call_first_part + inputPath + polyphony_rnn_string_call_second_part + inputTemperature;

    // Insert activate magenta enviroment command into the script
    system("echo 'eval \"$(conda shell.bash hook)\"' >> $HOME/HarmonizationMachine/input/script.sh");
    system("echo 'conda activate magenta' >> $HOME/HarmonizationMachine/input/script.sh");

    // Write the bundle path into the script (.mag file has to be downloaded from https://github.com/magenta/magenta/tree/master/magenta/models/polyphony_rnn)
    std::string bundleCommandString = selectModel(p_modelId);
    const char *bundleCommand = bundleCommandString.c_str();
    system(bundleCommand);

    std::string bash = "echo '";
    std::string redirect = "' >> $HOME/HarmonizationMachine/input/script.sh";
    std::string command = bash + full_polyphony_rnn_command + redirect;
    const char *finalModelCommand = command.c_str();
    system(finalModelCommand);

    // Execute script.sh
    system("$HOME/HarmonizationMachine/input/script.sh");
  }
};