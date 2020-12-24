#pragma once

#include <algorithm>
#include <cstring>
#include <string>

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "Voice.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class VoiceProcessor {
			public:
				static const uint8 MAX_VOICES = 1;

			private:
				static const uint8 CHUNK_SIZE = 32;

				Voice voices[MAX_VOICES];
				int32 numActiveVoices = 0;

				// Get voice with matching noteID or the first free voice
				Voice *getBestVoice(int32 noteID);

				// Get voice with matching noteID (or return nullptr)
				Voice *findMatchingVoice(int32 noteID);

			public:
				Voice *getVoice(uint8 index);
				int32 getNumActiveVoices();
				tresult process(ProcessData &data);
			};
		}
	}
}
