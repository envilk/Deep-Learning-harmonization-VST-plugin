/*
  ==============================================================================

    InputDragAndDrop.h
    Created: 8 Jan 2021 1:04:27pm
    Author:  enrique

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PolyphonyRNN.h"
#define DEBUGPLUGIN

//==============================================================================
/*
*/
class InputDragAndDrop : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    InputDragAndDrop()
    {
    }

    ~InputDragAndDrop() override
    {
    }

    void setRNN(PolyphonyRNN polyphonyRNNInstance)
    {
        polyphonyRNN = polyphonyRNNInstance;
    }

    void paint(juce::Graphics &g) override
    {
        g.fillAll(juce::Colours::black);

        g.setColour(juce::Colours::white);
        g.setFont(15.0f);

        if(somethingIsBeingDragged)
        {
            g.setColour(juce::Colours::red);
            g.drawRect(getLocalBounds(), 3);
        }

        if (dragged)
        {
            g.fillAll(juce::Colours::red);
            g.drawText("MIDI file loaded", getWidth() / 2, getHeight() / 2, 100, 20, juce::Justification::centred, true);
        }
        else
        {
            g.drawText("Load a MIDI file", getWidth() / 2, getHeight() / 2, 100, 20, juce::Justification::centred, true);
        }
    }

    void resized() override
    {
    }
 
    bool isInterestedInFileDrag(const juce::StringArray &files) override
    {
        for (auto file : files)
        {
            if (file.contains(".mid"))
                return true;
        }
        return false;
    }

    void filesDropped(const juce::StringArray &files, int x, int y) override
    {
        for (auto file : files)
        {
            if (isInterestedInFileDrag(files))
            {
                polyphonyRNN.setInputPath(file.toStdString());
                activateDragged();
            }
        }
        somethingIsBeingDragged = false;
        repaint();
    }

    void activateDragged()
    {
        dragged = true;
    }

    void resetDragged()
    {
        dragged = false;
    }

    void fileDragEnter (const juce::StringArray&, int, int) override
    {
        somethingIsBeingDragged = true;
        repaint();
    }

    void fileDragExit (const juce::StringArray&) override
    {
        somethingIsBeingDragged = false;
        repaint();
    }

private:
    PolyphonyRNN polyphonyRNN;
    bool dragged = false;
    bool somethingIsBeingDragged = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputDragAndDrop)
};
