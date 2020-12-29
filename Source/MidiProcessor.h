/*
  ==============================================================================

    MidiProcessor.h
    Created: 29 Dec 2020 12:05:20pm
    Author:  enrique

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#DEBUGPLUGIN

class MidiProcessor
{

private:
    juce::MidiBuffer processedBuffer;

public:
    MidiProcessor()
    {
#ifdef DEBUGPLUGIN
        m_flogger = std::unique_ptr<juce::FileLogger>(juce::FileLogger::createDateStampedLogger("/home/enrique", "mylog", ".txt", "Welcome to plugin"));
#endif
    }

    void process(juce::MidiBuffer &midiMessages)
    {
        processedBuffer.clear();

        for (const juce::MidiMessageMetadata metadata : midiMessages)
        {

#ifdef DEBUGPLUGIN
            if (m_flogger)
            {
                m_flogger->logMessage(currentMessage.getDescription());
            }
#endif

            auto currentMessage = metadata.getMessage();

            if (currentMessage.isNoteOnOrOff())
            {
                auto transposedMessage = currentMessage;

                auto oldNoteNum = transposedMessage.getNoteNumber();
                transposedMessage.setNoteNumber(oldNoteNum + 3);

                processedBuffer.addEvent(transposedMessage, metadata.samplePosition);
            }
            processedBuffer.addEvent(currentMessage, metadata.samplePosition);
        }

        midiMessages.swapWith(processedBuffer);
    }
};
