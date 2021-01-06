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
						USTRING("%"), 0, 100, 100
					);
					param->setPrecision(1);
					parameters.addParameter(param);

					// Amp envelope
					param = new RangeParameter(
						USTRING("Volume A"), ParamState::VOLUME_A_ID,
						USTRING("s"), 0, EnvelopeParamState::MAX_TIME_S, 0.05
					);
					param->setPrecision(2);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Volume D"), ParamState::VOLUME_D_ID,
						USTRING("s"), 0, EnvelopeParamState::MAX_TIME_S, 0
					);
					param->setPrecision(2);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Volume S"), ParamState::VOLUME_S_ID,
						USTRING("%"), 0, 100, 100
					);
					param->setPrecision(1);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Volume R"), ParamState::VOLUME_R_ID,
						USTRING("s"), 0, EnvelopeParamState::MAX_TIME_S, 0.05
					);
					param->setPrecision(2);
					parameters.addParameter(param);

					// Filter envelope
					param = new RangeParameter(
						USTRING("Filter A"), ParamState::FILTER_A_ID,
						USTRING("s"), 0, EnvelopeParamState::MAX_TIME_S, 0
					);
					param->setPrecision(2);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Filter D"), ParamState::FILTER_D_ID,
						USTRING("s"), 0, EnvelopeParamState::MAX_TIME_S, 0
					);
					param->setPrecision(2);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Filter S"), ParamState::FILTER_S_ID,
						USTRING("%"), 0, 100, 100
					);
					param->setPrecision(1);
					parameters.addParameter(param);

					param = new RangeParameter(
						USTRING("Filter R"), ParamState::FILTER_R_ID,
						USTRING("s"), 0, EnvelopeParamState::MAX_TIME_S, 0
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
