#define _USE_MATH_DEFINES
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"

#pragma comment (lib, "fmodex_vc.lib")

#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
#define __PACKED                         /* dummy */
#else
#define __PACKED __attribute__((packed)) /* gcc packed */
#endif

using namespace std;
using namespace FMOD;

// 음계 번호 상수
enum SOUNDKIND
{
	SD_A = 0,	//라
	SD_AS,		//라#
	SD_B,		//시
	SD_C,		//도
	SD_CS,		//도#
	SD_D,		//레
	SD_DS,		//레#
	SD_E,		//미
	SD_F,		//파
	SD_FS,		//파#
	SD_G,		//솔
	SD_GS,		//솔#
	SD_END		//도+
};

// 음계 주파수 (Hz)
double frequency = 0.0;

// 음량의 변화
static float envelope = 32767.0f;

// 소리 설정(읽기)
FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *sound, void *data, unsigned int datalen)
{
	// Sample rate
	static int const sampleRate = 44100;

	// Volume level (0.0 - 1.0)
	static float const volume = 1.0f;

	// How many samples we have generated so far
	static int samplesElapsed = 0;

	// Get buffer in 16-bit format
	signed short *buffer = (signed short *)data;

	// A 2-channel 16-bit stereo stream uses 4 bytes per sample
	for (unsigned int sample = 0; sample < datalen / 4; sample++)
	{
		// Get the position in the sample
		double pos = frequency * static_cast<float>(samplesElapsed) / sampleRate;

		// The generator function returns a value from -1 to 1 so we multiply this by the
		// maximum possible volume of a 16-bit PCM sample (32767) to get the true volume to store

		// Generate a sample for the left channel
		*buffer++ = (signed short)(sin(pos * M_PI*2) * envelope * volume);

		// Generate a sample for the right channel
		*buffer++ = (signed short)(sin(pos * M_PI*2) * envelope * volume);

		// Increment number of samples generated
		samplesElapsed++;
		envelope-=1.5;
		if(envelope <= 0.0)
			envelope = 0.0f;
	}

	return FMOD_OK;
}

// 소리 설정(쓰기)
FMOD_RESULT F_CALLBACK pcmsetposcallback(FMOD_SOUND *sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype)
{
	/*
	This is useful if the user calls Channel::setPosition and you want to seek your data accordingly.
	*/
	return FMOD_OK;
}

// 저장
void SaveToWav(FMOD::Sound *sound)
{
	FILE *fw;
	int             channels, bits;
	float           rate;
	void           *ptr1, *ptr2;
	unsigned int    lenbytes, len1, len2;

	if (!sound)
	{
		return;
	}

	sound->getFormat  (0, 0, &channels, &bits);
	sound->getDefaults(&rate, 0, 0, 0);
	sound->getLength  (&lenbytes, FMOD_TIMEUNIT_PCMBYTES);

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
		} DataChunk = { {{'d','a','t','a'}, lenbytes } };

		struct
		{
			RiffChunk   chunk;
			signed char rifftype[4];
		} WavHeader = { {{'R','I','F','F'}, sizeof(FmtChunk) + sizeof(RiffChunk) + lenbytes }, {'W','A','V','E'} };

#if defined(WIN32) || defined(_WIN64) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
#pragma pack()
#endif
		fw = fopen("result.wav", "wb");

		/*
		Write out the WAV header.
		*/
		fwrite(&WavHeader, sizeof(WavHeader), 1, fw);
		fwrite(&FmtChunk, sizeof(FmtChunk), 1, fw);
		fwrite(&DataChunk, sizeof(DataChunk), 1, fw);

		/*
		Lock the sound to get access to the raw data.
		*/
		sound->lock(0, lenbytes, &ptr1, &ptr2, &len1, &len2);

		/*
		Write it to disk.
		*/
		fwrite(ptr1, len1, 1, fw);

		/*
		Unlock the sound to allow FMOD to use it again.
		*/
		sound->unlock(ptr1, ptr2, len1, len2);

		fclose(fw);
	}
}

// 에러 체크
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

