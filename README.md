acidOrbs
=========

Use of the openFrameworks tool to create an audio visual experience

Description
---------

Complete overhaul from previous ideas. Basically has the waveform of incoming audio through line-inputs wrapped around a circle. (Not really though). The older buffers are maintained and are drawn as a trail behind the live buffer. The FFT of the incoming signal has also been represented (although it has been combined wiht the raw buffer just to make it look pretty).

Usage
-----------------------------

Copy project folder to myApps in openFrameworks directory. (Requires ofxFft addon!)

Controls are as follows --

1. Reset orbs using 		-: o
2. Color changes using 		-: r
3. Change orb shape using 	-: p
4. Toggle mic input using	-: m

(Reference audio playback using 'space')

See it in action here -- https://youtu.be/76zc5309BrE


Author
-------

Rahul Agnihotri (ragni@ccrma.stanford.edu)