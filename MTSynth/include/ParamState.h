#pragma once

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class ParamState {
			public:
				static ParamState global;

				enum {
					MASTER_VOLUME_ID,
					SINE_VOLUME_ID,
					SQUARE_VOLUME_ID
				};

				ParamValue masterVolume = 0.5;
				ParamValue sineVolume = 1.0;
				ParamValue squareVolume = 0.0;

				tresult getState(IBStream *stream);
				tresult setState(IBStream *stream);
			};
		}
	}
}
