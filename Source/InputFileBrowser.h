/*
  ==============================================================================

    InputFileBrowser.h
    Created: 9 Jan 2021 1:34:56pm
    Author:  enrique

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class InputFileBrowser  : public juce::Component//, private juce::FileBrowserListener
{
public:

    InputFileBrowser()
    {
    }

    ~InputFileBrowser() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (juce::Colours::white);
        g.setFont (14.0f);
        g.drawText ("InputFileBrowser", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
    }

private:
    //juce::DirectoryContentsList directoryList {nullptr, thread};
    //juce::FileTreeComponent fileTreeComp {directoryList};
    //juce::Label explanation { {}, "Select a MIDI file in the treeview above" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputFileBrowser)
};
