#include "Voice.h"
#include "base/source/fstreamer.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {

			static uint64 currentParamStateVersion = 3;

			//-----------------------------------------------------------------------------
			tresult GlobalParameterState::setState(IBStream* stream)
			{
				IBStreamer s(stream, kLittleEndian);
				if (!s.readDouble(masterVolume))
					return kResultFalse;
				return kResultTrue;
			}

			//-----------------------------------------------------------------------------
			tresult GlobalParameterState::getState(IBStream* stream)
			{
				IBStreamer s(stream, kLittleEndian);
				if (!s.writeDouble(masterVolume))
					return kResultFalse;
				return kResultTrue;
			}

			//-----------------------------------------------------------------------------
			float VoiceStatics::freqTab[kNumFrequencies];
			const float VoiceStatics::scaleHeadRoom =
				(float)(pow(10.0, -12.0 / 20.0) * 0.70710677); // for 12 dB head room
			const float VoiceStatics::scaleNorm2GainC1 =
				(float)(VoiceStatics::scaleHeadRoom * pow(10.0, 24.0 / 20.0));
			const float VoiceStatics::scaleNorm2GainC2 =
				(float)(24.0 / 20.0 / 0.30102999566398119521373889472449); // Mathd::kLog2

			const double VoiceStatics::kNormTuningOneOctave = 12.0 / 240.0; // full in VST 3 is +- 10 octaves
			const double VoiceStatics::kNormTuningOneTune = 1.0 / 240.0;

			//-----------------------------------------------------------------------------
			class VoiceStaticsOnce
			{
			public:
				VoiceStaticsOnce()
				{
					// make frequency (Hz) table
					double k = 1.059463094359; // 12th root of 2
					double a = 6.875; // a
					a *= k; // b
					a *= k; // bb
					a *= k; // c, frequency of midi note 0
					for (int32 i = 0; i < VoiceStatics::kNumFrequencies; i++) // 128 midi notes
					{
						VoiceStatics::freqTab[i] = (float)a;
						a *= k;
					}
				}
			};

			static VoiceStaticsOnce gVoiceStaticsOnce;
		} // NoteExpressionSynth
	} // Vst
} // Steinberg

