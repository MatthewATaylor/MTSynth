#pragma once

#include <algorithm>

#include "base/source/fdebug.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "Voice.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class VoiceProcessor {
			private:
				static const uint8 CHUNK_SIZE = 32;

				int32 numActiveVoices = 0;

				Voice *getVoice(int32 noteId);
				Voice *findVoice(int32 noteId);

			public:
				static const uint8 MAX_VOICES = 64;

				Voice voices[MAX_VOICES];

				tresult process(ProcessData &data);
				int32 getNumActiveVoices() const;
			};
		}
	}
}
