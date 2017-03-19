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
	FMOD::Sound            *sound[2];
	FMOD::Channel          *channel[4];
	FMOD_RESULT             result;
	FMOD_MODE               mode = FMOD_SOFTWARE | FMOD_2D;
	int                     key = 0;
	bool					playing = false;
	unsigned int            version;
	
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
	result = system->init(4, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	// 사운드 로딩
	result = system->createSound("sample.wav", mode, 0, &sound[0]);
	ERRCHECK(result);
	result = system->createSound("sample_invert.wav", mode, 0, &sound[1]);
	ERRCHECK(result);

	cout << "재생하려면 1을, 종료는 esc를 눌러주세요." << endl;
	int check = 0;
	do 
	{
		if (_kbhit())
		{
			key = _getch();
		}

		if(key == '1'){
			if(check == 0){
				// 사운드 재생(좌)
				result = system->playSound(FMOD_CHANNEL_FREE, sound[0], false, &channel[0]);
				ERRCHECK(result);		
				result = channel[0]->setSpeakerMix(1.0f, 0, 0, 0, 0, 0, 0, 0);
				ERRCHECK(result);
				result = system->playSound(FMOD_CHANNEL_FREE, sound[1], false, &channel[2]);
				ERRCHECK(result);		
				result = channel[2]->setSpeakerMix(1.0f, 0, 0, 0, 0, 0, 0, 0);
				ERRCHECK(result);
				// 사운드 재생(우)
				result = system->playSound(FMOD_CHANNEL_FREE, sound[0], false, &channel[1]);
				ERRCHECK(result);		
				result = channel[1]->setSpeakerMix(0, 1.0f, 0, 0, 0, 0, 0, 0);
				ERRCHECK(result);
				result = system->playSound(FMOD_CHANNEL_FREE, sound[1], false, &channel[3]);
				ERRCHECK(result);		
				result = channel[3]->setSpeakerMix(0, 1.0f, 0, 0, 0, 0, 0, 0);
				ERRCHECK(result);

				channel[0]->setVolume(1.0f);
				channel[1]->setVolume(1.0f);
				channel[2]->setVolume(0.0f);
				channel[3]->setVolume(0.0f);
				check++;
			}

			// 좌 : 1, 우 : 1 -> 좌 : 1, 우 : 0
			cout << "정면" << endl;
			for(int i = 1; i <= 125; i++){
				channel[1]->setVolume(1.0f - 0.008f*i);
				Sleep(1);
			}

			// 좌 : 1, 우 : 0 -> 좌 : 1, 우 : (-)1
			cout << "좌" << endl;
			for(int i = 1; i <= 125; i++){
				channel[3]->setVolume(0.008f*i);
				Sleep(1);
			}

			// 좌 : 1, 우 : (-)1 -> 좌 : 0, 우 : 1
			cout << "뒤" << endl;
			channel[0]->setVolume(0.0f);
			channel[1]->setVolume(1.0f);
			channel[2]->setVolume(1.0f);
			channel[3]->setVolume(0.0f);
			for(int i = 1; i <= 125; i++){
				channel[2]->setVolume(1.0f - 0.008f*i);
				Sleep(1);
			}

			// 좌 : 0, 우 : 1 -> 좌 : 1, 우 : 1
			cout << "우" << endl;
			for(int i = 1; i <= 125; i++){
				channel[0]->setVolume(0.008f*i);
				Sleep(1);
			}

			channel[0]->isPlaying(&playing);
			if(!playing)
			{
				break;
			}

			system->update();
		}

	} while (key != 27);

	/*
	Shut down
	*/
	result = sound[0]->release();
	ERRCHECK(result);
	result = sound[1]->release();
	ERRCHECK(result);
	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);

	return 0;
}
