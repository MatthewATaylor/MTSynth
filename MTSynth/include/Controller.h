#pragma once

#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Steinberg {
	namespace mts {
		class Controller : public Vst::EditController {
		public:
			static const FUID ID;
			static const Vst::ParamID testParamID = 100;

			static FUnknown *createInstance(void*);

			tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
			tresult PLUGIN_API setComponentState(IBStream *state) SMTG_OVERRIDE;
		};
	}
}
