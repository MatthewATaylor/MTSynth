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
				static const double PI;
				static const double SAMPLE_RATE;

				uint32 sampleIndex = 0;

				int32 noteID = -1;
				int32 pitch = -1;
				float tuning = 0.0f;

				float noteOnVelocity = 0.0f;
				float noteOffVelocity = 0.0f;
				
				int32 noteOnSampleOffset = 0;
				int32 noteOffSampleOffset = 0;

				ParamValue volume;

			public:
				void noteOn(int32 noteID, int32 pitch, float tuning, float velocity, int32 sampleOffset);
				void noteOff(float velocity, int32 sampleOffset);
				bool process(float *outputBuffers[2], int32 numSamples);
				void reset();
				int32 getNoteID() const;
			};
		}
	}
}
