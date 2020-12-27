#pragma once

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/vsttypes.h"

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
					FILTER_CUTOFF_ID
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

				static tresult getState(IBStream *stream);
				static tresult setState(IBStream *stream);
			};
		}
	}
}
