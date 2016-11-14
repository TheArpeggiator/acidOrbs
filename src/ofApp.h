#pragma once

#include "ofMain.h"
#include "ofxFft.h"

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

    // Get buffer samples
    void audioIn(float * input, int bufferSize, int nChannels);

private:
    // Our sound stream object
    ofSoundStream soundStream;

    // FFT object
    ofxFft *leftFourier, *rightFourier;

    // Vectors for our left- and right-channel waveforms
    vector<float> left;
    vector<float> right;
    vector<float> leftTemp;
    vector<float> rightTemp;
    vector<float> lBins;
    vector<float> rBins;

    // Double vectors to save previous waveforms, for plotting in the waterfall
    vector< vector<float> > leftHistory;
    vector< vector<float> > rightHistory;
    vector< vector<float> > lBinHistory;
    vector< vector<float> > rBinHistory;

    // Audio file playback/mic input toggles
    bool playback;
    bool micOn;

    // Object for sound playback
    ofSoundPlayer audioFileL, audioFileR;

    // Size of Orbs
    float orbSizeF, orbSizeM;

    // RGB Values
    int r_col;
    int g_col;
    int b_col;
};
