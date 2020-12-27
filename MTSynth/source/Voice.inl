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
				ParamValue sineVolume = ParamState::masterVolume * ParamState::sineVolume;
				ParamValue squareVolume = ParamState::masterVolume * ParamState::squareVolume;
				double rc = 1.0 / (2 * PI * ParamState::filterCutoff);

				double freq = FrequencyTable::get()[pitch] * std::pow(2, ParamState::tuning / 1200);
				double adjustedFreq = freq * 2 * PI / sampleRate; // Used to calculate sample values

				for (int32 i = 0; i < numSamples; ++i) {
					if (noteOnSampleOffset <= 0) {
						if (noteOffSampleOffset == 0) {
							// Release
							return false;
						}

						SampleType sineSample = static_cast<SampleType>(std::sin(sampleIndex * adjustedFreq));
						SampleType squareSample = sineSample > 0 ? 1 : -1;
						SampleType oscillatorOutput = sineSample * sineVolume + squareSample * squareVolume;
						
						SampleType filterOutput;
						switch (ParamState::filterType) {
						case ParamState::FilterType::LOW_PASS:
							filterOutput = static_cast<SampleType>(
								filter.getLowPass(oscillatorOutput, rc, sampleRate)
							);
							break;
						case ParamState::FilterType::HIGH_PASS:
							filterOutput = static_cast<SampleType>(
								filter.getHighPass(oscillatorOutput, rc, sampleRate)
							);
							break;
						}

						for (uint8 j = 0; j < NUM_CHANNELS; ++j) {
							// Add sample to each channel's buffer
							outputBuffers[j][i] += filterOutput;
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
				filter.reset();
			}

			inline int32 Voice::getNoteID() const {
				return noteID;
			}
		}
	}
}
