#pragma once

#include "base/source/fstreamer.h"
#include "base/source/fobject.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vstnoteexpressiontypes.h"
#include "pluginterfaces/base/ftypes.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstmidilearn.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/ivstphysicalui.h"

#include "Params.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Controller : public EditController {
			public:
				static const FUID ID;

				static FUnknown *createInstance(void*);

				// EditController
				tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
				tresult PLUGIN_API terminate() SMTG_OVERRIDE;
				tresult PLUGIN_API setComponentState(IBStream *state) SMTG_OVERRIDE;
			};
		}
	}
}
