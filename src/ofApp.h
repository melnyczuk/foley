#pragma once
#include "ofMain.h"
#include "Body.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void audioIn(float * input, int bufferSize, int nChannels);

		ofSoundStream soundStream;
		vector <ofVec2f> samples;
		Body body;
		float threshold;
		int gain;
		float rms;
		float t, p;
};