int main()
{
	FMOD::System           *system = 0;
	FMOD::Sound            *sound[SD_END + 1];
	FMOD::Channel          *channel = 0;
	FMOD_RESULT             result;
	FMOD_MODE               mode = FMOD_2D | FMOD_OPENUSER | FMOD_HARDWARE;
	int                     key = 0;
	int                     channels = 2;		// 채널 수(좌, 우)
	FMOD_CREATESOUNDEXINFO  createsoundexinfo;
	unsigned int            version;
	
	for(int i = 0; i < SD_END + 1; i++)
		sound[i] = NULL;
	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&system);	// 시스템 생성
	ERRCHECK(result);

	result = system->getVersion(&version);	// 버전 확인
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 0;
	}

	// 출력 방식 : Windows Multimedia WaveOut
	result = system->setOutput(FMOD_OUTPUTTYPE_WINMM);
	ERRCHECK(result);
	
	// 기본 드라이버 선택
	result = system->setDriver(0);
	ERRCHECK(result);

	// 시스템 초기화
	result = system->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	// 사운드 정보
	memset(&createsoundexinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	createsoundexinfo.cbsize            = sizeof(FMOD_CREATESOUNDEXINFO);                /* required. */
	createsoundexinfo.decodebuffersize  = 44100;										 /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
	createsoundexinfo.length            = 44100 * channels * sizeof(signed short);		 /* Length of PCM data in bytes of whole song (for Sound::getLength) */	// 0.5초
	createsoundexinfo.numchannels       = channels;                                      /* Number of channels in the sound. */
	createsoundexinfo.defaultfrequency  = 44100;                                         /* Default playback rate of sound. */
	createsoundexinfo.format            = FMOD_SOUND_FORMAT_PCM16;                       /* Data format of sound. */
	createsoundexinfo.pcmreadcallback   = pcmreadcallback;                               /* User callback for reading. */
	createsoundexinfo.pcmsetposcallback = pcmsetposcallback;                             /* User callback for seeking. */	

	/*
	Main loop.
	*/
	do 
	{
		if (_kbhit())
		{
			key = _getch();
			envelope = 32767.0f;

			switch (key)
			{
			case 'q' :	// 도
				{
					cout << "도" << endl;
					frequency = 261.6256;	// 도
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_C]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_C], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break;
				}
			case 'w' :	// 레
				{ 
					cout << "레" << endl;
					frequency = 293.6648;	// 레
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_D]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_D], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case 'e' :	// 미
				{ 
					cout << "미" << endl;
					frequency = 329.6276;	// 미
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_E]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_E], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case 'r' :	// 파
				{ 
					cout << "파" << endl;
					frequency = 349.2282;	// 파
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_F]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_F], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case 't' :	// 솔
				{ 
					cout << "솔" << endl;
					frequency = 391.9954;	// 솔
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_G]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_G], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case 'y' :	// 라
				{ 
					cout << "라" << endl;
					frequency = 440.0000;	// 라
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_A]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_A], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case 'u' :	// 시
				{ 
					cout << "시" << endl;
					frequency = 493.8833;	// 시
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_B]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_B], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case 'i' :	// 도+
				{ 
					cout << "도+" << endl;
					frequency = 523.2511;	// 도+
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_END]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_END], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case '2' :	// 도#
				{ 
					cout << "도#" << endl;
					frequency = 277.1826;	// 도#
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_CS]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_CS], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case '3' :	// 레#
				{ 
					cout << "레#" << endl;
					frequency = 311.1270;	// 레#
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_DS]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_DS], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case '5' :	// 파#
				{ 
					cout << "파#" << endl;
					frequency = 369.9944;	// 파#
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_FS]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_FS], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case '6' :	// 솔#
				{ 
					cout << "솔#" << endl;
					frequency = 415.3047;	// 솔#
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_GS]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_GS], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			case '7' :	// 라#
				{ 
					cout << "라#" << endl;
					frequency = 466.1638;	// 라#
					createsoundexinfo.pcmreadcallback   = pcmreadcallback;
					result = system->createSound(0, mode, &createsoundexinfo, &sound[SD_AS]);	// 사운드 생성
					ERRCHECK(result);

					result = system->playSound(FMOD_CHANNEL_FREE, sound[SD_AS], FALSE, &channel);	// 사운드 재생
					ERRCHECK(result);
					break; 
				}
			default : 
				break;
			}
			
			system->update();
		}
	} while (key != 27);

	/*
	Shut down
	*/
	for(int i = 0; i < SD_END + 1; i++){
		if(sound[i]){
			result = sound[i]->release();
			ERRCHECK(result);
		}
	}
	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);

	return 0;
}