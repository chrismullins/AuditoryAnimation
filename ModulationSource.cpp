#include "ModulationSource.h"


ModulationSource::ModulationSource(int low, int high, stk::StkFrames dry_audio_signal, int frame_rate)
{
	(*this).low = low;
	(*this).high = high;
	(*this).audio_data = dry_audio_signal;
	filterAudio();
	(*this).num_samples = dry_audio_signal.size();
	(*this).sample_rate = dry_audio_signal.dataRate();
	(*this).frame_rate = frame_rate;
	(*this).samples_per_frame = sample_rate / frame_rate;
	for (int i = 0; i < CIRCULAR_BUFFER_SIZE; i++)
		circular_buffer[i] = 0;
}

ModulationSource* ModulationSource::initModSource(std::string filename, int num_freq_bands, int* crossover_pts, int frame_rate)
{
	// Load the audio file
	stk::FileWvIn input = stk::FileWvIn(filename, false, true, 100000000, 1024);
	int num_samples = input.getSize();

	//This centers the audio data around the point 0
	input.normalize();
	// I am assuming that the numbers stored in crossover_pts are stored in increasing order
	ModulationSource* to_return = new ModulationSource[num_freq_bands];

	for (int i = 0; i < num_freq_bands; i++)
	{
		stk::StkFrames audio_buffer = stk::StkFrames(num_samples, 2);
		input.tick(audio_buffer); // This stores the audio buffer located in the FileWvIn into audio_buffer
		// Now let the constructor handle the initialization and filtering of the audio_data
		to_return[i] = ModulationSource(crossover_pts[i], crossover_pts[i + 1], audio_buffer, frame_rate);
	}

	return to_return;
}


ModulationSource::~ModulationSource(void)
{
}

ModulationSource::ModulationSource()
{
}

void ModulationSource::filterAudio()
{
	// We want to filter the data so that the only frequency components between 'low' Hz and 'high' Hz remain
	// When this was written, 3/29, James did not have audio effect book, the following was copied from a different program.
	// James will fix soon:
	//float a0;
	//float b1;
	//float gamma;
	//float theta;
	//float fs = 44100.0;
	//float fc = 1000.0;
	//
	//theta = (2 * M_PI * fc) / fs;
	//gamma = 2 - cos(theta);
	//b1 = sqrt(pow(gamma, 2.0) - 1) - gamma;
	//a0 = 1 + b1;
	//
	//for (int i = 1; i < audio_data.frames; i++)
	//{
	//
	//}

}

float ModulationSource::getModForTimeStep(int time_step)
{
	// First we need to figure out which samples we can use for 
	float sum = 0;
	int offset = time_step * samples_per_frame;

	for (int i = 0; i < samples_per_frame; i++)
		sum += abs(audio_data[i + offset]);
	
	update_circ_buff((float) sum / samples_per_frame);
	float to_return = 0;
	for (int i = 0; i < CIRCULAR_BUFFER_SIZE; i++)
	{
		to_return += circular_buffer[i];
	}
	return (float) to_return / CIRCULAR_BUFFER_SIZE;
}

void ModulationSource::update_circ_buff(float newest_mod_value)
{
	for (int i = 0; i < CIRCULAR_BUFFER_SIZE - 1; i++)
	{
		circular_buffer[i] = circular_buffer[i + 1];
	}
	circular_buffer[CIRCULAR_BUFFER_SIZE - 1] = newest_mod_value;
}