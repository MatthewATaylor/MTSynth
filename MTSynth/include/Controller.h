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

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Controller : public EditController,
				public INoteExpressionController,
				public IMidiMapping,
				public INoteExpressionPhysicalUIMapping {

			public:
				static const FUID ID;

				static FUnknown *createInstance(void*);

				// EditController
				tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
				tresult PLUGIN_API terminate() SMTG_OVERRIDE;
				tresult PLUGIN_API setComponentState(IBStream *state) SMTG_OVERRIDE;

				// INoteExpressionController
				int32 PLUGIN_API getNoteExpressionCount(int32 busIndex, int16 channel) SMTG_OVERRIDE;
				tresult PLUGIN_API getNoteExpressionInfo(
					int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo &info
				) SMTG_OVERRIDE;
				tresult PLUGIN_API getNoteExpressionStringByValue(
					int32 busIndex, int16 channel, NoteExpressionTypeID id,
					NoteExpressionValue valueNormalized, String128 string
				) SMTG_OVERRIDE;
				tresult PLUGIN_API getNoteExpressionValueByString(
					int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar *string,
					NoteExpressionValue &valueNormalized
				) SMTG_OVERRIDE;

				// IMidiMapping
				tresult PLUGIN_API getMidiControllerAssignment(
					int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID &id
				) SMTG_OVERRIDE;

				// INoteExpressionPhysicalUIMapping
				tresult PLUGIN_API getPhysicalUIMapping(
					int32 busIndex, int16 channel, PhysicalUIMapList &list
				) SMTG_OVERRIDE;

				OBJ_METHODS(Controller, EditController)
				DEFINE_INTERFACES
					DEF_INTERFACE(INoteExpressionController)
					DEF_INTERFACE(IMidiMapping)
					DEF_INTERFACE(INoteExpressionPhysicalUIMapping)
				END_DEFINE_INTERFACES(EditController)
				REFCOUNT_METHODS(EditController)
			};
		}
	}
}
