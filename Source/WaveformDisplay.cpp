/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);

  // hint to the user that the waveform can be clicked to scrub
  setMouseCursor(MouseCursor::PointingHandCursor);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::orange);
    if(fileLoaded)
    {
      audioThumb.drawChannel(g,
        getLocalBounds(),
        0,
        audioThumb.getTotalLength(),
        0,
        1.0f
      );

      // shade the part of the track that has already played
      g.setColour(Colours::white.withAlpha(0.15f));
      g.fillRect(0.0f, 0.0f, (float)(position * getWidth()), (float)getHeight());

      // thin playhead line so it is clear exactly where we are
      g.setColour(Colours::lightgreen);
      g.fillRect((float)(position * getWidth()) - 1.0f, 0.0f, 2.0f, (float)getHeight());
    }
    else
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text

    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }


}

void WaveformDisplay::mouseDown (const MouseEvent& event)
{
  scrubToMouse(event);
}

void WaveformDisplay::mouseDrag (const MouseEvent& event)
{
  scrubToMouse(event);
}

void WaveformDisplay::scrubToMouse (const MouseEvent& event)
{
  // nothing to scrub through until a track is loaded
  if (!fileLoaded || getWidth() <= 0)
  {
    return;
  }

  // turn the x pixel the user clicked into a 0-1 position along the track
  double pos = (double) event.position.x / (double) getWidth();
  pos = jlimit(0.0, 1.0, pos);

  setPositionRelative(pos);

  // tell the DeckGUI so it can move the actual player
  if (onPositionChanged)
  {
    onPositionChanged(pos);
  }
}




