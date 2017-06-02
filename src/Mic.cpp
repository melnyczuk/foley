#include "Mic.h"
Mic::Mic() {
	samples.clear();
}

//------------------------------------------------------------------
void Mic::setup(int _gain) {
	samples.assign(BUFFERSIZE, ofVec2f(0.0, 0.0));						//	Creates fresh array for holding sample data;
	fft.setup(BUFFERSIZE);
	gain = _gain;
	old = 0.0;
}

//------------------------------------------------------------------
//	PASSES THE SAMPLES FROM THE OFAPP AUDIOIN FUNCTION //
void Mic::update(vector <ofVec2f> _samples) {
	for (int i = 0; i < BUFFERSIZE; i++) {
		samples[i] = _samples[i];
	}
	fft.update();
}

//------------------------------------------------------------------
// THIS FUNCTION CALCULATES THE ROOT-MEAN-SQUARED AVERAGE OF ALL THE SOUND DATA AT ANY GIVEN TIME	//
// THIS IS USED TO GET A SENSE OF THE OVERALL TOTAL VOLUME AT ANY GIVEN POINT IN TIME	//
float Mic::calcRMS() 
{
	float rms = 0.0;
	int numCounted = 0;
	for (int i = 0; i < BUFFERSIZE; i++) {
		rms += samples[i].x * samples[i].x + samples[i].y + samples[i].y;
		numCounted += 2;
	}
	rms /= (float)numCounted;
	rms *= gain/2;
	rms = sqrt(abs(rms));
	rms = ofClamp(rms, 0.0, 1.0);
	return rms;
}

//------------------------------------------------------------------
//	THIS FUNCTION GETS THE AVERAGE AMPLITUDE OF ANY GIVEN 'BIN' OF FREQUENCIES	//
//	THE LOWER THE BIN NUMBER, THE LOWER THE FREQUENCY BAND BEING ANAYLSED	//
float Mic::getBinAvg(int bin) 
{
	float bAvg = 0;
	for (int w = 0; w < bin; w++) {									//	By using the passed bin number itself to get the width of the bin, I hoped to account for the logarymthic progression of higher frequencies.
		bAvg += fft.getBins()[bin + w];								//	This is because lower octaves of sound are smaller than higher octaves of sound.
	}																//	However the FFT process does not account for this, which can produce a disproportionate over analysis of bass frequencies;
	bAvg /= bin*1.01;
	bAvg *= gain;
	bAvg = ofClamp(bAvg, 0.0, 1.0);
	if (bAvg < calcRMS() * 2) {										//	If I'm honest, I am not sure why this was necessary. But without this thresholding, the figures were totally wrong.
		return bAvg;												//	I hope to research this further as part of my plans to improve the overal implementation of the FFT processing;
	}
	else return 0.0;
}

//------------------------------------------------------------------
//	THIS FUNCTION CALCULATES THE STEREO PAN OF THE INCOMING SOUND AND WILL BE USED IN FUTURE IMPLEMENTATIONS //
//	IT USES A SIMILAR ALGORITHM TO THE RMS FUNCTION ABOVE	//
float Mic::calcPan()													
{
	float pan = 0.0;
	float l = 0.0;
	float r = 0.0;
	int numCounted = 0;
	for (int i = 0; i < BUFFERSIZE; i++) {
		pan += samples[i].x;
		pan -= samples[i].y;
	}
	pan = 0.1 * pan + (1 - 0.1) * old;
	old = pan;
	return pan;
}