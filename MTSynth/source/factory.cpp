#include "VST3_SDK/public.sdk/source/main/pluginfactory.h"
#include "VST3_SDK/pluginterfaces/vst/ivstcomponent.h"

#include "meta.h"
#include "Controller.h"
#include "Processor.h"

BEGIN_FACTORY_DEF(meta::COMPANY_NAME, meta::COMPANY_WEBSITE, meta::COMPANY_EMAIL)
	DEF_CLASS2(
		INLINE_UID_FROM_FUID(Steinberg::mts::Controller::ID),
		PClassInfo::kManyInstances,
		kVstComponentControllerClass,
		meta::CONTROLLER_NAME,
		0,
		"",
		meta::VERSION,
		kVstVersionString,
		Steinberg::mts::Controller::createInstance
	)
	DEF_CLASS2(
		INLINE_UID_FROM_FUID(Steinberg::mts::Processor::ID),
		PClassInfo::kManyInstances,
		kVstAudioEffectClass,
		meta::NAME,
		Vst::kDistributable,
		meta::CATEGORY,
		meta::VERSION,
		kVstVersionString,
		Steinberg::mts::Processor::createInstance
	)
END_FACTORY

bool InitModule() {
	return true;
}

bool DeinitModule() {
	return true;
}
