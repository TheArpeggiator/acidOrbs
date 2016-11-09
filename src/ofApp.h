#pragma once

#include "ofMain.h"

//-----------------------------------------------------------------------------
// Preprocessor definitions
//-----------------------------------------------------------------------------
#define MY_SRATE         44100            // sample rate
#define MY_CHANNELS      2                // number of channels
#define MY_BUFFERHISTORY 50               // number of buffers to save
#define MY_BUFFERSIZE    512              // number of frames in a buffer
#define MY_NBUFFERS      2                // number of buffers latency
#define MY_PIE           3.14159265358979 // for convenience


//-----------------------------------------------------------------------------
// My application
//-----------------------------------------------------------------------------
class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // Get buffer samples
    void audioIn(float * input, int bufferSize, int nChannels);

private:
    // Our sound stream object
    ofSoundStream soundStream;

    // Vectors for our left- and right-channel waveforms
    vector<float> left;
    vector<float> right;

    // Double vectors to save previous waveforms, for plotting in the waterfall
    vector< vector<float> > leftHistory;
    vector< vector<float> > rightHistory;
};
