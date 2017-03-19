#define _USE_MATH_DEFINES
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "inc/fmod.hpp"
#include "inc/fmod_dsp.h"
#include "inc/fmod_errors.h"

#pragma comment (lib, "fmodex_vc.lib")

#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
#define __PACKED                         /* dummy */
#else
#define __PACKED __attribute__((packed)) /* gcc packed */
#endif

using namespace std;
using namespace FMOD;

//======================================================================
// FMOD Setting
FMOD::System			*mySystem      = 0;
FMOD::Sound				*sound         = 0;
FMOD::Channel			*channel       = 0;
FMOD::DSP			    *dsplowpass    = 0;
FMOD::DSP			    *dsphighpass   = 0;
FMOD_RESULT				result;
FMOD_CREATESOUNDEXINFO	exinfo;
int						key = 0, recorddriver, numdrivers, count_;
unsigned int			version;
FILE					*fp;
unsigned int			datalength = 0, soundlength;
//======================================================================

void SaveWav(char *file_name);
void WriteWavHeader(FILE *fp, FMOD::Sound *sound, int length);
void ERRCHECK(FMOD_RESULT result);
void LowPass(float f_uc);
void BandPass(float f_lc, float f_uc);
void HighPass(float f_lc);

int main()
{
	
	/*
	Create a System object and initialize.
	*/
	// Create a system
	result = FMOD::System_Create(&mySystem);
	ERRCHECK(result);

	// Version check
	result = mySystem->getVersion(&version);	
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 0;
	}

	// Output Type : Windows Multimedia WaveOut
	result = mySystem->setOutput(FMOD_OUTPUTTYPE_WINMM);
	ERRCHECK(result);

	// Init
	result = mySystem->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	// Load Sound
	result = mySystem->createSound("white_noise.wav", FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &sound);
	ERRCHECK(result);

	//======================================================================
	// Play Original Sound
	result = mySystem->playSound(FMOD_CHANNEL_FREE, sound, FALSE, &channel);
	ERRCHECK(result);

	// Create a Filter
	result = mySystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpass);
	ERRCHECK(result);
	result = mySystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpass);
	ERRCHECK(result);

	// Frequency
	float f_lc = 0;	// Highpass Frequency
	float f_uc = 0;	// Lowpass Frequency
	int menu = 0;

	do{
		cout << "=============MENU=============" << endl;
		cout << "1. low-pass filter" << endl;
		cout << "2. band-pass filter" << endl;
		cout << "3. high-pass filter" << endl;
		cout << "4. Save" << endl;
		cout << "EXIT(ESC)" << endl << endl;

		key = _getch();	// Keyboard Input

		// Init
		result = dsplowpass->remove();
		ERRCHECK(result);
		result = dsphighpass->remove();
		ERRCHECK(result);

		switch(key)
		{
		case '1' :	// Low Pass
			{
				menu = 1;
				cout << "<low-pass filter>" << endl;
				cout << "Please Input Frequency(f_uc)(1.0Hz ~ 22000.0Hz) : ";
				cin >> f_uc;

				LowPass(f_uc);

				break;
			}
		case '2' :	// Band Pass
			{
				menu = 2;
				cout << "<band-pass filter>" << endl;
				cout << "Please Input Frequency(f_lc)(10.0Hz ~ 22000.0Hz) : ";
				cin >> f_lc;
				cout << "Please Input Frequency(f_uc)(1.0Hz ~ 22000.0Hz) : ";
				cin >> f_uc;

				BandPass(f_lc, f_uc);

				break;
			}
		case '3' :	// High Pass
			{
				menu = 3;
				cout << "<high-pass filter>" << endl;
				cout << "Please Input Frequency(f_lc)(10.0Hz ~ 22000.0Hz) : ";
				cin >> f_lc;

				HighPass(f_lc);

				break;
			}
		case '4' :	// Save
			{
				if(menu == 1){
					LowPass(f_uc);
					SaveWav("lowpass.wav");
				}
				else if(menu == 2){
					BandPass(f_lc, f_uc);
					SaveWav("bandpass.wav");
				}
				else if(menu == 3){
					HighPass(f_lc);
					SaveWav("highpass.wav");
				}
				break;
			}
		default :
			break;
		}

		mySystem->update();

	} while(key != 27);

	/*
	Shut down
	*/
	result = sound->release();
	ERRCHECK(result);
	result = mySystem->close();
	ERRCHECK(result);
	result = mySystem->release();
	ERRCHECK(result);

	return 0;
}

