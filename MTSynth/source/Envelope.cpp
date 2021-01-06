#include "Envelope.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			double Envelope::calculate(int32 sampleOffset, SampleRate sampleRate, State state) {
				double samplePeriod_s = 1.0 / sampleRate;
				double sampleTime_s = samplePeriod_s * sampleOffset;
				if (state == State::NOTE_ON) {
					if (sampleTime_s <= ParamState::volumeEnvelopeParams.a) {
						// Attack
						if (ParamState::volumeEnvelopeParams.a == 0) {
							prevValue = 1.0;
						}
						else {
							prevValue += 1.0 / ParamState::volumeEnvelopeParams.a * samplePeriod_s;
						}
					}
					if (sampleTime_s >= ParamState::volumeEnvelopeParams.a &&
						sampleTime_s <= ParamState::volumeEnvelopeParams.a + ParamState::volumeEnvelopeParams.d
					) {
						// Decay
						if (ParamState::volumeEnvelopeParams.d == 0) {
							prevValue = ParamState::volumeEnvelopeParams.s;
						}
						else {
							prevValue -= (1.0 - ParamState::volumeEnvelopeParams.s) /
								ParamState::volumeEnvelopeParams.d * samplePeriod_s;
						}
					}
				}
				else if (state == State::NOTE_OFF) {
					// Release
					if (ParamState::volumeEnvelopeParams.r == 0) {
						prevValue = 0.0;
					}
					else {
						if (!releaseStarted) {
							releaseStartValue = prevValue;
							releaseStarted = true;
						}
						prevValue -= releaseStartValue /
							ParamState::volumeEnvelopeParams.r * samplePeriod_s;
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
