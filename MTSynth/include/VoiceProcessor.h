#pragma once

#include <algorithm>

#include "base/source/fdebug.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "Voice.h"

#ifdef DEBUG_LOG
#undef DEBUG_LOG
#endif

#define DEBUG_LOG	DEVELOPMENT

#ifndef VOICEPROCESSOR_BLOCKSIZE
#define VOICEPROCESSOR_BLOCKSIZE	32
#endif

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class VoiceProcessor {
			public:
				static const uint8 MAX_VOICES = 64;

			private:
				int32 activeVoices = 0;
				bool mClearOutputNeeded = true;

				Voice* getVoice(int32 noteId);
				Voice* findVoice(int32 noteId);

			public:
				Voice voices[MAX_VOICES];

				tresult process(ProcessData& data);

				/** Returns the number of active voices. */
				int32 getActiveVoices() const { return activeVoices; }

				void clearOutputNeeded(bool val) { mClearOutputNeeded = val; }
			};
		}
	} // namespace Vst
} // namespace Steinberg

