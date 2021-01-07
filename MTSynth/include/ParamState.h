#pragma once

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/vsttypes.h"

#include "EnvelopeParamState.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class ParamState {
			public:
				enum {
					MASTER_VOLUME_ID,
					TUNING_ID,
					SINE_VOLUME_ID,
					SQUARE_VOLUME_ID,
					FILTER_TYPE_ID,
					FILTER_CUTOFF_ID,
					VOLUME_A_ID,
					VOLUME_D_ID,
					VOLUME_S_ID,
					VOLUME_R_ID,
					FILTER_A_ID,
					FILTER_D_ID,
					FILTER_S_ID,
					FILTER_R_ID
				};

				enum class FilterType {
					LOW_PASS,
					HIGH_PASS
				};

				ParamValue masterVolume = 0.5;
				ParamValue tuning = 0.0;
				ParamValue sineVolume = 1.0;
				ParamValue squareVolume = 0.0;
				FilterType filterType = FilterType::LOW_PASS;
				ParamValue filterCutoff = 20000.0;
				EnvelopeParamState volumeEnvelopeParams = { 0.01, 0.0, 1.0, 0.01 };
				EnvelopeParamState filterEnvelopeParams = { 0.01, 0.0, 1.0, 0.01 };

				tresult getState(IBStream *stream);
				tresult setState(IBStream *stream);
			};
		}
	}
}
