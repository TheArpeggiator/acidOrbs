#include "ofApp.h"
#include <stdlib.h>


//--------------------------------------------------------------
// setup() -- Setup our application
//--------------------------------------------------------------
void ofApp::setup()
{
    //-AUDIO----------------------------------------------------

    // Setup the sound stream
    soundStream.setup(this,0, MY_CHANNELS, MY_SRATE, MY_BUFFERSIZE, MY_NBUFFERS);

    // Setup wavefile playback
    audioFile.load(ofToDataPath("wowWav.wav"));
    audioFile.setVolume(0.5f);
    audioFile.play();
    audioFile.setPaused(true);

    micOn = true;
    //playback = false;

    // Resize and initialize left and right buffers...
    left.resize( MY_BUFFERSIZE, 0 );
    right.resize( MY_BUFFERSIZE, 0 );
    leftTemp.resize( MY_BUFFERSIZE, 0 );
    rightTemp.resize( MY_BUFFERSIZE, 0 );
    lBins.resize( MY_BUFFERSIZE, 0 );
    rBins.resize( MY_BUFFERSIZE, 0 );

    // Resize and initialize left and right history buffers...
    leftHistory.resize(  MY_BUFFERHISTORY, left);
    rightHistory.resize( MY_BUFFERHISTORY, right);
    lBinHistory.resize( MY_BUFFERHISTORY, lBins);
    rBinHistory.resize( MY_BUFFERHISTORY, rBins);


    //-VIDEO----------------------------------------------------

    // Sync video refresh rate for our computer
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    // Set background color grayish
    ofBackground(0, 0, 0);

    // Tell OpenFrameworks to use smooth edges
    ofEnableSmoothing();

    // FFT setup
    leftFourier = ofxFft::create(MY_BUFFERSIZE, OF_FFT_WINDOW_HAMMING);
    rightFourier = ofxFft::create(MY_BUFFERSIZE, OF_FFT_WINDOW_HAMMING);

}

//--------------------------------------------------------------
void ofApp::update(){
    // Update audio buffer history with most recent buffer
    leftHistory.push_back(left);
    rightHistory.push_back(right);
    lBinHistory.push_back(lBins);
    rBinHistory.push_back(rBins);

    // Remove oldest buffers
    leftHistory.erase(  leftHistory.begin(),  leftHistory.begin()+1  );
    rightHistory.erase( rightHistory.begin(), rightHistory.begin()+1 );
    lBinHistory.erase(  lBinHistory.begin(),  lBinHistory.begin()+1  );
    rBinHistory.erase( rBinHistory.begin(), rBinHistory.begin()+1 );


    // Perform normalization for left channel
    float maxValue = 0;
    for(int i = 0; i < leftTemp.size(); i++)
    {
        if(abs(leftTemp[i]) > maxValue)
        {
            maxValue = abs(leftTemp[i]);
        }
    }
    for(int i = 0; i < leftTemp.size(); i++)
    {
        leftTemp[i] /= maxValue;
    }

    // Perform normalization for right channel
    maxValue = 0;
    for(int i = 0; i < rightTemp.size(); i++)
    {
        if(abs(rightTemp[i]) > maxValue)
        {
            maxValue = abs(rightTemp[i]);
        }
    }
    for(int i = 0; i < rightTemp.size(); i++)
    {
        rightTemp[i] /= maxValue;
    }

    // Set FFT buffer with left and right signals
    leftFourier->setSignal(&leftTemp[0]);
    rightFourier->setSignal(&rightTemp[0]);

    float *lFft = leftFourier->getAmplitude();
    float *rFft = rightFourier->getAmplitude();


    // Copy amplitude based FFT into bin buffers for left and right
    memcpy(&lBins[0],lFft,sizeof(float)*leftFourier->getBinSize());
    memcpy(&rBins[0],rFft,sizeof(float)*rightFourier->getBinSize());

    // Normalize for left bin buffer
    maxValue = 0;
    for(int i = 0; i < leftFourier->getBinSize(); i++)
        if(abs(lBins[i]) > maxValue)
            maxValue = abs(lBins[i]);

    for(int i = 0; i < leftFourier->getBinSize(); i++)
        lBins[i] /= (maxValue*20);

    // Normalize for right bin buffer
    maxValue = 0;
    for(int i = 0; i < rightFourier->getBinSize(); i++)
        if(abs(rBins[i]) > maxValue)
            maxValue = abs(rBins[i]);

    for(int i = 0; i < rightFourier->getBinSize(); i++)
        rBins[i] /= (maxValue*20);

}

