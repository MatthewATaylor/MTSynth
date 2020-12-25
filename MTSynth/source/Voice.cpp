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
				uint64 version = 0;

				// version 0
				if (!s.readInt64u(version))
					return kResultFalse;
				if (!s.readDouble(noiseVolume))
					return kResultFalse;
				if (!s.readDouble(sinusVolume))
					return kResultFalse;
				if (!s.readDouble(triangleVolume))
					return kResultFalse;
				if (!s.readDouble(releaseTime))
					return kResultFalse;
				if (!s.readDouble(sinusDetune))
					return kResultFalse;
				if (!s.readDouble(triangleSlop))
					return kResultFalse;
				if (!s.readInt8(bypassSNA))
					return kResultFalse;

				if (version >= 1)
				{
					if (!s.readInt8(filterType))
						return kResultFalse;
					if (!s.readDouble(filterFreq))
						return kResultFalse;
					if (!s.readDouble(filterQ))
						return kResultFalse;
				}
				if (version >= 2)
				{
					if (!s.readDouble(masterVolume))
						return kResultFalse;
					if (!s.readDouble(masterTuning))
						return kResultFalse;
					if (!s.readDouble(velToLevel))
						return kResultFalse;
					if (!s.readDouble(freqModDepth))
						return kResultFalse;
					if (!s.readInt8(tuningRange))
						return kResultFalse;
				}
				if (version >= 3)
				{
					if (!s.readDouble(squareVolume))
						return kResultFalse;
				}
				return kResultTrue;
			}

			//-----------------------------------------------------------------------------
			tresult GlobalParameterState::getState(IBStream* stream)
			{
				IBStreamer s(stream, kLittleEndian);

				// version 0
				if (!s.writeInt64u(currentParamStateVersion))
					return kResultFalse;
				if (!s.writeDouble(noiseVolume))
					return kResultFalse;
				if (!s.writeDouble(sinusVolume))
					return kResultFalse;
				if (!s.writeDouble(triangleVolume))
					return kResultFalse;
				if (!s.writeDouble(releaseTime))
					return kResultFalse;
				if (!s.writeDouble(sinusDetune))
					return kResultFalse;
				if (!s.writeDouble(triangleSlop))
					return kResultFalse;
				if (!s.writeInt8(bypassSNA))
					return kResultFalse;

				// version 1
				if (!s.writeInt8(filterType))
					return kResultFalse;
				if (!s.writeDouble(filterFreq))
					return kResultFalse;
				if (!s.writeDouble(filterQ))
					return kResultFalse;

				// version 2
				if (!s.writeDouble(masterVolume))
					return kResultFalse;
				if (!s.writeDouble(masterTuning))
					return kResultFalse;
				if (!s.writeDouble(velToLevel))
					return kResultFalse;
				if (!s.writeDouble(freqModDepth))
					return kResultFalse;
				if (!s.writeInt8(tuningRange))
					return kResultFalse;

				// version 3
				if (!s.writeDouble(squareVolume))
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

