#include "Voice.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const double Voice::PI = 3.141592653589793;
			const double Voice::SAMPLE_RATE = 44100.0;

			void Voice::noteOn(int32 noteID, int32 pitch, float tuning, float velocity, int32 sampleOffset) {
				this->noteID = noteID;
				this->pitch = pitch;
				this->tuning = tuning;
				this->noteOnVelocity = velocity;
				this->noteOnSampleOffset = sampleOffset;
			}
			
			void Voice::noteOff(float velocity, int32 sampleOffset) {
				this->noteOffVelocity = velocity;
				this->noteOffSampleOffset = sampleOffset;
			}
			
			bool Voice::process(float *outputBuffers[2], int32 numSamples) {
				volume = ParamState::global.volume;
				double adjustedFreq = FrequencyTable::get()[pitch] * 2 * PI / SAMPLE_RATE;

				for (int32 i = 0; i < numSamples; ++i) {
					if (noteOnSampleOffset <= 0) {
						if (noteOffSampleOffset == 0) {
							// Release
							volume = 0;
							return false;
						}
						
						float sample = static_cast<float>(std::sin(sampleIndex * adjustedFreq) * volume);
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
				pitch = -1;
				tuning = 0.0f;
				noteOnSampleOffset = -1;
				noteOffSampleOffset = -1;
			}

			int32 Voice::getNoteID() const {
				return noteID;
			}
		}
	}
}
