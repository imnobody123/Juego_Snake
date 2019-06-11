// mp3player.h: headers for psp mp3player code
//
// All public functions for mp3player
//
//////////////////////////////////////////////////////////////////////
#ifndef _MP3PLAYER_H_
#define _MP3PLAYER_H_

#include <mad.h>
//#include "../../codec.h"

#ifdef __cplusplus

extern "C"
{
	#endif
	//  Function prototypes for public functions
	//    void MP3setStubs(codecStubs * stubs);

	//private functions
		void MP3_Init(int channel);
		void MP3_Pause();
		void MP3_End();
		void MP3_FreeTune();
		void MP3_GetTimeString(char *dest);

		int MP3_Play();
		int MP3_Stop();
		int MP3_Load(char *filename);
		int MP3_EndOfStream();

	#ifdef __cplusplus
}
#endif
#endif
