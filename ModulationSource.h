#pragma once
#include "Stk.h"
#include "FileWvIn.h"
#define CIRCULAR_BUFFER_SIZE 5
#define USE_MATH_DEFINES

class ModulationSource
{
private:
	stk::StkFrames audio_data;
	// The parameters for the constructor represent the high and low limits of the bandpass filter that will be
	// applied to the signal. dry_audio is the unaffected signal. The constructor will prefilter the audio according
	// to the numbers low and high. For example: if low == 500 and high == 1500, then the dry_audio will be filtered
	// so that there is only audio between 500Hz and 1500Hz. frame_rate is the number of video frames in a second.
	ModulationSource(int low, int high, stk::StkFrames dry_audio, int frame_rate);

	// Recorded for further use.
	int low;
	int high;
	int num_samples;
	int sample_rate;		// Samples per second
	int frame_rate;
	int samples_per_frame;	// The number of samples used to calculate modulation for one frame.

	// The constructor will call this function, and should not need to be called again.
	void filterAudio();

	// Stores the 5 previous values which have been generated by getModForTimeStep. By using an average
	// of the previous values, the result is a smoother look.
	float circular_buffer[CIRCULAR_BUFFER_SIZE];
	void update_circ_buff(float newest_mod_value);

	// This is only included because C++ requires a default constructor make a 'new' array of objects
	// and should not be called.
	ModulationSource();
public:

	// This is a pseudo-factory method for initializing num_freq_bands ModulationSources. The
	// filename parameter is the location of the .wav file that will be used as input. num_freq_bands
	// is the number of bands that we are going to break our signal into. freq_bands is an array that
	// holds the crossover points between frequency bands
	static ModulationSource* initModSource(std::string filename, int num_freq_bands, int* freq_bands, int frame_rate);

	// This function takes an integer, representing the current time step of the animation, and returns a float
	// between 0 and 1 which represents the intensity of the modulation at that instant
	float getModForTimeStep(int time_step);


	~ModulationSource(void);
};

