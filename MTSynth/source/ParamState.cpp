#include "ParamState.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			ParamState ParamState::global;

			tresult ParamState::getState(IBStream *stream) {
				IBStreamer streamer(stream, kLittleEndian);
				if (!streamer.writeDouble(volume)) {
					return kResultFalse;
				}
				return kResultTrue;
			}

			tresult ParamState::setState(IBStream *stream) {
				IBStreamer streamer(stream, kLittleEndian);
				if (!streamer.readDouble(volume)) {
					return kResultFalse;
				}
				return kResultTrue;
			}
		}
	}
}
