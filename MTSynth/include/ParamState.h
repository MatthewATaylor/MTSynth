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

				static ParamValue masterVolume;
				static ParamValue tuning;
				static ParamValue sineVolume;
				static ParamValue squareVolume;
				static FilterType filterType;
				static ParamValue filterCutoff;
				static EnvelopeParamState volumeEnvelopeParams;
				static EnvelopeParamState filterEnvelopeParams;

				static tresult getState(IBStream *stream);
				static tresult setState(IBStream *stream);
			};
		}
	}
}
