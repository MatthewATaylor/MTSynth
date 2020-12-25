#include "Controller.h"
#include "Voice.h" // only needed for setComponentState
#include "base/source/fstring.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/ustring.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {

			FUID Controller::cid(0x2AC0A888, 0x9406497F, 0xBBA6EABF, 0xC78D1372);

			//-----------------------------------------------------------------------------
			//-----------------------------------------------------------------------------
			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::initialize(FUnknown* context)
			{
				tresult result = EditController::initialize(context);
				if (result == kResultTrue)
				{
					// Init parameters
					Parameter* param;

					param = new RangeParameter(USTRING("Master Volume"), kParamMasterVolume, USTRING("%"), 0, 100, 80);
					param->setPrecision(1);
					parameters.addParameter(param);

					parameters.addParameter(new RangeParameter(USTRING("Active Voices"), kParamActiveVoices, nullptr, 0, MAX_VOICES, 0, MAX_VOICES, ParameterInfo::kIsReadOnly));

					// Init Default MIDI-CC Map
					std::for_each(midiCCMapping.begin(), midiCCMapping.end(), [](ParamID& pid) { pid = InvalidParamID; });
					midiCCMapping[ControllerNumbers::kCtrlVolume] = kParamMasterVolume;
				}
				return kResultTrue;
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::terminate()
			{
				noteExpressionTypes.removeAll();
				return EditController::terminate();
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::setComponentState(IBStream* state)
			{
				GlobalParameterState gps;
				tresult result = gps.setState(state);
				if (result == kResultTrue)
				{
					setParamNormalized(kParamMasterVolume, gps.masterVolume);
				}
				return result;
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::getMidiControllerAssignment(int32 busIndex, int16 channel,
				CtrlNumber midiControllerNumber,
				ParamID& id /*out*/)
			{
				if (busIndex == 0 && channel == 0 && midiControllerNumber < kCountCtrlNumber)
				{
					if (midiCCMapping[midiControllerNumber] != InvalidParamID)
					{
						id = midiCCMapping[midiControllerNumber];
						return kResultTrue;
					}
				}
				return kResultFalse;
			}

			//------------------------------------------------------------------------
		} // NoteExpressionSynth
	} // Vst
} // Steinberg
