#include "ofApp.h"
#include <stdlib.h>


//--------------------------------------------------------------
// setup() -- Setup our application
//--------------------------------------------------------------
void ofApp::setup(){
    //-AUDIO----------------------------------------------------

    // Setup the sound stream
    soundStream.setup(this, 0, MY_CHANNELS, MY_SRATE, MY_BUFFERSIZE, MY_NBUFFERS);

    // Resize and initialize left and right buffers...
    left.resize( MY_BUFFERSIZE, 0 );
    right.resize( MY_BUFFERSIZE, 0 );

    // Resize and initialize left and right history buffers...
    leftHistory.resize(  MY_BUFFERHISTORY, left  );
    rightHistory.resize( MY_BUFFERHISTORY, right );


    //-VIDEO----------------------------------------------------

    // Sync video refresh rate for our computer
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    //ofToggleFullscreen();

    // Set background color grayish
    ofBackground(0, 0, 0);

    // Tell OpenFrameworks to use smooth edges
    ofEnableSmoothing();

}

//--------------------------------------------------------------
void ofApp::update(){
    // Update audio buffer history with most recent buffer
    leftHistory.push_back( left );
    rightHistory.push_back( right );

    // Remove oldest buffers
    leftHistory.erase(  leftHistory.begin(),  leftHistory.begin()+1  );
    rightHistory.erase( rightHistory.begin(), rightHistory.begin()+1 );
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
                    disVar=(leftHistory[j][k]*disIntensity) + distance;

                    total_radius = (waveform_width + waveform_amp * leftHistory[j][k]) *disVar;

                    xcorr = total_radius * cos(k * pi_inc) + ww *0.25; //Thanks
                    ycorr = total_radius * sin(k * pi_inc) - wh *0.75; //Victoria!

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
                    disVar=(rightHistory[j][k]*disIntensity) + distance;

                    total_radius = (waveform_width + waveform_amp * rightHistory[j][k]) *disVar;

                    xcorr = total_radius * cos(k * pi_inc) + ww *0.75;
                    ycorr = total_radius * sin(k * pi_inc) - wh *0.25;

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
void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    // Write incoming audio to buffer. Note: samples are interleaved.
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2];
        right[i]	= input[i*2+1];
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
