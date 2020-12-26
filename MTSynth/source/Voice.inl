#pragma once

namespace Steinberg {
	namespace Vst {
		namespace mts {
			inline void Voice::noteOn(int32 noteID, int32 pitch, float tuning, float velocity, int32 sampleOffset) {
				this->noteID = noteID;
				this->pitch = pitch;
				this->tuning = tuning;
				this->noteOnVelocity = velocity;
				this->noteOnSampleOffset = sampleOffset;
			}

			inline void Voice::noteOff(float velocity, int32 sampleOffset) {
				this->noteOffVelocity = velocity;
				this->noteOffSampleOffset = sampleOffset;
			}

			template <typename SampleType>
			inline bool Voice::process(SampleType **outputBuffers, int32 numSamples, SampleRate sampleRate) {
				sineVolume = ParamState::global.masterVolume * ParamState::global.sineVolume;
				squareVolume = ParamState::global.masterVolume * ParamState::global.squareVolume;
				double adjustedFreq = FrequencyTable::get()[pitch] * 2 * PI / sampleRate;

				for (int32 i = 0; i < numSamples; ++i) {
					if (noteOnSampleOffset <= 0) {
						if (noteOffSampleOffset == 0) {
							// Release
							sineVolume = 0;
							squareVolume = 0;
							return false;
						}

						SampleType sineSample = static_cast<SampleType>(std::sin(sampleIndex * adjustedFreq));
						SampleType squareSample = sineSample > 0 ? 1 : -1;
						for (uint8 j = 0; j < NUM_CHANNELS; ++j) {
							// Add sample to each channel's buffer
							outputBuffers[j][i] += sineSample * sineVolume;
							outputBuffers[j][i] += squareSample * squareVolume;
						}

						++sampleIndex;
					}

					--noteOnSampleOffset;
					--noteOffSampleOffset;
				}

				return true;
			}

			inline void Voice::reset() {
				sampleIndex = 0;
				noteID = -1;
				pitch = -1;
				tuning = 0.0f;
				noteOnSampleOffset = -1;
				noteOffSampleOffset = -1;
				sineVolume = 0.0;
				squareVolume = 0.0;
			}

			inline int32 Voice::getNoteID() const {
				return noteID;
			}
		}
	}
}
