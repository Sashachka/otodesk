/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 9 Jun 2026 6:23:53pm
    Author:  Sasha Shkurnikova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");
    

    tableComponent.getHeader().addColumn("Track Name", 1, 400 );
    tableComponent.getHeader().addColumn("Play Buttons", 2, 200);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0, 0 , getWidth(), getHeight());
}


int PlaylistComponent::getNumRows ()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground (juce::Graphics& g,
                                           int rowNumber,
                                           int width, int height,
                                           bool rowIsSelected) 
{
    if (rowIsSelected)
        g.fillAll (juce::Colours::orange);
    else
        g.fillAll (juce::Colours::darkgrey);
}

void PlaylistComponent::paintCell (juce::Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width, int height,
                                  bool rowIsSelected) 
{
    g.setColour (juce::Colours::black);
    g.setFont (juce::FontOptions (14.0f));

    switch (columnId)
    {
        case 1:
            g.drawText (trackTitles[rowNumber], 2, 0, width-4, height, juce::Justification::left, true);
            break;
        case 2:
            g.drawText ("Artist " + std::to_string(rowNumber + 1), 2, 0, width-4, height, juce::Justification::left, true);
            break;
    }
}

Component* PlaylistComponent::refreshComponentForCell (int rowNumber,  
                                         int columnId,
                                         bool isRowSelected,
                                        Component* existingComponentToUpdate)
{
  if(columnId == 2)
  {
      if(existingComponentToUpdate == nullptr)
      {
          TextButton* btn = new TextButton{"play"};
          String id{std::to_string(rowNumber)};
          btn->setComponentID(id);
          btn->addListener(this);
          existingComponentToUpdate = btn;
      }
      
  }
  return existingComponentToUpdate;
}

void  PlaylistComponent::buttonClicked(Button* button)
{
  int id = std::stoi(button -> getComponentID().toStdString());
    if(button->getButtonText() == "play")
    {
      std::cout << "Play button clicked!" << trackTitles[id] << std::endl;
        // Handle play button click
    }
}