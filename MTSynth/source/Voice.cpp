#include "Voice.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const double Voice::PI = 3.141592653589793;
			Frequencies Voice::frequencies;

			void Voice::noteOn(int32 noteID, int16 pitch, ParamValue velocity, int32 sampleOffset) {
				this->noteID = noteID;
				this->pitch = pitch;
				noteOnVelocity = velocity;
				noteOnSampleOffset = sampleOffset;
			}

			void Voice::noteOff(ParamValue velocity, int32 sampleOffset) {
				noteOffVelocity = velocity;
				noteOffSampleOffset = sampleOffset;
			}

			bool Voice::process(double *outputBuffers[2], int32 numSamples) {
				if (pitch < 0 || pitch > 127) {
					return false;
				}
				ParamValue adjustedFreq = 2 * PI * frequencies.table[pitch] / SAMPLE_RATE;
				// TODO: Update phase on frequency change
				for (int32 i = 0; i < numSamples; ++i) {
					if (noteOnSampleOffset <= 0) { // If note is on
						if (noteOffSampleOffset == 0) { // If note is released
							return false;
						}
						double sample = std::sin(sampleIndex * adjustedFreq) * Params::volume;
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
			}

			int32 Voice::getNoteID() {
				return noteID;
			}
		}
	}
}
