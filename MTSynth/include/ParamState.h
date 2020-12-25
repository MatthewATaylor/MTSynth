#pragma once

#include "base/source/fstreamer.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class ParamState {
			public:
				static ParamState global;

				enum {
					VOLUME_ID,
					ACTIVE_VOICES_ID
				};

				double volume = 0.5;

				tresult getState(IBStream *stream);
				tresult setState(IBStream *stream);
			};
		}
	}
}
