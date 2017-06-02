// THIS CLASS HANDLES THE BREEDING OF GENES AND THE GENERATION OF NEW CODE //
/* This class was began as the example "Genetic Sculpture" */

#pragma once
#include "ofMain.h"

#define MAX_DNA 128				//	This can vary, and it affects the number of points the program draws;
#define NUM_GENES 7					//	This is linked to the number of parameters the programme body has;

class DNA{

	public:
		DNA();
		void setup();
		void setup(float init);
        DNA crossover(DNA partner);
        void mutate(float mutationRate);

		vector <float> genes;
        float fitness;
        float mutationAmount;
};
