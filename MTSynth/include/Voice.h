#pragma once

#include <cmath>
#include <string>

#include "base/source/fdebug.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"

#include "ParamState.h"
#include "FrequencyTable.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Voice {
			private:
				inline static const double PI = 3.141592653589793;

				uint32 sampleIndex = 0;

				int32 noteID = -1;
				int32 pitch = -1;
				float tuning = 0.0f;

				float noteOnVelocity = 0.0f;
				float noteOffVelocity = 0.0f;
				
				int32 noteOnSampleOffset = 0;
				int32 noteOffSampleOffset = 0;

			public:
				static const uint8 NUM_CHANNELS = 2; // Left and right channels

				inline void noteOn(int32 noteID, int32 pitch, float tuning, float velocity, int32 sampleOffset);
				inline void noteOff(float velocity, int32 sampleOffset);
				
				template <typename SampleType>
				inline bool process(SampleType **outputBuffers, int32 numSamples, SampleRate sampleRate);
				
				inline void reset();
				inline int32 getNoteID() const;
			};
		}
	}
}

#include "../source/Voice.inl"
