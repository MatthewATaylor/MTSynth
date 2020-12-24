#pragma once

#include <cmath>
#include <string>

#include "public.sdk/source/vst/vsteditcontroller.h"

#include "Frequencies.h"
#include "Params.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Voice {
			private:
				static const double PI;

				const double SAMPLE_RATE = 44100.0;

				uint32 sampleIndex = 0;
				int32 noteID = -1;
				int16 pitch = -1;
				int32 noteOnSampleOffset = 0;
				int32 noteOffSampleOffset = 0;

				ParamValue noteOnVelocity = 0.0;
				ParamValue noteOffVelocity = 0.0;
				ParamValue currentVolume = 0.0;

				static Frequencies *getFrequencies();

			public:
				void noteOn(int32 noteID, int16 pitch, ParamValue velocity, int32 sampleOffset);
				void noteOff(ParamValue velocity, int32 sampleOffset);
				bool process(double *outputBuffers[2], int32 numSamples);
				void reset();
				int32 getNoteID();
			};
		}
	}
}
