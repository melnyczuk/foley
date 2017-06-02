/* This was borrowed from the example "Genetic Sculpture" */

#include "DNA.h"

DNA::DNA(){
	genes.clear();
	fitness = ofRandom(0.5);
	setup();
}

//------------------------------------------------------------------
void DNA::setup()
{
	genes.assign(NUM_GENES, 0.0);
}

//------------------------------------------------------------------
void DNA::setup(float init)
{
	genes.clear();
	for (int i = 0; i < NUM_GENES; i++) {
		genes.push_back(init);
	}
}

//------------------------------------------------------------------
DNA DNA::crossover(DNA partner) {
    // A new child
    DNA child;
    child.setup(0.0);

    for (int i = 0; i < NUM_GENES; i++) {
		child.genes[i] = ofLerp(partner.genes[i], genes[i], fitness);			//	The algorithm determines how the new genes will be made up by anaylsing the fitness of the dominant gene;
		child.genes[i] = ofClamp(child.genes[i], 0.0, 1.0);						//	This ensures the gene does get out of bounds for some freakish reason;
    }
	fitness = ofRandom(1.0);													//	The newly bred gene is given a random fitness;
    return child;
}

//------------------------------------------------------------------
void DNA::mutate(float mutationRate) {											//	This is perhaps the only part of the programme that introduces any real randomness to the aesthetic/look;
    for (int i = 0; i < genes.size(); i++) {
      if (ofRandom(1) < mutationRate) {
        genes[i] += ofRandom(0.4, 0.6);											//	This limits the random variance of the gene to a small increment or decrement;
      }																			
   }
}
//------------------------------------------------------------------