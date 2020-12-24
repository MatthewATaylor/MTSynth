#include "Voice.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const double Voice::PI = 3.141592653589793;

			Frequencies *Voice::getFrequencies() {
				static Frequencies *frequencies = new Frequencies;
				return frequencies;
			}

			void Voice::noteOn(int32 noteID, int16 pitch, ParamValue velocity, int32 sampleOffset) {
				this->noteID = noteID;
				this->pitch = pitch;
				noteOnVelocity = velocity;
				noteOnSampleOffset = sampleOffset;
				currentVolume = 1.0;
			}

			void Voice::noteOff(ParamValue velocity, int32 sampleOffset) {
				noteOffVelocity = velocity;
				noteOffSampleOffset = sampleOffset;
			}

			bool Voice::process(double *outputBuffers[2], int32 numSamples) {
				if (pitch < 0 || pitch > 127) {
					return false;
				}
				ParamValue adjustedFreq = 2 * PI * 440 / SAMPLE_RATE;//getFrequencies()->table[pitch] / SAMPLE_RATE;
				// TODO: Update phase on frequency change
				for (int32 i = 0; i < numSamples; ++i) {
					if (noteOnSampleOffset <= 0) { // If note is on
						if (noteOffSampleOffset == 0) { // If note is released
							currentVolume = 0.0;
							return false;
						}
						double sample = std::sin(sampleIndex * adjustedFreq); //* currentVolume * (Params::volume / 100.0);
						outputBuffers[0][i] += sample;
						outputBuffers[1][i] += sample;
						++sampleIndex;
					}
					--noteOnSampleOffset;
					--noteOffSampleOffset;
				}
				return true;
			}

			void Voice::reset() {
				sampleIndex = 0;
				noteID = -1;
				noteOnSampleOffset = -1;
				noteOffSampleOffset = -1;
				currentVolume = 0.0;
			}

			int32 Voice::getNoteID() {
				return noteID;
			}
		}
	}
}
