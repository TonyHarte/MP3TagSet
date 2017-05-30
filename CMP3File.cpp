#include <windows.h>
//#include <cstdlib>
//#include <cstdio>
#include <strsafe.h>
#define NO_SHLWAPI_STRFCNS      //This stops warnings about deprecated string functions
#include <Shlwapi.h>
//#include <commctrl.h>
#include <string>


//****************** TEMP TEMP DELETE *************
#include "TEMPDEBUG.h"
//****************** TEMP TEMP DELETE *************

#include "CMP3File.h"


//================== static data ================
int CMP3File::nTrackNumber = 0;      
//================== static data ================


using namespace std;


CMP3File::CMP3File() {
//******************************************************************************
//* 
//******************************************************************************
//-------------------------------------
    sTitle[0] = '\0';
    sArtist[0] = '\0';
    sGenre[0] = '\0';
}



CMP3File::~CMP3File() {
//******************************************************************************
//* 
//******************************************************************************
//-------------------------------------
}



void CMP3File::Extract(char * pPathAndFile) {
//******************************************************************************
//* 
//******************************************************************************
    char    sValues[MAX_PATH];
//-------------------------------------
    Extract_FileName(pPathAndFile);

    strcpy_s(sValues, MAX_PATH, PathFindFileName(pPathAndFile));
    Extract_Track(sValues);
    Extract_Artist(sValues);
    Extract_Title(sValues);

    Tag.Extract(pPathAndFile);
}



void CMP3File::Extract_FileName(char * pPathAndFile) {
//******************************************************************************
//* 
//******************************************************************************
    unsigned int     nLen = 0;
/*--------------------------------*/
    strcpy_s(sFileName, MAX_PATH, PathFindFileName(pPathAndFile));

    nLen = strlen(sFileName);
//    nMaxFileNameLen = (nLen > nMaxFileNameLen) ? nLen : nMaxFileNameLen;
}



void CMP3File::Extract_Track(char * sValues) {
//******************************************************************************
// Tries to find the track number in the first 5 digits.  can be 1, 2 or 3 digits.
// Trying to be freeformish so ignore spaces or punctuation before number
// Only extract to remove from name.
//******************************************************************************
    const int nTrackCheckLen = TRACK_TEXT_LEN - 1;
    char    cDigit[nTrackCheckLen];
    int     ix, NumPos, RestPos;
    int     ValueLen = 0;
    char    sTrackPresent = 'Y';
    char    sContiguousDigits = 'Y';
    char    sEnd = 'N';
    int     nDigitCount = 0;
	char    sFileNameTrack[TRACK_TEXT_LEN];           
//-------------------------------------
    nTrackNumber++;
    _itoa_s(nTrackNumber, sTrack, TRACK_TEXT_LEN, 10);

    PathRemoveExtension(sValues);

    for (ix = 0; ix < nTrackCheckLen; ix++) {
        cDigit[ix] = Is_Numeric(sValues[ix]);

        if (cDigit[ix] == 'Y') {
            nDigitCount++;

            if (nDigitCount == 1) {
                NumPos = ix;
            }
        }
    }

    if (nDigitCount == 0) 
        sTrackPresent = 'N';
    else {
        for (ix = 1; ix < nDigitCount; ix++) {
            if (cDigit[NumPos + ix] == 'N') {
                sContiguousDigits = 'N';
            }
        }
        if (sContiguousDigits == 'N') {
            sTrackPresent = 'N';
        }
    }
    
    if (sTrackPresent == 'Y') {
        if (sFileName[NumPos + nDigitCount] != ' ') {
            sTrackPresent = 'N';
        }
    }

    ValueLen = nDigitCount;
    ValueLen = strlen(sValues);

    if (sTrackPresent == 'Y') {
        for (ix = 0; ix < nDigitCount; ix++) {
            sFileNameTrack[ix] = sValues[NumPos + ix ];
        }
        sFileNameTrack[nDigitCount] = '\0'; 
        
        ix++;
        while (ix < ValueLen
        &&     sEnd == 'N') {
            if (sValues[ix] != ' ') {
                sEnd = 'Y';
            }
            ix++;
        }
        RestPos = ix - 1; 
        ValueLen -= RestPos;
        memmove(sValues, (void *) &sValues[RestPos], ValueLen);  
        sValues[ValueLen] = '\0'; 
    }

//    nMaxTrackLen = (ValueLen > nMaxTrackLen) ? ValueLen : nMaxTrackLen;
}



char CMP3File::Is_Numeric(char cChar) {
//******************************************************************************
// Had to do my own version that didn't explode when non number present
//******************************************************************************
//-------------------------------------
    if (cChar == '0'
    ||  cChar == '1'
    ||  cChar == '2'
    ||  cChar == '3'
    ||  cChar == '4'
    ||  cChar == '5'
    ||  cChar == '6'
    ||  cChar == '7'
    ||  cChar == '8'
    ||  cChar == '9') 
        return 'Y';
    else {
        return 'N';
    }
}



void CMP3File::Extract_Artist(char * sValues) {
//******************************************************************************
//* 
//******************************************************************************
    HRESULT RC;
    int     nValuesLen = 0;
    int     ix = 0, Pos, Len;
    char    sSeperatorFound = 'N';

//-------------------------------------
    sArtist[0] = ' ';
    sArtist[1] = '\0';

    nValuesLen = strlen(sValues);
    ix = nValuesLen - 1;

    while (ix >= 0
    &&     sSeperatorFound == 'N') {
        if (memcmp(&sValues[ix], " - ", 3) == 0) {
            sSeperatorFound = 'Y'; 
        }
        ix--;
    }

    if (sSeperatorFound == 'Y') {
        Pos = ix + 4;
        Len = nValuesLen - Pos; 

        memcpy(sArtist, &sValues[Pos], Len);
        sArtist[Len] = '\0';
        sValues[Pos - 3] = '\0'; 
    }

    Len = strlen(sArtist);
//    nMaxArtistLen = (Len > nMaxArtistLen) ? Len : nMaxArtistLen;

}



void CMP3File::Extract_Title(char * sValues) {
//******************************************************************************
//* 
//******************************************************************************
    int     nValuesLen = 0;
//-------------------------------------
    sTitle[0] = ' ';
    sTitle[1] = '\0';

    nValuesLen = strlen(sValues);

    if (nValuesLen != 0) {
        memcpy(sTitle, sValues, nValuesLen);
        sTitle[nValuesLen] = '\0'; 
    }

    nValuesLen = strlen(sTitle);
//    nMaxTitleLen = (nValuesLen > nMaxTitleLen) ? nValuesLen : nMaxTitleLen;
}



//******************************************************************************
//*  Get Functions 
//******************************************************************************
//=====================================
char * CMP3File::File_Name(void)    {return sFileName;}

char * CMP3File::Track(void)        {return sTrack;}

char * CMP3File::Title(void)        {return sTitle;}

char * CMP3File::Artist(void)       {return sArtist;}

char * CMP3File::TagTitle(void)     {return Tag.Title();}

char * CMP3File::TagArtist(void)    {return Tag.Artist();}

char * CMP3File::TagAlbum(void)     {return Tag.Album();}

char * CMP3File::TagTrack(void)     {return Tag.Track();}
