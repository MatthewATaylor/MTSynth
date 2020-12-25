#pragma once

#include "VoiceBase.h"
#include "Controller.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif
#ifndef M_PI_MUL_2
#define M_PI_MUL_2		6.28318530717958647692
#endif

namespace Steinberg {
	namespace Vst {
		namespace mts {

			//-----------------------------------------------------------------------------
			struct GlobalParameterState
			{

				ParamValue masterVolume;	// [0, +1]
				ParamValue masterTuning;	// [-1, +1]
				ParamValue velToLevel;		// [0, +1]

				ParamValue noiseVolume;		// [0, +1]
				ParamValue sinusVolume;		// [0, +1]
				ParamValue triangleVolume;	// [0, +1]
				ParamValue squareVolume;	// [0, +1]

				ParamValue releaseTime;		// [0, +1]

				ParamValue sinusDetune;		// [-1, +1]
				ParamValue triangleSlop;	// [0, +1]

				ParamValue filterFreq;		// [-1, +1]
				ParamValue filterQ;			// [-1, +1]
				ParamValue freqModDepth;	// [-1, +1]

				int8 filterType;			// [0, 1, 2]
				int8 tuningRange;			// [0, 1]

				int8 bypassSNA;				// [0, 1]

				tresult setState(IBStream* stream);
				tresult getState(IBStream* stream);
			};

			//-----------------------------------------------------------------------------
			enum VoiceParameters
			{
				kVolumeMod,
				kTuningMod,
				kPanningLeft,
				kPanningRight,
				kNoiseVolume,
				kFilterFrequencyMod,
				kFilterQMod,
				kSinusVolume,
				kTriangleVolume,
				kFilterType,
				kTriangleSlope,
				kSinusDetune,
				kReleaseTimeMod,
				kSquareVolume,

				kNumParameters
			};

			//-----------------------------------------------------------------------------
			class VoiceStatics
			{
			public:
				//------------------------------------------------------------------------
				static double normalizedLevel2Gain(float normalized)
				{
					double level;
					if (normalized >= 0.5)
						level = scaleHeadRoom * ::pow(10, (normalized - 0.5f) * 24 / 20.0f);
					else
						level = scaleNorm2GainC1 * ::pow(normalized, scaleNorm2GainC2);

					return level;
				}

				enum {
					kNumFrequencies = 128
				};

				static float freqTab[kNumFrequencies];
				static const float scaleHeadRoom;
				static const float scaleNorm2GainC1;
				static const float scaleNorm2GainC2;
				static const double kNormTuningOneOctave;
				static const double kNormTuningOneTune;

			};

			//-----------------------------------------------------------------------------
			/** Example Note Expression Synth Voice Class

			\sa Steinberg::Vst::VoiceBase
			*/
			template<class SamplePrecision>
			class Voice : public VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>
			{
			public:
				void noteOn(int32 pitch, ParamValue velocity, float tuning, int32 sampleOffset, int32 nId) SMTG_OVERRIDE;
				void noteOff(ParamValue velocity, int32 sampleOffset) SMTG_OVERRIDE;
				bool process(SamplePrecision* outputBuffers[2], int32 numSamples);
				void reset() SMTG_OVERRIDE;

				void setNoteExpressionValue(int32 index, ParamValue value) SMTG_OVERRIDE;

			protected:
				uint32 n;
				int32 noisePos;
				int32 noiseStep;

				SamplePrecision trianglePhase;
				SamplePrecision sinusPhase;
				ParamValue currentTriangleF;
				ParamValue currentSinusF;
				ParamValue currentVolume;
				ParamValue currentPanningLeft;
				ParamValue currentPanningRight;
				ParamValue currentNoiseVolume;
				ParamValue currentSinusVolume;
				ParamValue currentSinusDetune;
				ParamValue currentSquareVolume;
				ParamValue currentTriangleVolume;
				ParamValue currentTriangleSlope;
				ParamValue currentLPFreq;
				ParamValue currentLPQ;

