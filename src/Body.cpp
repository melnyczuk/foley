#include "Body.h"

Body::Body() {
	//mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);					//	Changing the drawing mode produces new an interesting representations of the programmes data, but the use of a Triangle Strip is most pleasing in my opinion;
}

//------------------------------------------------------------------
void Body::setup(float _mutationRate, float _gain)
{
	mesh.clear();													//	Clear the mesh;
	dna.clear();													//	Clear the vectors;
	point.clear();
	colour.clear();

	mic.setup(_gain);												//	Initalises the Mic class with additional gain (optional);
	mutationRate = _mutationRate;									//	Passes the mutation rate the the DNA class;
	light = false;													//	Initialises the colour mode (in this case dark is selected);
	alpha = false;													//	Initialises the alpha mode (in this case, alpha is less in more central positions);

	DNA _dna;
	_dna.setup(0.0);
	dna.assign(MAX_DNA, _dna);										//	Initialise the vector-arrays for the DNA, mesh points, and mesh colours,
	point.assign(MAX_DNA, ofPoint(0.0, 0.0, 0.0));					//	These are all set at zero, so the shape has no size or color to begin with;
	colour.assign(MAX_DNA, ofFloatColor(0.0));

	for (int d = 0; d < MAX_DNA; d++) {								//	Analyse all these values and pass them to the mesh;
		calcPhenotype(d);
		mesh.addVertex(point[d]);
		mesh.addIndex(d);
		mesh.addColor(colour[d]);
	}
}

//------------------------------------------------------------------
//	THIS FUNCTION COMBINES THE TWO SETS OF GENETIC DATA TO DECIDE HOW THE NEXT GENERATION OF GENES SHOULD BE PROPAGATED	//
//	Largely ripped from 'Generative Sculptures';
void Body::incubate()
{
	int d = int(ofRandom(dna.size()));								//	First an existing DNA-set is chosen at random;
	if (dna[d].fitness > mic.calcRMS()) {							//	If the fitness of that DNA-set is stronger than the current RMS volume,
		dna[d] = dna[d].crossover(micDNA());						//	Then it's genes are dominant in the genetic crossover calculation;
	}
	else {															//	If not,
		dna[d] = micDNA().crossover(dna[d]);						//	Then the genetic data of the incoming sound is dominant instead and a change will occur;
	}
	dna[d].mutate(mutationRate);									//	This creates some variance and is the main source of randomness in the aesthetic;
	calcPhenotype(d);												//	Calculate the new gene's phenotype;

	mesh.setVertex(d, point[d]);									//	Update the mesh vertex points to those generated from the DNA-set phenotype;
	mesh.setColor(d, colour[d]);									//	Update the mesh colour index to the new one generated from the DNA-set phenotype;
}

//------------------------------------------------------------------
void Body::calcPhenotype(int d)										//	Passes the reference to which DNA-set is being processed;
{
	ofPoint centroid = mesh.getCentroid();
	int x, y, z, w;
	float l, a;														// The below ?: statement allows the screen to be resized dynamically, without driving the shape off the edge;
	(ofGetWidth() > ofGetHeight()) ? w = ofGetHeight() : w = ofGetWidth();
	(centroid.x < 0) ? x = 1 : x = -1;								//	These three ?: statements ensure that the shape doesn't wander away from the center of the screen,
	(centroid.y < 0) ? y = 1 : y = -1;								//	They do so by flipping the direction to compensate for any net movement in the central position of the mesh;
	(centroid.z < 0) ? z = 1 : z = -1;
	(light == true) ? l = -0.5 : l = 0.5;							//	This allows the two colour modes;
	(alpha == true) ? a = -0.5 : a = 0.5;							//	This allows the two alpha modes;

	colour[d].a = ofMap(dna[d].genes[0], 0.0, 1.0, 0.5 - a, 0.5 + a, true);
	point[d].x = ofMap(dna[d].genes[1], 0.0, 1.0, 0.0, x * w * 0.35, true);
	point[d].y = ofMap(dna[d].genes[2], 0.0, 1.0, 0.0, y * w * 0.35, true);
	point[d].z = ofMap(dna[d].genes[3], 0.0, 1.0, 0.0, z * w * 0.35, true);
	colour[d].r = ofMap(dna[d].genes[4], 0.0, 1.0, 0.5 - l, 0.5 + l, true);
	colour[d].g = ofMap(dna[d].genes[5], 0.0, 1.0, 0.5 - l, 0.5 + l, true);
	colour[d].b = ofMap(dna[d].genes[6], 0.0, 1.0, 0.5 - l, 0.5 + l, true);
}

//	THIS FUNCTION TURNS THE FFT PROCESSED SOUND DATA INTO THE CORRECT GENETIC FORMAT TO BE BRED WITH THE EXISTING SHAPE	//
DNA Body::micDNA() {
	DNA dna;
	dna.genes.clear();
	for (int g = 0; g < NUM_GENES; g++) {
		float avg = mic.getBinAvg(10 * (g + 1));					//	By passing the 10 times the bin number, the programme analyses a larger section of the frequency data;
		dna.genes.push_back(avg);									//	The FFT processed data for this frequency section is then passed back as a float where the higher the amplitude of the sound, the stronger the gene;
	}
	dna.fitness = mic.calcRMS();									//	The dna-set's fitness is determined by the volume at the time it is created;
	return dna;
}