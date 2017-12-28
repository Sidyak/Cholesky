/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
  Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
  Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/


#include <Bela.h>
#include <cmath>

float gPhase,gPhaseMod;
float gInverseSampleRate;
int gAudioFramesPerAnalogFrame;

// Set the analog channels to read from
int gSensorInputFrequency = 0;
int gSensorInputAmplitude = 1;

bool setup(BelaContext *context, void *userData)
{
	
	// Check if analog channels are enabled
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
		rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
		return false;
	}

	// Check that we have the same number of inputs and outputs.
	if(context->audioInChannels != context->audioOutChannels ||
			context->analogInChannels != context-> analogOutChannels){
		printf("Error: for this project, you need the same number of input and output channels.\n");
		return false;
	}

	// Useful calculations
	gAudioFramesPerAnalogFrame = context->audioFrames / context->analogFrames;
	gInverseSampleRate = 1.0 / context->audioSampleRate;
	gPhase = 0.0;
	gPhaseMod = 0.0;

	return true;
}

void render(BelaContext *context, void *userData)
{
	float frequency = 440.0;
	float amplitude = .998;

	// There are twice as many audio frames as matrix frames since audio sample rate
	// is twice as high

	for(unsigned int n = 0; n < context->audioFrames; n++) {
		if(!(n % gAudioFramesPerAnalogFrame)) {
			// On even audio samples: read analog inputs and update frequency and amplitude
			frequency = map(analogRead(context, n/gAudioFramesPerAnalogFrame, gSensorInputFrequency), 0, 1, 0, 1000);
			//amplitude = analogRead(context, n/gAudioFramesPerAnalogFrame, gSensorInputAmplitude);
		}

        float sine1 = 0.99*sinf(gPhaseMod);
		float sine2 = amplitude * sinf(gPhase);
        float out = 0.70711-sqrtf(1.0+sine2) * sine1;
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			audioWrite(context, n, channel, out);
		}

		// Update and wrap phase of sine tone
		gPhase += 2.0 * M_PI * frequency * gInverseSampleRate;
		if(gPhase > 2.0 * M_PI){
			//printf("phase=%f\tfrequency=%f\tgInverseSampleRate=%f\n",gPhase, frequency, gInverseSampleRate);
			gPhase -= 2.0 * M_PI;
		}
		
		gPhaseMod += 2.0 * M_PI * 440.0 * gInverseSampleRate;
		if(gPhaseMod > 2.0 * M_PI){
			gPhaseMod -= 2.0 * M_PI;
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{

}


/**
\example analog-input/render.cpp

Connecting potentiometers
-------------------------

This sketch produces a sine tone, the frequency and amplitude of which are 
modulated by data received on the analog input pins. Before looping through each audio 
frame, we declare a value for the `frequency` and `amplitude` of our sine tone; 
we adjust these values by taking in data from analog sensors (for example potentiometers)
with `analogRead()`.

- connect a 10K pot to 3.3V and GND on its 1st and 3rd pins.
- connect the 2nd middle pin of the pot to analogIn 0.
- connect another 10K pot in the same way but with the middle pin connected to analogIn 1.

The important thing to notice is that audio is sampled twice as often as analog 
data. The audio sampling rate is 44.1kHz (44100 frames per second) and the 
analog sampling rate is 22.05kHz (22050 frames per second). Notice that we are 
processing the analog data and updating frequency and amplitude only on every 
second audio sample, since the analog sampling rate is half that of the audio.

````
if(!(n % gAudioFramesPerAnalogFrame)) {
    // Even audio samples: update frequency and amplitude from the matrix
    frequency = map(analogRead(context, n/gAudioFramesPerAnalogFrame, gSensorInputFrequency), 0, 1, 100, 1000);
    amplitude = analogRead(context, n/gAudioFramesPerAnalogFrame, gSensorInputAmplitude);
}
````

*/
