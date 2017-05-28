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

#include "CProposedValues.h"


//================== static data ================
int CProposedValues::nTrackNumber = 0;      
//================== static data ================


using namespace std;


CProposedValues::CProposedValues() {
//******************************************************************************
//* 
//******************************************************************************
//-------------------------------------
//    fTrackPresent  = 'N';
//    fTitlePresent  = 'N';
//    fArtistPresent = 'N';
//    fGenrePresent  = 'N';
//    fDatePresent   = 'N';

    sTitle[0] = '\0';
    sArtist[0] = '\0';
    sAlbum[0] = '\0';
    sGenre[0] = '\0';
//    sDate[0] = '\0';
}


CProposedValues::~CProposedValues() {
//******************************************************************************
//* 
//******************************************************************************
//-------------------------------------
}


void CProposedValues::Extract(char * pPathAndFile) {
//******************************************************************************
//* 
//******************************************************************************
//    HRESULT RC;
    char    sValues[MAX_PATH];
//    int     nLen = 0;
//    int     ix = 0, Pos, Len;
//    int     nHyphenCount = 0;
//    char    sSeperatorFound = 'N';
//-------------------------------------
    Extract_FileName(pPathAndFile);
    Extract_Album(pPathAndFile);

    strcpy_s(sValues, MAX_PATH, PathFindFileName(pPathAndFile));
    Extract_Track(sValues);
    Extract_Artist(sValues);
    Extract_Title(sValues);
}



void CProposedValues::Extract_FileName(char * pPathAndFile) {
//******************************************************************************
//* 
//******************************************************************************
    unsigned int     nLen = 0;
/*--------------------------------*/
    strcpy_s(sFileName, MAX_PATH, PathFindFileName(pPathAndFile));

    nLen = strlen(sFileName);
//    nMaxFileNameLen = (nLen > nMaxFileNameLen) ? nLen : nMaxFileNameLen;
}



void CProposedValues::Extract_Album(char * pPathAndFile) {
//******************************************************************************
//* 
//******************************************************************************
    char    sDir[MAX_PATH];
    int     nLen;
//-------------------------------------
    strcpy_s(sDir, MAX_PATH, pPathAndFile);
    PathRemoveFileSpec(sDir);
    strcpy_s(sAlbum, MAX_PATH, PathFindFileName(sDir));
    nLen = strlen(sAlbum);
//    nMaxAlbumLen = (nLen > nMaxAlbumLen) ? nLen : nMaxAlbumLen;

}



void CProposedValues::Extract_Track(char * sValues) {
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

//    sTrack[0] = ' ';
//    sTrack[1] = '\0';
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



char CProposedValues::Is_Numeric(char cChar) {
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



void CProposedValues::Extract_Artist(char * sValues) {
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
//        fArtistPresent = 'Y';
        Pos = ix + 4;
        Len = nValuesLen - Pos; 

        memcpy(sArtist, &sValues[Pos], Len);
        sArtist[Len] = '\0';
        sValues[Pos - 3] = '\0'; }
    else {
//        fArtistPresent = 'N';
    }

    Len = strlen(sArtist);
//    nMaxArtistLen = (Len > nMaxArtistLen) ? Len : nMaxArtistLen;

}



void CProposedValues::Extract_Title(char * sValues) {
//******************************************************************************
//* 
//******************************************************************************
    int     nValuesLen = 0;
//-------------------------------------
    sTitle[0] = ' ';
    sTitle[1] = '\0';

    nValuesLen = strlen(sValues);

    if (nValuesLen != 0) {
//        fTitlePresent = 'Y';
        memcpy(sTitle, sValues, nValuesLen);
        sTitle[nValuesLen] = '\0'; }
    else {
//        fTitlePresent = 'N';
    }

    nValuesLen = strlen(sTitle);
//    nMaxTitleLen = (nValuesLen > nMaxTitleLen) ? nValuesLen : nMaxTitleLen;
}



//******************************************************************************
//*  Get Functions 
//******************************************************************************
//=====================================
char * CProposedValues::File_Name(void) {
//=====================================
    return sFileName;
}

//=====================================
char * CProposedValues::Track(void) {
//=====================================
    return sTrack;
}

//=====================================
char * CProposedValues::Title(void) {
//=====================================
    return sTitle;
}

//=====================================
char * CProposedValues::Artist(void) {
//=====================================
    return sArtist;
}

//=====================================
char * CProposedValues::Album(void) {
//=====================================
    return sAlbum;
}
