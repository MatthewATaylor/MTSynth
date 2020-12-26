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
				static const uint8 CHUNK_SIZE = 32; // Samples per processing chunk
				static const uint8 NUM_CHANNELS = 2; // Left and right channels

				int32 numActiveVoices = 0;

				// Return matching voice or first free voice if no match is found
				Voice *getBestVoice(int32 noteID);

				// Return matching voice or nullptr if no match is found
				Voice *findMatchingVoice(int32 noteID);

			public:
				static const uint8 MAX_VOICES = 64;

				Voice voices[MAX_VOICES];

				tresult process(ProcessData &data);
				int32 getNumActiveVoices() const;
			};
		}
	}
}
