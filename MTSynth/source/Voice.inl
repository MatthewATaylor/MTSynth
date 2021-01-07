#pragma once

namespace Steinberg {
	namespace Vst {
		namespace mts {
			inline void Voice::noteOn(int32 noteID, int32 pitch, float tuning, float velocity, int32 sampleOffset) {
				// Reset to end release envelope
				reset();
				
				this->noteID = noteID;
				this->pitch = pitch;
				this->tuning = tuning;
				noteOnVelocity = velocity;
				noteOnSampleOffset = sampleOffset;
			}

			inline void Voice::noteOff(float velocity, int32 sampleOffset) {
				noteOffVelocity = velocity;
				noteOffSampleOffset = sampleOffset;
				noteTurnedOff = true;
			}

			template <typename SampleType>
			inline bool Voice::process(
				SampleType **outputBuffers, int32 numSamples, SampleRate sampleRate,
				const ParamState &paramState
			) {
				ParamValue sineVolume = paramState.masterVolume * paramState.sineVolume;
				ParamValue squareVolume = paramState.masterVolume * paramState.squareVolume;
				double rc = 1.0 / (2 * PI * paramState.filterCutoff);

				double freq = FrequencyTable::get()[pitch] * std::pow(2, paramState.tuning / 1200);
				double adjustedFreq = freq * 2 * PI / sampleRate; // Used to calculate sample values

				// Adjust phase to prevent pitch bend crackle
				if (prevFreq < 0.0) {
					prevFreq = adjustedFreq;
				}
				if (adjustedFreq != prevFreq) {
					sinePhase += (prevFreq - adjustedFreq) * sampleIndex;
					prevFreq = adjustedFreq;
				}

				for (int32 i = 0; i < numSamples; ++i) {
					if (noteOnSampleOffset <= 0) {
						double volumeEnvelopeValue;
						double filterEnvelopeValue;
						if (noteOffSampleOffset <= 0 && noteTurnedOff) {
							// Release
							volumeEnvelopeValue = volumeEnvelope.calculate(
								-noteOffSampleOffset, sampleRate, Envelope::State::NOTE_OFF,
								paramState.volumeEnvelopeParams
							);
							filterEnvelopeValue = filterEnvelope.calculate(
								-noteOffSampleOffset, sampleRate, Envelope::State::NOTE_OFF,
								paramState.filterEnvelopeParams
							);
							if (volumeEnvelopeValue <= 0.000001) {
								// Note is off
								return false;
							}
						}
						else {
							volumeEnvelopeValue = volumeEnvelope.calculate(
								-noteOnSampleOffset, sampleRate, Envelope::State::NOTE_ON,
								paramState.volumeEnvelopeParams
							);
							filterEnvelopeValue = filterEnvelope.calculate(
								-noteOnSampleOffset, sampleRate, Envelope::State::NOTE_ON,
								paramState.filterEnvelopeParams
							);
						}

						SampleType sineSample = static_cast<SampleType>(
							std::sin(sampleIndex * adjustedFreq + sinePhase)
						);
						SampleType squareSample = sineSample > 0 ? 1 : -1;
						SampleType oscillatorOutput = sineSample * sineVolume + squareSample * squareVolume;
						oscillatorOutput *= volumeEnvelopeValue;
						
						// TODO: Apply filter envelope
						SampleType filterOutput;
						switch (paramState.filterType) {
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
				noteTurnedOff = false;
				
				filter.reset();

				sinePhase = 0.0;
				prevFreq = -1.0;

				volumeEnvelope.reset();
				filterEnvelope.reset();
			}

			inline int32 Voice::getNoteID() const {
				return noteID;
			}
		}
	}
}
