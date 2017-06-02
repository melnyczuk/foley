// THIS CLASS HANDLES THE INCOMING SOUND DATA AND THE ALGORITHMS REGARDING THE GENERATION OF A GENETIC MODEL FROM THAT INPUT //

#pragma once
#include "ofMain.h"
#include "ofxEasyFft.h"							//	This addon handles the Fast Fourier Transform necessary to analyse the sound. I tried a number of FFT addons and this was the best I foudn for my needs;

#define BUFFERSIZE 512							//	This is the buffer size and is used to ensure no descrepencies anywhere where the buffer size is referenced;
#define SAMPLERATE 44100						//	This is the sample rate and is used to ensure no descrepencies anywhere where the sample rate is referenced;

class Mic
{
public:
	Mic();

	void setup(int _gain);
	void update(vector <ofVec2f> _samples);		//	Passes the data from the ofApp 'AudioIn' function;
	float calcRMS();
	float getBinAvg(int bin);
	float calcPan();
	
	vector <ofVec2f> samples;					//	I chose to use an vector-array of 2D float vectors to store both channels together for easy and concision;
	ofxEasyFft fft;
	float gain, old;
};