/*
  ==============================================================================

    MidiProcessor.h
    Created: 29 Dec 2020 12:05:20pm
    Author:  enrique

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#define DEBUGPLUGIN

class MidiProcessor
{

private:
    std::unique_ptr<juce::FileLogger> m_flogger;

    juce::MidiFile midiFile;
    juce::MidiMessageSequence mms;
    juce::MidiMessage tempoEvent;
    double startTime;
    double msPerTick;
    double timeStampInMS;
    int microsecondsPerQuarter;
    int counter = 0;

public:
    const std::string inputPath = "/home/enrique/inputMelody.mid";

    MidiProcessor()
    {
#ifdef DEBUGPLUGIN
        m_flogger = std::unique_ptr<juce::FileLogger>(juce::FileLogger::createDateStampedLogger("/home/enrique", "mylog", ".txt", "Welcome to plugin"));
#endif
    }

    void process(juce::MidiBuffer &p_midiMessages, bool p_isRecording, float p_tempo)
    {
        if (p_isRecording)
        {
            counter++;
            if (counter < 2) // First event of the recorded sequence in the Piano roll
            {
                startTimer(p_tempo);
            }

            for (const juce::MidiMessageMetadata metadata : p_midiMessages)
            {
                fillMidiMessagesSequence(metadata);

#ifdef DEBUGPLUGIN
                if (m_flogger)
                {
                    m_flogger->logMessage(currentMessage.getDescription());
                }
#endif
            }
        }
        else if (counter > 0) // Last event of the recorded sequence in the Piano roll, and Midi file generation
        {
            counter = 0;

            createMidiFile(p_tempo);
        }
    }

    void startTimer(int p_tempo)
    {
        startTime = juce::Time::getMillisecondCounterHiRes();
        msPerTick = (60000.f / p_tempo) / 960.f; //960 ticks per quarternote
    }

    void fillMidiMessagesSequence(juce::MidiMessageMetadata metadata)
    {
        auto currentMessage = metadata.getMessage();

        timeStampInMS = juce::Time::getMillisecondCounterHiRes() - startTime;
        currentMessage.setTimeStamp(timeStampInMS / msPerTick);

        mms.addEvent(currentMessage);
    }

    void createMidiFile(float p_tempo)
    {
        microsecondsPerQuarter = (60000.f / p_tempo) * 1000.f;
        tempoEvent = juce::MidiMessage::tempoMetaEvent(microsecondsPerQuarter);
        tempoEvent.setTimeStamp(0);
        mms.addEvent(tempoEvent);
        mms.updateMatchedPairs();
        mms.sort();

        midiFile.setTicksPerQuarterNote(960);
        midiFile.addTrack(mms);

        //Writing all to the outputStream
        juce::File file(inputPath);
        if (file.exists())
            file.replaceFileIn(file);
        juce::FileOutputStream stream(file);
        midiFile.writeTo(stream, 1);
        stream.flush();

        mms.clear();
        midiFile.clear();
    }
};
