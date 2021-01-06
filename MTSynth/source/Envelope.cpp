#include "Envelope.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			Envelope::Envelope(const EnvelopeParamState *paramState) : paramState(paramState) {}

			double Envelope::calculate(int32 sampleOffset, SampleRate sampleRate, State state) {
				double samplePeriod_s = 1.0 / sampleRate;
				double sampleTime_s = samplePeriod_s * sampleOffset;
				if (state == State::NOTE_ON) {
					if (sampleTime_s <= paramState->a) {
						// Attack
						if (paramState->a == 0) {
							prevValue = 1.0;
						}
						else {
							prevValue += 1.0 / paramState->a * samplePeriod_s;
						}
					}
					if (sampleTime_s >= paramState->a &&
						sampleTime_s <= paramState->a + paramState->d
					) {
						// Decay
						if (paramState->d == 0) {
							prevValue = paramState->s;
						}
						else {
							prevValue -= (1.0 - paramState->s) /
								paramState->d * samplePeriod_s;
						}
					}
				}
				else if (state == State::NOTE_OFF) {
					// Release
					if (paramState->r == 0) {
						prevValue = 0.0;
					}
					else {
						if (!releaseStarted) {
							releaseStartValue = prevValue;
							releaseStarted = true;
						}
						prevValue -= releaseStartValue /
							paramState->r * samplePeriod_s;
					}
				}
				return prevValue;
			}

			void Envelope::reset() {
				prevValue = 0.0;
				releaseStarted = false;
			}
		}
	}
}
