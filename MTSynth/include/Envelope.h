#pragma once

#include "pluginterfaces/vst/vsttypes.h"

#include "ParamState.h"
#include "EnvelopeParamState.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Envelope {
			private:
				const EnvelopeParamState *paramState = nullptr;

				double prevValue = 0.0;
				
				bool releaseStarted = false;
				double releaseStartValue = 0.0;

			public:
				enum class State {
					NOTE_ON,
					NOTE_OFF
				};

				Envelope(const EnvelopeParamState *paramState);
				double calculate(int32 sampleOffset, SampleRate sampleRate, State state);
				void reset();
			};
		}
	}
}
