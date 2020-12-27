#include "ParamState.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			ParamValue ParamState::masterVolume = 0.5;
			ParamValue ParamState::tuning = 0.0;
			ParamValue ParamState::sineVolume = 1.0;
			ParamValue ParamState::squareVolume = 0.0;
			ParamState::FilterType ParamState::filterType = ParamState::FilterType::LOW_PASS;
			ParamValue ParamState::filterCutoff = 20000;

			tresult ParamState::getState(IBStream *stream) {
				IBStreamer streamer(stream, kLittleEndian);

				if (!streamer.writeDouble(masterVolume)) {
					return kResultFalse;
				}
				if (!streamer.writeDouble(tuning)) {
					return kResultFalse;
				}
				if (!streamer.writeDouble(sineVolume)) {
					return kResultFalse;
				}
				if (!streamer.writeDouble(squareVolume)) {
					return kResultFalse;
				}
				
				int8 filterTypeInt = static_cast<int8>(filterType);
				if (!streamer.writeInt8(filterTypeInt)) {
					return kResultFalse;
				}

				if (!streamer.writeDouble(filterCutoff)) {
					return kResultFalse;
				}

				return kResultTrue;
			}

			tresult ParamState::setState(IBStream *stream) {
				IBStreamer streamer(stream, kLittleEndian);
				
				if (!streamer.readDouble(masterVolume)) {
					return kResultFalse;
				}
				if (!streamer.readDouble(tuning)) {
					return kResultFalse;
				}
				if (!streamer.readDouble(sineVolume)) {
					return kResultFalse;
				}
				if (!streamer.readDouble(squareVolume)) {
					return kResultFalse;
				}

				int8 filterTypeInt;
				if (!streamer.readInt8(filterTypeInt)) {
					return kResultFalse;
				}
				filterType = static_cast<FilterType>(filterTypeInt);

				if (!streamer.readDouble(filterCutoff)) {
					return kResultFalse;
				}
				
				return kResultTrue;
			}
		}
	}
}
