//	THIS FUNCTION COMBINES THE DATA OF THE INCOMING SOUND WITH THE EXISTING GENETIC DATA	//
//	FROM THIS IT IS ABLE TO DRAW A SHAPE THAT BOTH SHOWS LIVE CHANGES IN THE SONIC ENVIRONMENT AND A PAST HISTORY OF THAT ENVIRONMENT	//

#pragma once
#include "ofMain.h"
#include "Mic.h"
#include "DNA.h"

class Body
{
public:
	Body();
	void setup(float _muatationRate, float _gain);
	void incubate();
	void calcPhenotype(int d);
	DNA micDNA();
	
	Mic mic;
	ofMesh mesh;
	float mutationRate;
	bool light, alpha;

	vector <DNA> dna;
	vector <ofPoint> point;
	vector <ofFloatColor> colour;
};