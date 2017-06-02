///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////// ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////// FOLEY by HPM /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// AN AMBIENT GENETIC PROGRAMME /////////////////////////////////////////////
//////////////////////////////// THAT BUILDS A MODEL BASED UPON IT'S SONIC ENVIRONMENT ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "ofApp.h"

//------------------------------------------------------------------
void ofApp::setup() 
{
	ofSetFrameRate(60);
	glPointSize(1);
	ofSetLineWidth(1);

	samples.assign(BUFFERSIZE, ofVec2f(0.0, 0.0));					//	Initialise the vector-array for incoming samples;
	soundStream.printDeviceList();									//	This will print a list of all the audio ports linked to the soundcard;
	soundStream.setDeviceID(1);										//	This should be set to the desired stereo input;
	soundStream.setup(this, 0, 2, SAMPLERATE, BUFFERSIZE, 4);		//	This initilises the incoming soundstream (see Mic.h);
	body.setup(0.22, 15.0);											//	Initalises the Body class (see Body.h);

	threshold = 0.08;												//	This sets a threshold to cut out potentially unwanted low volume sounds & noise;
	body.light = false;												//	Sets the colour mode: true = light mode / false = dark mode;
	body.alpha = false;												//	Sets the alpha mode: true = central position will have least alpha / vice versa;
}

//------------------------------------------------------------------
void ofApp::update() 
{
	body.mic.update(samples);										//	Update the Mic class with incoming sound;
	if (body.mic.calcRMS() > threshold) {							//	If the Root-Mean-Squared average volume (see Mic.cpp) is above the threshold, 
		body.incubate();											//	The genetic algorithm will be triggered and the model will be updated;
	}	
}

//------------------------------------------------------------------
void ofApp::draw() 
{
	ofBackground(20);												//	Off-black background;
	ofPushMatrix();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);			//	Translates to the center of the screen;
	ofRotate(30.0, 0.0, 1.0, 0.0);									//	Adds a slight offset rotation on the Y-axis for aesthetic reasons;
	ofPushMatrix();
	body.mesh.draw(OF_MESH_FILL);									//	Draw the Body class mesh which contains all the vertices for the model;
	ofPopMatrix();
	ofPopMatrix();
}

//------------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
	for (int i = 0; i < bufferSize; i++) {							//	Gets the incoming sound from the microphone;
		samples[i].x = input[i * 2] * 0.5;
		samples[i].y = input[i * 2 + 1] * 0.5;
	}
}
//------------------------------------------------------------------