//--------------------------------------------------------------
void ofApp::draw(){

    int wh = ofGetWindowHeight();
    int ww = ofGetWindowWidth();

    float waveform_amp   = wh * 0.25;
    float waveform_width = ww * 0.25;

    //Vicotria's Code --------------- From Here
    float pi_inc = (float)(2*MY_PIE) / MY_BUFFERSIZE;
    float xcorr = 0.0;
    float ycorr = 0.0;
    float total_radius = 0.0;
    float distance = 0.5;
    float disVar = 0.0;
    float disIntensity = 10.0;
    //------------------------------- To Here

    int r_col;
    int g_col;
    int b_col;

    //----------- -----------Top-Left Channel
    r_col = rand() % 255 + 0;
    g_col = rand() % 255 + 0;
    b_col = rand() % 255 + 0;

    ofPushStyle();
    ofPushMatrix();

        ofTranslate(0, wh, 0);

        ofSetLineWidth(1);
        ofNoFill();

        // Loop over the buffer history. Most recent is last, oldest is first.
        // Thus int j starts from max, and loops to zero
        for ( int j = MY_BUFFERHISTORY-1; j >= 0; j--)
        {
            // Each older buffer is further back by this amount
            ofTranslate( 5, 4, -5);

            // Randomized color
            ofSetColor(r_col, g_col, b_col, 256*(j/(float)MY_BUFFERHISTORY));

            // Start the line for this particular waveform
            ofBeginShape();
                // Make a vertex for each sample value
                for (unsigned int k = 0; k < MY_BUFFERSIZE; k++)
                {
                    disVar=(lBinHistory[j][k]*disIntensity) + distance;

                    total_radius = (waveform_width + waveform_amp * lBinHistory[j][k]) *disVar;

                    xcorr = total_radius * cos(k * pi_inc * 2) + ww *0.25; //Thanks
                    ycorr = total_radius * sin(k * pi_inc * 2) - wh *0.75; //Victoria!

                    ofVertex(xcorr, ycorr, 0);
                }
            // End line. False == Don't connect first and last points.
            ofEndShape(false);
        }

    ofPopMatrix();
    ofPopStyle();

    //-----------------------Bottom-Right Channel
    r_col = rand() % 255 + 0;
    g_col = rand() % 255 + 0;
    b_col = rand() % 255 + 0;

    ofPushStyle();
    ofPushMatrix();

        ofTranslate(0, wh, 0);

        ofSetLineWidth(1);
        ofNoFill();

        // Loop over the buffer history. Most recent is last, oldest is first.
        // Thus int j starts from max, and loops to zero
        for ( int j = MY_BUFFERHISTORY-1; j >= 0; j--)
        {
            // Each older buffer is further back by this amount
            ofTranslate( -5, -4, -5);

            // Randomized color
            ofSetColor(r_col, g_col, b_col, 256*(j/(float)MY_BUFFERHISTORY));

            // Start the line for this particular waveform
            ofBeginShape();
                // Make a vertex for each sample value
                for (unsigned int k = 0; k < MY_BUFFERSIZE; k++)
                {
                    disVar=(rBinHistory[j][k]*disIntensity) + distance;

                    total_radius = (waveform_width + waveform_amp * rBinHistory[j][k]) *disVar;

                    xcorr = total_radius * cos(k * pi_inc * 2) + ww *0.75;
                    ycorr = total_radius * sin(k * pi_inc * 2) - wh *0.25;

                    ofVertex(xcorr, ycorr, 0);
                }
            // End line. False == Don't connect first and last points.
            ofEndShape(false);
        }

    ofPopMatrix();
    ofPopStyle();

    //----------- -----------Bottom-Left Channel
    r_col = rand() % 255 + 0;
    g_col = rand() % 255 + 0;
    b_col = rand() % 255 + 0;

    ofPushStyle();
    ofPushMatrix();

        ofTranslate(0, wh, 0);

        ofSetLineWidth(1);
        ofNoFill();

        // Loop over the buffer history. Most recent is last, oldest is first.
        // Thus int j starts from max, and loops to zero
        for ( int j = MY_BUFFERHISTORY-1; j >= 0; j--)
        {
            // Each older buffer is further back by this amount
            ofTranslate( 5, -4, -5);

            // Randomized color
            ofSetColor(r_col, g_col, b_col, 256*(j/(float)MY_BUFFERHISTORY));

            // Start the line for this particular waveform
            ofBeginShape();
                // Make a vertex for each sample value
                for (unsigned int k = 0; k < MY_BUFFERSIZE; k++)
                {
                    disVar=(leftHistory[j][k]*disIntensity) + distance;

                    total_radius = (waveform_width + waveform_amp * leftHistory[j][k]) *disVar;

                    xcorr = total_radius * cos(k * pi_inc) + ww *0.25;
                    ycorr = total_radius * sin(k * pi_inc) - wh *0.25;

                    ofVertex(xcorr, ycorr, 0);
                }
            // End line. False == Don't connect first and last points.
            ofEndShape(false);
        }

    ofPopMatrix();
    ofPopStyle();

    //-----------------------Top-Right Channel
    r_col = rand() % 255 + 0;
    g_col = rand() % 255 + 0;
    b_col = rand() % 255 + 0;

    ofPushStyle();
    ofPushMatrix();

        ofTranslate(0, wh, 0);

        ofSetLineWidth(1);
        ofNoFill();

        // Loop over the buffer history. Most recent is last, oldest is first.
        // Thus int j starts from max, and loops to zero
        for ( int j = MY_BUFFERHISTORY-1; j >= 0; j--)
        {
            // Each older buffer is further back by this amount
            ofTranslate( -5, 4, -5);

            // Randomized color
            ofSetColor(r_col, g_col, b_col, 256*(j/(float)MY_BUFFERHISTORY));

            // Start the line for this particular waveform
            ofBeginShape();
                // Make a vertex for each sample value
                for (unsigned int k = 0; k < MY_BUFFERSIZE; k++)
                {
                    disVar=(rightHistory[j][k]*disIntensity) + distance;

                    total_radius = (waveform_width + waveform_amp * rightHistory[j][k]) *disVar;

                    xcorr = total_radius * cos(k * pi_inc) + ww *0.75;
                    ycorr = total_radius * sin(k * pi_inc) - wh *0.75;

                    ofVertex(xcorr, ycorr, 0);
                }
            // End line. False == Don't connect first and last points.
            ofEndShape(false);
        }

    ofPopMatrix();
    ofPopStyle();
}


//--------------------------------------------------------------
// audioIn() -- deal with incoming audio buffer
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{

    // Write incoming audio to buffer. Note: samples are interleaved.
    if(micOn)
    {
        // Store incomping input into buffers
        for (int i = 0; i < bufferSize; i++)
        {            
            left[i]         = input[i*2];       //unmodulated buffer
            right[i]        = input[i*2+1];     //unmodulated buffer
            leftTemp[i]		= input[i*2];          //buffer for FFT
            rightTemp[i]	= input[i*2+1];         //buffer for FFT
        }
    }

}

void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if(key == 32)
    {
        playback = !playback;
        if(playback)
            audioFile.setPaused(false);
        else
            audioFile.setPaused(true);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
