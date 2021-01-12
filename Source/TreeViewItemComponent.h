/*
  ==============================================================================

    TreeViewItemComponent.h
    Created: 12 Jan 2021 11:09:49am
    Author:  enrique

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TreeViewItemComponent : public juce::Component
{
public:
    TreeViewItemComponent(juce::TreeViewItem* p_creatorItem, juce::File p_dragableFile)
    {
        creatorItem = p_creatorItem;
        dragableFile = p_dragableFile;
    }

    ~TreeViewItemComponent() override
    {
    }

    void paint(juce::Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background

        g.setColour(juce::Colours::grey);
        g.drawRect(getLocalBounds(), 1); // draw an outline around the component

        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        g.drawText("TreeViewItemComponent", getLocalBounds(),
                   juce::Justification::centred, true); // draw some placeholder text
    }

    void resized() override
    {
    }

    void mouseDown(const juce::MouseEvent &e) override
    {
        m_flogger->logMessage("DOWN");
        componentBeingDragged.startDraggingComponent(this, e);
    }

    void mouseDrag(const juce::MouseEvent &e) override
    {
        m_flogger->logMessage("DRAG");
        componentBeingDragged.dragComponent(this, e, nullptr);
    }

    void mouseUp(const juce::MouseEvent &e) override
    {
        m_flogger->logMessage("DOWN");
        juce::DragAndDropContainer::performExternalDragDropOfFiles(dragableFile.getFullPathName(), false, nullptr);
    }

private:

    juce::ComponentDragger componentBeingDragged;
    juce::TreeViewItem* creatorItem;

    juce::File dragableFile;

    std::unique_ptr<juce::FileLogger> m_flogger = std::unique_ptr<juce::FileLogger>(juce::FileLogger::createDateStampedLogger("/home/enrique", "TREE VIEW ITEM", ".txt", "Welcome to plugin"));

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreeViewItemComponent)
};
