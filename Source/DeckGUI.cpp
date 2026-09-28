/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);

    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);

    addAndMakeVisible(waveformDisplay);

    // name each slider so the user can tell them apart
    volLabel.setText("Volume", dontSendNotification);
    speedLabel.setText("Speed", dontSendNotification);

    volLabel.setJustificationType(Justification::centredLeft);
    speedLabel.setJustificationType(Justification::centredLeft);

    // clicking or dragging on the waveform scrubs the track
    waveformDisplay.onPositionChanged = [this](double pos)
    {
        player->setPositionRelative(pos);
    };


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);


    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 5.0);   // never 0 -> resampling ratio of 0 = silence

    // Give the sliders sensible starting values so the player isn't muted/stopped
    volSlider.setValue(1.0);          // full volume
    speedSlider.setValue(1.0);        // normal speed

    startTimer(50);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{
    int rowH = getHeight() / 8;
    int labelW = 70;                    // room at the left of each slider row for its name
    int sliderW = getWidth() - labelW;

    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);

    volLabel.setBounds(0, rowH * 2, labelW, rowH);
    volSlider.setBounds(labelW, rowH * 2, sliderW, rowH);

    speedLabel.setBounds(0, rowH * 3, labelW, rowH);
    speedSlider.setBounds(labelW, rowH * 3, sliderW, rowH);

    // the waveform doubles as the position control, so give it the space
    // the old position slider used to take
    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH * 3);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
       auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
                loadFile(URL{chosenFile});
            }
        });
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    loadFile(URL{File{files[0]}});
  }
}

void DeckGUI::loadFile(URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;

    // don't move the playhead out from under the user while they are scrubbing
    if (!waveformDisplay.isMouseButtonDown())
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}


    

