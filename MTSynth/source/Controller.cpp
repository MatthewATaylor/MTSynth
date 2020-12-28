#include "Controller.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const FUID Controller::ID(0x2AC0A888, 0x9406497F, 0xBBA6EABF, 0xC78D1372);

			FUnknown *Controller::createInstance(void*) {
				return static_cast<IEditController*>(new Controller());
			}

			tresult PLUGIN_API Controller::initialize(FUnknown *context) {
				tresult result = EditController::initialize(context);
				if (result == kResultTrue) {
					Parameter *param = new RangeParameter(
						USTRING("Master Volume"), ParamState::MASTER_VOLUME_ID,
						USTRING("%"), 0, 100, 50
					);
					param->setPrecision(1);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Tuning"), ParamState::TUNING_ID,
						USTRING("cents"), -200, 200, 0
					);
					param->setPrecision(0);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Sine Volume"), ParamState::SINE_VOLUME_ID,
						USTRING("%"), 0, 100, 100
					);
					param->setPrecision(1);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Square Volume"), ParamState::SQUARE_VOLUME_ID,
						USTRING("%"), 0, 100, 0
					);
					param->setPrecision(1);
					parameters.addParameter(param);

					StringListParameter *filterTypeParam = new StringListParameter(
						USTRING("Filter Type"), ParamState::FILTER_TYPE_ID
					);
					filterTypeParam->appendString(USTRING("Low-Pass"));
					filterTypeParam->appendString(USTRING("High-Pass"));
					parameters.addParameter(filterTypeParam);

					param = new RangeParameter(
						USTRING("Filter Cutoff"), ParamState::FILTER_CUTOFF_ID,
						USTRING(""), 0, 1, 1
					);
					param->setPrecision(2);
					parameters.addParameter(param);

					// Initialize MIDI control mapping
					for (int i = 0; i < ControllerNumbers::kCountCtrlNumber; ++i) {
						midiControllerMapping[i] = -1; // -1 = unused MIDI controls
					}
					midiControllerMapping[ControllerNumbers::kCtrlVolume] = ParamState::MASTER_VOLUME_ID;
					midiControllerMapping[ControllerNumbers::kPitchBend] = ParamState::TUNING_ID;
					midiControllerMapping[ControllerNumbers::kCtrlModWheel] = ParamState::FILTER_CUTOFF_ID;
				}
				return kResultTrue;
			}

			tresult PLUGIN_API Controller::getMidiControllerAssignment(
				int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID &id
			) {
				if (busIndex == 0 &&
					channel == 0 &&
					midiControllerNumber < kCountCtrlNumber &&
					midiControllerMapping[midiControllerNumber] != -1
				) {
					// Map MIDI controller number to parameter ID
					id = static_cast<ParamID>(midiControllerMapping[midiControllerNumber]);
					return kResultTrue;
				}
				return kResultFalse;
			}
		}
	}
}
