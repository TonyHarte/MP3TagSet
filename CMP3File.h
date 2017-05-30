#include "CID3v2Tag.h"

#ifndef	_AJHMP3VALUES
#define	_AJHMP3VALUES


using namespace std;
class CMP3File {
//******************************************************************************
//* 
//******************************************************************************
//========= Public Data ==============
public:     //Data
    #define TEXT_LEN        100
    #define GENRE_LEN       50
    #define TRACK_TEXT_LEN  8
    #define DATE_LEN        10


public:     //Functions
	CMP3File();
	~CMP3File();

    void    Extract(char *); 
    char *  File_Name(void);

    char *  Track(void); 
    char *  Title(void); 
    char *  Artist(void); 

    char *  TagTitle(void); 
    char *  TagArtist(void); 
    char *  TagAlbum(void); 
    char *  TagTrack(void); 


//========= Public Static Data ==============
//    static int nMaxFileNameLen;			
//    static int nMaxTitleLen;			
//    static int nMaxArtistLen;			
//    static int nMaxAlbumLen;			
	static int     nTrackNumber; 
 
  
//--------------------------------------------------------------
private:    //Data
	char    sFileName[_MAX_PATH];	

	char    sTitle[TEXT_LEN];            			
	char    sArtist[TEXT_LEN];            			
	char    sTrack[TRACK_TEXT_LEN];           
	char    sGenre[GENRE_LEN];

    CID3v2Tag   Tag;

	int     nTrackTotal; 


private:    //functions
    void    Extract_FileName(char *);
    void    Extract_Title(char *);
    void    Extract_Artist(char *);
    void    Extract_Track(char *);
    char    Is_Numeric(char);


};  // end of Class CMP3File

#endif	_AJHMP3VALUES
