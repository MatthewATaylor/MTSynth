#pragma once

#include "base/source/fstreamer.h"
#include "base/source/fstring.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vstnoteexpressiontypes.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstmidilearn.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/ivstphysicalui.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/ustring.h"

#include "ParamState.h"
#include "Voice.h"
#include "VoiceProcessor.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Controller : public EditController, public IMidiMapping {
			private:
				int midiControllerMapping[ControllerNumbers::kCountCtrlNumber];

			public:
				static const FUID ID;

				static FUnknown* createInstance(void*);

				// EditController
				tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;

				// IMidiMapping
				tresult PLUGIN_API getMidiControllerAssignment(
					int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID &id
				) SMTG_OVERRIDE;

				OBJ_METHODS(Controller, EditController)
				DEFINE_INTERFACES
					DEF_INTERFACE(IMidiMapping)
				END_DEFINE_INTERFACES(EditController)
				REFCOUNT_METHODS(EditController)
			};
		}
	}
}
