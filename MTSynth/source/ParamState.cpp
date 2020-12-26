#include "ParamState.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			ParamState ParamState::global;

			tresult ParamState::getState(IBStream *stream) {
				IBStreamer streamer(stream, kLittleEndian);
				if (!streamer.writeDouble(masterVolume)) {
					return kResultFalse;
				}
				if (!streamer.writeDouble(sineVolume)) {
					return kResultFalse;
				}
				if (!streamer.writeDouble(squareVolume)) {
					return kResultFalse;
				}
				return kResultTrue;
			}

			tresult ParamState::setState(IBStream *stream) {
				IBStreamer streamer(stream, kLittleEndian);
				if (!streamer.readDouble(masterVolume)) {
					return kResultFalse;
				}
				if (!streamer.readDouble(sineVolume)) {
					return kResultFalse;
				}
				if (!streamer.readDouble(squareVolume)) {
					return kResultFalse;
				}
				return kResultTrue;
			}
		}
	}
}
