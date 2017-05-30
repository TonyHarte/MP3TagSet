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

#include "CID3v2Tag.h"
#include "CFileIO.h"


using namespace std;

//================== static data ================
int CID3v2Tag::nMaxFileNameLen = 20;      //Always want at least some width
int CID3v2Tag::nMaxTitleLen = 20;         //Always want at least some width
int CID3v2Tag::nMaxArtistLen = 20;        //Always want at least some width
int CID3v2Tag::nMaxAlbumLen = 20;         //Always want at least some width
int CID3v2Tag::nMaxTrackLen = 20;         //Always want at least some width
//================== static data ================


CID3v2Tag::CID3v2Tag() {
//******************************************************************************
//* 
//******************************************************************************
/*--------------------------------*/
    Initialise();
}



CID3v2Tag::~CID3v2Tag() {
//******************************************************************************
//* 
//******************************************************************************
/*--------------------------------*/
//	if (m_szFileName != NULL)
//        delete m_szFileName;
}



void CID3v2Tag::Initialise() {
//******************************************************************************
//* 
//******************************************************************************
/*--------------------------------*/
    fID3TagPresent      = 'N';
    fArtistFramePresent = 'N';
    fAlbumFramePresent  = 'N';
    fGenreFramePresent  = 'N';
    fTrackFramePresent  = 'N';
    fDateFramePresent   = 'N';

    nTagLength = 0;
    sFileName[0] = '\0';
    sTitle[0] = '\0';
    sArtist[0] = '\0';
    sAlbum[0] = '\0';
    sTrack[0] = '\0';
    sAlbumArtist[0] = '\0';
    sGenre[0] = '\0';
    nBitRate = 0;
    sDate[0] = '\0';
    sVBR[0] = '\0';
}



void CID3v2Tag::Extract(char * pPathAndFile) {
//******************************************************************************
//* 
//******************************************************************************
    CFile   MP3File;
    HANDLE  hMP3File;
/*--------------------------------*/
    Initialise();
    hMP3File = MP3File.Map_File(pPathAndFile);
    pFileContents = MP3File.Get_Pointer_To_Contents();
    nFileOffset = 0;
    Extract_FileName(pPathAndFile);
    fID3TagPresent = Extract_Header_Tag(pFileContents);
 
    if (fID3TagPresent == 'Y') {  
        nTagLength = Convert_ID3Len(gID3v2Header.Size);
        nFileOffset = HEADER_LEN;
        Extract_Frames(pFileContents + HEADER_LEN);
    }
}




void CID3v2Tag::Extract_FileName(char * pPathAndFile) {
//******************************************************************************
//* 
//******************************************************************************
    unsigned int     nLen = 0;
/*--------------------------------*/
    strcpy_s(sFileName, MAX_PATH, PathFindFileName(pPathAndFile));

    nLen = strlen(sFileName);
    nMaxFileNameLen = (nLen > nMaxFileNameLen) ? nLen : nMaxFileNameLen;

TEMPDEBUG("  ");
TEMPDEBUG("=========================");
TEMPDEBUG("Demangling file %s with name length %d (max is now %d)", sFileName, nLen, nMaxFileNameLen);
}



char CID3v2Tag::Extract_Header_Tag(char * pFileContents) {
//******************************************************************************
//* 
//******************************************************************************
    char fFound;
/*--------------------------------*/
    if (memcmp(pFileContents, "ID3", 3) == 0) {
        fFound = 'Y';
        memcpy(gID3v2Header.Id, pFileContents, 3);
        gID3v2Header.MajorVersion   = (unsigned char) pFileContents+3;
        gID3v2Header.Revision       = (unsigned char) *(pFileContents+4);
        gID3v2Header.Flags          = (char) *(pFileContents+5);
        memcpy((char *) &(gID3v2Header.Size), pFileContents+6, 4); 
TEMPDEBUG("ID3 Tag found with len %d", Convert_ID3Len(gID3v2Header.Size));

        nLengthOfData += HEADER_LEN; }
    else {
TEMPDEBUG("ID3 Tag Not found");
        fFound = 'N';
	}
    
    return fFound;
}



