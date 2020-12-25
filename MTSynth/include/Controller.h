#pragma once

#include <array>
#include <limits>

#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vstnoteexpressiontypes.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstmidilearn.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/ivstphysicalui.h"

#include "ParamState.h"
#include "VoiceProcessor.h"

#define MAX_RELEASE_TIME_SEC	5.0
#define NUM_FILTER_TYPE			3
#define NUM_TUNING_RANGE		2

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Controller : public EditController,
				public IMidiMapping
			{
			public:
				//--- EditController -----------------------------
				tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
				tresult PLUGIN_API terminate() SMTG_OVERRIDE;
				tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;

				//--- IMidiMapping -------------------------------
				tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id/*out*/) SMTG_OVERRIDE;

				//--- ---------------------------------------------
				static FUnknown* createInstance(void*) { return (IEditController*)new Controller(); }

				static FUID cid;

				OBJ_METHODS(Controller, EditController)
					DEFINE_INTERFACES
					DEF_INTERFACE(IMidiMapping)
					END_DEFINE_INTERFACES(EditController)
					REFCOUNT_METHODS(EditController)

			protected:
				static const ParamID INVALID_PARAM_ID = std::numeric_limits<ParamID>::max();

				std::array<ParamID, ControllerNumbers::kCountCtrlNumber> midiCCMapping;
			};

			//------------------------------------------------------------------------
		} // NoteExpressionSynth
	} // Vst
} // Steinberg