// Save
void SaveWav(char *file_name)
{
	// Set Sound Driver
	result = mySystem->getRecordNumDrivers(&numdrivers);
	ERRCHECK(result);

	printf("---------------------------------------------------------\n");    
	printf("Choose a RECORD driver\n");
	printf("---------------------------------------------------------\n");    
	for (count_=0; count_ < numdrivers; count_++)
	{
		char name[256];

		result = mySystem->getRecordDriverInfo(count_, name, 256, 0);
		ERRCHECK(result);

		printf("%d : %s\n", count_ + 1, name);
	}
	printf("---------------------------------------------------------\n");

	recorddriver = 0;
	do
	{
		key = _getch();
		if (key == 27)
		{
			break;
		}
		recorddriver = key - '1';
	} while (recorddriver < 0 || recorddriver >= numdrivers);

	// Set Setting
	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

	exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.numchannels      = 1;
	exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
	exinfo.defaultfrequency = 10000;
	exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels;

	// Create Sound
	result = mySystem->createSound(0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound);
	ERRCHECK(result);

	// Recording
	result = mySystem->recordStart(recorddriver, sound, true);
	ERRCHECK(result);

	// Create a File
	fp = fopen(file_name, "wb");
	if (!fp)
	{
		printf("ERROR : could not open record.wav for writing.\n");
		exit(0);
	}

	cout << endl << "Recording now." << endl;
	/*
	Write out the wav header.  As we don't know the length yet it will be 0.
	*/
	WriteWavHeader(fp, sound, datalength);
	result = sound->getLength(&soundlength, FMOD_TIMEUNIT_PCM);
	ERRCHECK(result);

	do
	{
		static unsigned int lastrecordpos = 0;
		unsigned int recordpos = 0;

		if (_kbhit())
		{
			key = _getch();
		}

		mySystem->getRecordPosition(recorddriver, &recordpos);
		ERRCHECK(result);

		if (recordpos != lastrecordpos)        
		{
			void *ptr1, *ptr2;
			int blocklength;
			unsigned int len1, len2;

			blocklength = (int)recordpos - (int)lastrecordpos;
			if (blocklength < 0)
			{
				blocklength += soundlength;
			}

			/*
			Lock the sound to get access to the raw data.
			*/
			sound->lock(lastrecordpos * exinfo.numchannels, blocklength * exinfo.numchannels, &ptr1, &ptr2, &len1, &len2);   /* * exinfo.numchannels * 2 = stereo 16bit.  1 sample = 4 bytes. */

			/*
			Write it to disk.
			*/
			if (ptr1 && len1)
			{
				datalength += fwrite(ptr1, 1, len1, fp);
			}
			if (ptr2 && len2)
			{
				datalength += fwrite(ptr2, 1, len2, fp);
			}

			/*
			Unlock the sound to allow FMOD to use it again.
			*/
			sound->unlock(ptr1, ptr2, len1, len2);
		}

		lastrecordpos = recordpos;

		mySystem->update();

		Sleep(10);

	} while (key != 27);

	WriteWavHeader(fp, sound, datalength);
	fclose(fp);
}
// Write a header
void WriteWavHeader(FILE *fp, FMOD::Sound *sound, int length)
{
	int             channels, bits;
	float           rate;

	if (!sound)
	{
		return;
	}

	fseek(fp, 0, SEEK_SET);

	sound->getFormat  (0, 0, &channels, &bits);
	sound->getDefaults(&rate, 0, 0, 0);

	{
#if defined(WIN32) || defined(_WIN64) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
#pragma pack(1)
#endif

		/*
		WAV Structures
		*/
		typedef struct
		{
			signed char id[4];
			int 		size;
		} RiffChunk;

		struct
		{
			RiffChunk       chunk           __PACKED;
			unsigned short	wFormatTag      __PACKED;    /* format type  */
			unsigned short	nChannels       __PACKED;    /* number of channels (i.e. mono, stereo...)  */
			unsigned int	nSamplesPerSec  __PACKED;    /* sample rate  */
			unsigned int	nAvgBytesPerSec __PACKED;    /* for buffer estimation  */
			unsigned short	nBlockAlign     __PACKED;    /* block size of data  */
			unsigned short	wBitsPerSample  __PACKED;    /* number of bits per sample of mono data */
		} FmtChunk  = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, channels, (int)rate, (int)rate * channels * bits / 8, 1 * channels * bits / 8, bits } __PACKED;

		struct
		{
			RiffChunk   chunk;
		} DataChunk = { {{'d','a','t','a'}, length } };

		struct
		{
			RiffChunk   chunk;
			signed char rifftype[4];
		} WavHeader = { {{'R','I','F','F'}, sizeof(FmtChunk) + sizeof(RiffChunk) + length }, {'W','A','V','E'} };

#if defined(WIN32) || defined(_WIN64) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
#pragma pack()
#endif

		/*
		Write out the WAV header.
		*/
		fwrite(&WavHeader, sizeof(WavHeader), 1, fp);
		fwrite(&FmtChunk, sizeof(FmtChunk), 1, fp);
		fwrite(&DataChunk, sizeof(DataChunk), 1, fp);
	}
}

// Error Check
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

// Low pass
void LowPass(float f_uc)
{
	// Cutoff setting
	result = dsplowpass->setParameter(FMOD_DSP_LOWPASS_CUTOFF, f_uc);
	ERRCHECK(result);
	// Add a pass
	result = mySystem->addDSP(dsplowpass, 0);
	ERRCHECK(result);
}

// Bnad pass
void BandPass(float f_lc, float f_uc)
{
	// Cutoff setting
	result = dsplowpass->setParameter(FMOD_DSP_LOWPASS_CUTOFF, f_uc);
	ERRCHECK(result);
	// Add a pass
	result = mySystem->addDSP(dsplowpass, 0);
	ERRCHECK(result);

	// Cutoff setting
	result = dsphighpass->setParameter(FMOD_DSP_HIGHPASS_CUTOFF, f_lc);
	ERRCHECK(result);
	// Add a pass
	result = mySystem->addDSP(dsphighpass, 0);
	ERRCHECK(result);
}

// High pass
void HighPass(float f_lc)
{
	// Cutoff setting
	result = dsphighpass->setParameter(FMOD_DSP_HIGHPASS_CUTOFF, f_lc);
	ERRCHECK(result);	
	// Add a pass
	result = mySystem->addDSP(dsphighpass, 0);
	ERRCHECK(result);
}