void CID3v2Tag::Extract_Frames(char * pFileContents) {
//******************************************************************************
//* ID3 specification says a tag must have at least 1 frame.  Unfortunately must  
//* account for incorrect length specs and keep seaching the whole of the tag
//* until the end of the tag header length.  (I have found an APIC with the wrong
//* length that left the pointer half way through the binary picture data). 
//******************************************************************************
    FrameTypeTag FrameType;
    char *  pFile;
    int     nOffset = 0;
    char    sFrameFound = 'Y';
/*--------------------------------*/
    while (nOffset < nTagLength) {
//    &&     sFrameFound == 'Y') {
        pFile = pFileContents + nOffset;
        memcpy((char *) &(gFrameHeader), pFile,  HEADER_LEN);
        sFrameFound = Check_For_Frame_Header(gFrameHeader);

        if (sFrameFound == 'Y') {
            nFrameLength = Convert_ID3Len(gFrameHeader.Size); 
            FrameType = Determine_Frame_Type(gFrameHeader);

            switch (FrameType) {
             case   TITLE:
                Extract_Title(pFile);
                break;

             case   ARTIST:
               Extract_Artist(pFile);
               break;

             case   ALBUM:
                Extract_Album(pFile);
                break;

             case   TRACK:
                Extract_Track(pFile);
                break;

             case   LENGTH:
                Extract_TrackLen(pFile);
                break;

             default:
                Extract_Unknown_Frame(pFile);
                break;
            }

            nOffset += (HEADER_LEN + nFrameLength); }
        else {
            nOffset++;
        }
    }
         
    nRemainderLength = nTagLength;  //not correct - must store position of 
//  not correct - must store position of last valid frame given that the length in 
//  some frame may not be corret.
    
}



char CID3v2Tag::Check_For_Frame_Header(FrameHeaderTag gFrameHeader) {
//******************************************************************************
//* ID3 specification says a tag must have at least 1 frame.  Also the framename
//* must have 4 digits 
//******************************************************************************
    char    sFrameFound;
/*--------------------------------*/
    if (IsCharAlphaNumeric(gFrameHeader.FrameId[0]) != 0
    &&  IsCharAlphaNumeric(gFrameHeader.FrameId[1]) != 0
    &&  IsCharAlphaNumeric(gFrameHeader.FrameId[2]) != 0
    &&  IsCharAlphaNumeric(gFrameHeader.FrameId[3]) != 0) 
        sFrameFound = 'Y';
    else {
        sFrameFound = 'N';
    }

    return sFrameFound;
}



CID3v2Tag::FrameTypeTag CID3v2Tag::Determine_Frame_Type(FrameHeaderTag gFrameHeader) {
//******************************************************************************
//* This section is to take the massive indention out of the mainline code  
//******************************************************************************
    FrameTypeTag FrameType;
/*--------------------------------*/
    FrameType = OTHER;

    if (memcmp("TIT2", gFrameHeader.FrameId, 4) == 0) 
        FrameType = TITLE;
    else {
        if (memcmp("TPE1", gFrameHeader.FrameId, 4) == 0) 
            FrameType = ARTIST;
        else {
            if (memcmp("TALB", gFrameHeader.FrameId, 4) == 0) 
                FrameType = ALBUM;
            else {
                if (memcmp("TRCK", gFrameHeader.FrameId, 4) == 0) 
                    FrameType = TRACK;
                else {
                    if (memcmp("TLEN", gFrameHeader.FrameId, 4) == 0) 
                        FrameType = LENGTH;
                    else {
                    }
                }
            }
        }
    }
   
    return FrameType;
}



void CID3v2Tag::Extract_Title(char * pFileContents) {
//******************************************************************************
//* The song title
//******************************************************************************
    string fTextEncoding = "???";
    int nTextLength = 0;
    int nLen;
/*--------------------------------*/
    fTitleFramePresent = 'Y';
    Extract_Text(sTitle, pFileContents);
    nLen = strlen(sTitle);
    nMaxTitleLen = (nLen > nMaxTitleLen) ? nLen : nMaxTitleLen;

TEMPDEBUG("    Title Frame  TIT2 >>>%s<<<    with length %d (max now %d)", sTitle, nLen, nMaxTitleLen);
}



void CID3v2Tag::Extract_Artist(char * pFileContents) {
//******************************************************************************
//* Self Evident
//******************************************************************************
    string fTextEncoding = "???";
    int nTextLength = 0;
    int nLen;
/*--------------------------------*/
    fArtistFramePresent = 'Y';
    Extract_Text(sArtist, pFileContents);
    nLen = strlen(sArtist);
    nMaxArtistLen = (nLen > nMaxArtistLen) ? nLen : nMaxArtistLen;

TEMPDEBUG("    Artist Frame TPE1 >>>%s<<<    with length %d (max now %d)", sArtist, nLen, nMaxArtistLen);
}



void CID3v2Tag::Extract_Album(char * pFileContents) {
//******************************************************************************
//* Self Evident
//******************************************************************************
    int nTextLength = 0;
    int nLen;
/*--------------------------------*/
    fAlbumFramePresent = 'Y';
    Extract_Text(sAlbum, pFileContents);
    nLen = strlen(sArtist);
    nMaxAlbumLen = (nLen > nMaxAlbumLen) ? nLen : nMaxAlbumLen;

TEMPDEBUG("    Album Frame  TALB >>>%s<<<    with length %d (max now %d)", sAlbum, nLen, nMaxAlbumLen);
}