				ParamValue levelFromVel;
				ParamValue noteOffVolumeRamp;
			};

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			void Voice<SamplePrecision>::setNoteExpressionValue(int32 index, ParamValue value)
			{
				if (this->globalParameters->bypassSNA)
					return;

				switch (index)
				{
					//------------------------------
				case Steinberg::Vst::kVolumeTypeID:
				{
					ParamValue vol = VoiceStatics::normalizedLevel2Gain((float)value);
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kVolumeMod, vol);
					break;
				}
				//------------------------------
				case Steinberg::Vst::kTuningTypeID:
				{
					if (value == 0.5)
					{
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kTuningMod, 0);
					}
					else if (value > 0.5)
					{
						if (this->globalParameters->tuningRange > 0.5)
							value = std::min<ParamValue>(0.5 + 2 * VoiceStatics::kNormTuningOneTune, value);
						else
							value = std::min<ParamValue>(0.5 + VoiceStatics::kNormTuningOneOctave, value);
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kTuningMod, (value - 0.5) * 2.);
					}
					else
					{
						if (this->globalParameters->tuningRange > 0.5)
							value = std::max<ParamValue>(0.5 - 3 * VoiceStatics::kNormTuningOneTune, value);
						else
							value = std::max<ParamValue>(0.5 - VoiceStatics::kNormTuningOneOctave, value);
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kTuningMod, (value - 0.5) * 2.);
					}
					break;
				}
				//------------------------------
				case Steinberg::Vst::kPanTypeID:
				{
					if (value == 0.5)
					{
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kPanningLeft, 1.);
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kPanningRight, 1.);
					}
					else if (value > 0.5)
					{
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kPanningLeft, 1.);
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kPanningRight, 1. + ((0.5 - value) * 2.));
					}
					else
					{
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kPanningLeft, value * 2.);
						VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kPanningRight, 1.);
					}
					break;
				}
				//------------------------------
				case Controller::kNoiseVolumeTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kNoiseVolume, value * 2.);
					break;
				}
				//------------------------------
				case Controller::kSinusVolumeTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kSinusVolume, value * 2.);
					break;
				}
				//------------------------------
				case Controller::kTriangleVolumeTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kTriangleVolume, value * 2.);
					break;
				}
				//------------------------------
				case Controller::kSquareVolumeTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kSquareVolume, value * 2.);
					break;
				}
				//------------------------------
				case Controller::kFilterFreqModTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kFilterFrequencyMod, (value - 0.5) * 2.);
					break;
				}
				//------------------------------
				case Controller::kFilterQModTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kFilterQMod, (value - 0.5) * 2.);
					break;
				}
				//------------------------------
				case Controller::kTriangleSlopeTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kTriangleSlope, value);
					break;
				}
				//------------------------------
				case Controller::kSinusDetuneTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kSinusDetune, value);
					break;
				}
				//------------------------------
				case Controller::kReleaseTimeModTypeID:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(kReleaseTimeMod, 2 * (value - 0.5));
					break;
				}
				//------------------------------
				default:
				{
					VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(index, value);
					break;
				}
				}
			}

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			bool Voice<SamplePrecision>::process(SamplePrecision* outputBuffers[2], int32 numSamples)
			{
				//---compute tuning-------------------------

				// main tuning
				ParamValue tuningInHz = 0.;
				if (this->values[kTuningMod] != 0. || this->globalParameters->masterTuning != 0 || this->tuning != 0)
				{
					tuningInHz = VoiceStatics::freqTab[this->pitch] * (::pow(2.0, (this->values[kTuningMod] * 10 + this->globalParameters->masterTuning * 2.0 / 12.0 + this->tuning)) - 1);
				}

				ParamValue triangleFreq = (VoiceStatics::freqTab[this->pitch] + tuningInHz) * M_PI_MUL_2 / this->getSampleRate() / 2.;
				if (currentTriangleF == -1)
					currentTriangleF = triangleFreq;
				// check for frequency changes and update the phase so that it is crackle free
				if (triangleFreq != currentTriangleF)
				{
					// update phase
					trianglePhase = (SamplePrecision)((currentTriangleF - triangleFreq) * n + trianglePhase);
					currentTriangleF = triangleFreq;
				}

				// Sinus Detune
				if (currentSinusDetune != this->values[kSinusDetune])
				{
					currentSinusDetune = VoiceStatics::freqTab[this->pitch] * (::pow(2.0, this->values[kSinusDetune] * 2.0 / 12.0) - 1);
				}
				ParamValue sinusFreq = (VoiceStatics::freqTab[this->pitch] + tuningInHz + currentSinusDetune) * M_PI_MUL_2 / this->getSampleRate();
				if (currentSinusF == -1)
					currentSinusF = sinusFreq;
				if (sinusFreq != currentSinusF)
				{
					// update phase
					sinusPhase = (SamplePrecision)((currentSinusF - sinusFreq) * n) + sinusPhase;
					currentSinusF = sinusFreq;
				}

				//---calculate parameter ramps
				ParamValue volumeRamp = 0.;
				ParamValue panningLeftRamp = 0.;
				ParamValue panningRightRamp = 0.;
				ParamValue noiseVolumeRamp = 0.;
				ParamValue sinusVolumeRamp = 0.;
				ParamValue triangleVolumeRamp = 0.;
				ParamValue squareVolumeRamp = 0.;
				ParamValue filterFreqRamp = 0.;
				ParamValue filterQRamp = 0.;
				ParamValue triangleSlopeRamp = 0.;
				ParamValue rampTime = std::max<ParamValue>((ParamValue)numSamples, (this->sampleRate * 0.005));

				ParamValue wantedVolume = VoiceStatics::normalizedLevel2Gain((float)Bound(0.0, 1.0, this->globalParameters->masterVolume * levelFromVel + this->values[kVolumeMod]));
				if (wantedVolume != currentVolume)
				{
					volumeRamp = (wantedVolume - currentVolume) / rampTime;
				}

				if (this->values[kPanningLeft] != currentPanningLeft)
				{
					panningLeftRamp = (this->values[kPanningLeft] - currentPanningLeft) / rampTime;
				}
				if (this->values[kPanningRight] != currentPanningRight)
				{
					panningRightRamp = (this->values[kPanningRight] - currentPanningRight) / rampTime;
				}
				if (this->values[kNoiseVolume] != currentNoiseVolume)
				{
					noiseVolumeRamp = (this->values[kNoiseVolume] - currentNoiseVolume) / rampTime;
				}
				if (this->values[kSinusVolume] != currentSinusVolume)
				{
					sinusVolumeRamp = (this->values[kSinusVolume] - currentSinusVolume) / rampTime;
				}
				if (this->values[kSquareVolume] != currentSquareVolume)
				{
					squareVolumeRamp = (this->values[kSquareVolume] - currentSquareVolume) / rampTime;
				}
				if (this->values[kTriangleVolume] != currentTriangleVolume)
				{
					triangleVolumeRamp = (this->values[kTriangleVolume] - currentTriangleVolume) / rampTime;
				}

				ParamValue wantedLPFreq = Bound(0., 1., this->globalParameters->filterFreq + this->globalParameters->freqModDepth * this->values[kFilterFrequencyMod]);
				if (wantedLPFreq != currentLPFreq)
				{
					filterFreqRamp = (wantedLPFreq - currentLPFreq) / rampTime;
				}
				ParamValue wantedLPQ = Bound(0., 1., this->globalParameters->filterQ + this->values[kFilterQMod]);
				if (wantedLPQ != currentLPQ)
				{
					filterQRamp = (wantedLPQ - currentLPQ) / rampTime;
				}

				if (this->values[kTriangleSlope] != currentTriangleSlope)
				{
					triangleSlopeRamp = (this->values[kTriangleSlope] - currentTriangleSlope) / rampTime;
				}

				for (int32 i = 0; i < numSamples; i++)
				{
					this->noteOnSampleOffset--;
					this->noteOffSampleOffset--;

					if (this->noteOnSampleOffset <= 0)
					{
						// we are in Release
						if (this->noteOffSampleOffset == 0)
						{
							volumeRamp = 0;
							if (currentVolume > 0)
							{
								// ramp note off
								currentVolume -= noteOffVolumeRamp;
								if (currentVolume < 0.)
									currentVolume = 0.;
								this->noteOffSampleOffset++;
							}
							else
							{
								this->noteOffSampleOffset = this->noteOnSampleOffset = -1;
								return false;
							}
						}
						SamplePrecision sample;
						auto osc = (SamplePrecision)sin(n * triangleFreq + trianglePhase);
						// square osc
						sample = (SamplePrecision)((::floor(osc) + 0.5) * currentSquareVolume);
						// triangle osc
						sample += (SamplePrecision)((osc - ::fabs(sin(n * triangleFreq + trianglePhase + 1 + currentTriangleSlope))) * currentTriangleVolume);
						// sinus osc
						sample += (SamplePrecision)(sin(n * sinusFreq + sinusPhase) * currentSinusVolume);

						n++;

						// store in output
						outputBuffers[0][i] += (SamplePrecision)(sample * currentPanningLeft * currentVolume);
						outputBuffers[1][i] += (SamplePrecision)(sample * currentPanningRight * currentVolume);

						// ramp parameters
						currentVolume += volumeRamp;
						currentPanningLeft += panningLeftRamp;
						currentPanningRight += panningRightRamp;
						currentNoiseVolume += noiseVolumeRamp;
						currentSinusVolume += sinusVolumeRamp;
						currentSquareVolume += squareVolumeRamp;
						currentTriangleVolume += triangleVolumeRamp;
						currentTriangleSlope += triangleSlopeRamp;
					}
				}
				return true;
			}

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			void Voice<SamplePrecision>::noteOn(int32 _pitch, ParamValue velocity, float tuning, int32 sampleOffset, int32 nId)
			{
				currentVolume = 0;
				this->values[kVolumeMod] = 0;
				levelFromVel = 1.f + this->globalParameters->velToLevel * (velocity - 1.);

				currentSinusVolume = this->values[kSinusVolume] = this->globalParameters->sinusVolume;
				currentTriangleVolume = this->values[kTriangleVolume] = this->globalParameters->triangleVolume;
				currentNoiseVolume = this->values[kNoiseVolume] = this->globalParameters->noiseVolume;
				currentTriangleSlope = this->values[kTriangleSlope] = this->globalParameters->triangleSlop;
				currentSquareVolume = this->values[kSquareVolume] = this->globalParameters->squareVolume;

				// filter setting
				currentLPFreq = this->globalParameters->filterFreq;
				this->values[kFilterFrequencyMod] = 0;
				currentLPQ = this->globalParameters->filterQ;
				this->values[kFilterQMod] = 0;

				currentSinusDetune = 0.;
				if (this->globalParameters->sinusDetune != 0.)
				{
					currentSinusDetune = VoiceStatics::freqTab[this->pitch] * (::pow(2.0, this->globalParameters->sinusDetune * 2.0 / 12.0) - 1);
				}
				this->values[kSinusDetune] = currentSinusDetune;
				this->values[kTuningMod] = 0;

				VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOn(_pitch, velocity, tuning, sampleOffset, nId);
				this->noteOnSampleOffset++;
			}

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			void Voice<SamplePrecision>::noteOff(ParamValue velocity, int32 sampleOffset)
			{
				VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOff(velocity, sampleOffset);
				this->noteOffSampleOffset++;

				ParamValue timeFactor;
				if (this->values[kReleaseTimeMod] == 0)
					timeFactor = 1;
				else
					timeFactor = ::pow(100., this->values[kReleaseTimeMod]);

				noteOffVolumeRamp = 1.0 / (timeFactor * this->sampleRate * ((this->globalParameters->releaseTime * MAX_RELEASE_TIME_SEC) + 0.005));
				if (currentVolume)
					noteOffVolumeRamp *= currentVolume;
			}

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			void Voice<SamplePrecision>::reset()
			{
				noiseStep = 1;
				noisePos = 0;
				n = 0;
				sinusPhase = trianglePhase = 0.;
				currentSinusF = currentTriangleF = -1.;
				this->values[kVolumeMod] = 0.;
				this->values[kTuningMod] = 0.;
				this->values[kFilterFrequencyMod] = 0.;
				this->values[kFilterQMod] = 0.;
				this->values[kReleaseTimeMod] = 0.;
				currentPanningLeft = this->values[kPanningLeft] = 1.;
				currentPanningRight = this->values[kPanningRight] = 1.;
				currentNoiseVolume = this->values[kNoiseVolume] = 0.5;
				currentSinusVolume = this->values[kSinusVolume] = 0.5;
				currentSquareVolume = this->values[kSquareVolume] = 0.5;
				currentTriangleVolume = this->values[kTriangleVolume] = 0.5;
				currentLPFreq = 1.;
				currentLPQ = 0.;
				noteOffVolumeRamp = 0.005;

				VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::reset();
			}

		}
	}
} // namespaces