void CID3v2Tag::Extract_Track(char * pFileContents) {
//******************************************************************************
//* This is the number of the track on the album.  
//******************************************************************************
    char NumberString[30];
    int nTextLength = 0;
/*--------------------------------*/
    fTrackFramePresent = 'Y';
    Extract_Text(NumberString, pFileContents);
    nTextLength = strlen(NumberString);

    if (nTextLength < TRACK_TEXT_LEN) {
        strcpy_s(sTrack, TRACK_TEXT_LEN, NumberString);
    }
}



void CID3v2Tag::Extract_TrackLen(char * pFileContents) {
//******************************************************************************
//* This is the number of the track on the album.  
//******************************************************************************
    int nWorkNumber = 0;
    string fTextEncoding = "???";
    int nTextLength = 0;
    char NumberString[30];
    int ix;
/*--------------------------------*/
    fTrackLengthFramePresent = 'Y';
    Extract_Text(NumberString, pFileContents);

    nTrackLength = atoi(NumberString);
}



void CID3v2Tag::Extract_Unknown_Frame(char * pFileContents) {
//******************************************************************************
//* 
//******************************************************************************
//    int     nFrameLength;
    char sTemp[TEXT_LEN];
/*--------------------------------*/
TEMPDEBUG("**** Unknown Frame type %s has length %d", gFrameHeader.FrameId, nFrameLength);

    if (nFrameLength < TEXT_LEN) {
        Extract_Text(sTemp, pFileContents);
        TEMPDEBUG("  Unknown String is >>>%s<<<", sTemp);
    }

//    nFrameLength = Convert_ID3Len(gFrameHeader.Size); 
//    return nFrameLength;
}



void CID3v2Tag::Extract_Text(char * sText, char * pFileContents) {
//******************************************************************************
//* This determines if text is standard ISO or UNICODE.  If it's unicode 
//* then need to know if the byte order is little or big endian.  
//******************************************************************************
    unsigned char nEncodingFlag = 0;
    unsigned char nBOM1 = 0;
    unsigned char nBOM2 = 0;
    string fTextEncoding = "???";
    int nTextLength = 0;
    char * pSourceStart;
    int DInd = 0, SInd = 0;
    int nBumpValue;
/*--------------------------------*/
    sText[0] = ' ';  
    sText[1] = '\0';  

    nEncodingFlag = *(pFileContents + HEADER_LEN);

    if (nEncodingFlag == 0x00)   {
        fTextEncoding = "ISO";
TEMPDEBUG("ISO");
        nBumpValue = 1; 
        nTextLength = nFrameLength - 1;
        pSourceStart = pFileContents + HEADER_LEN + 1; }     //Skip past Header and Encoding flag
    else {
        fTextEncoding = "UNI";
TEMPDEBUG("UNICODE");
        nBumpValue = 2; 
        nTextLength = nFrameLength - 3;

        nBOM1 = *(pFileContents + HEADER_LEN + 1);
        nBOM2 = *(pFileContents + HEADER_LEN + 2);

        if (nBOM1 == 0xFF 
        ||  nBOM2 == 0xFE) {
            pSourceStart = pFileContents + HEADER_LEN + 3; }     //Little Endian - skip past Header, Encoding flag and BOM
        else {
            pSourceStart = pFileContents + HEADER_LEN + 4;       //Big Endian - skip past Header, Encoding flag and BOM
        }
    }

    while (SInd < nTextLength) {
//    &&     pSourceStart[SInd] != '\0') {
        sText[DInd] = pSourceStart[SInd];
        DInd++;
        SInd += nBumpValue;
    }
    sText[DInd] = '\0';                             //Just in case don't have terminator like supposed to
}



int CID3v2Tag::Convert_ID3Len(int ID3Len) {
//******************************************************************************
//* 
//******************************************************************************
    ID3LengthConversionTag gLenConv;
    ID3v2TagLenTag * pgTagLenID3Version;
/*--------------------------------*/
    pgTagLenID3Version = (ID3v2TagLenTag *) &ID3Len;

    gLenConv.SubPart.MSBs  = 0;
    gLenConv.SubPart.Byte1 = pgTagLenID3Version->Byte1;
    gLenConv.SubPart.Byte2 = pgTagLenID3Version->Byte2;
    gLenConv.SubPart.Byte3 = pgTagLenID3Version->Byte3;
    gLenConv.SubPart.Byte4 = pgTagLenID3Version->Byte4;
    
    return gLenConv.dTagLen;
}


static int Max_FileName_Length(void){
//******************************************************************************
//*--------------------------------*/
    return CID3v2Tag::nMaxFileNameLen;
}



//******************************************************************************
//*  Get Functions 
//******************************************************************************
char * CID3v2Tag::File_Name(void) {return sFileName;}

char * CID3v2Tag::Title(void)     {return sTitle;}

char * CID3v2Tag::Artist(void)    {return sArtist;}

char * CID3v2Tag::Album(void)     {return sAlbum;}

char * CID3v2Tag::Track(void)     {return sTrack;}
