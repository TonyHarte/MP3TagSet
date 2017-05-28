#ifndef	_AJHMP3TAGDATA
#define	_AJHMP3TAGDATA


#include <string>

class CID3Tag {
//******************************************************************************
//* 
//******************************************************************************
//========= Public Functions ==============
public:
	CID3Tag();
	~CID3Tag();

    void    Extract(char *); 
    char *  File_Name(void); 
    char *  Title(void); 
    char *  Artist(void); 
    char *  Album(void); 
    char *  Track(void); 

static    int     Max_FileName_Length(void);


//========= Public Data ==============
public:
    #define TEXT_LEN        100
    #define GENRE_LEN       50
    #define TRACK_TEXT_LEN  8
    #define DATE_LEN        10
    #define HEADER_LEN      10
    #define FRAMEID_LEN     4


//========= Public Static Data ==============
    static int nMaxFileNameLen;			
    static int nMaxTitleLen;			
    static int nMaxArtistLen;			
    static int nMaxAlbumLen;			
    static int nMaxTrackLen;			
 
  
//--------------------------------------------------------------
private:
    char *  pFileContents;
    int     nFileOffset;

    char    fID3TagPresent;
    int     nLengthOfData;

	char    sFileName[MAX_PATH];	

    struct ID3v2HeaderTag {
        char            Id[3];
        unsigned char   MajorVersion;
        unsigned char   Revision;
        char            Flags;
        int             Size;     } gID3v2Header;

    struct FrameHeaderTag {
        char            FrameId[4];
        int             Size;
        char            Flags1;
        char            Flags2;     } gFrameHeader;

    int     nTagLength;
    int     nRemainderLength;
    int     nFrameLength;


//****************** Extracted Information*************
	char    sTitle[TEXT_LEN];            			
	char    sArtist[TEXT_LEN];            			
	char    sAlbum[TEXT_LEN];            			
	char    sTrack[TRACK_TEXT_LEN]; 
	int     nTrackTotal; 
	int     nTrackLength; 
	char    sAlbumArtist[TEXT_LEN];          //???
	char    sGenre[GENRE_LEN];
    int     nBitRate;
    char    sDate[DATE_LEN];                  //???
    char    sVBR[3];                     //???

    char    fTitleFramePresent;
    char    fArtistFramePresent;
    char    fAlbumFramePresent;
    char    fGenreFramePresent;
    char    fTrackFramePresent;
    char    fTrackLengthFramePresent;
    char    fDateFramePresent;
//****************** Extracted Information*************
            			


    struct ID3v2TagLenTag {
        unsigned int Byte4           : 7;
        unsigned int Byte4ZeroBit    : 1;
        unsigned int Byte3           : 7;
        unsigned int Byte3ZeroBit    : 1;
        unsigned int Byte2           : 7;
        unsigned int Byte2ZeroBit    : 1;
        unsigned int Byte1           : 7;
        unsigned int Byte1ZeroBit    : 1;  };


    union ID3LengthConversionTag {
        struct {
            unsigned int Byte1  : 7;
            unsigned int Byte2  : 7;
            unsigned int Byte3  : 7;
            unsigned int Byte4  : 7;
            unsigned int MSBs   : 4;   } SubPart;
        int dTagLen;
    };
    
    enum FrameTypeTag {
        TITLE,
        ARTIST,
        ALBUM,
        TRACK,
        LENGTH,
        OTHER };

private:    //Functions
    HANDLE  Create_File_Mapping();
    void    Initialise();
    void    Extract_FileName(char *);

    int     Convert_ID3Len(int);
    char    Extract_Header_Tag(char *);
    void    Extract_Frames(char *);
    char    Check_For_Frame_Header(FrameHeaderTag);
    void    Extract_Title(char *);
    void    Extract_Artist(char *);
    void    Extract_Album(char *);
    void    Extract_Track(char *);
    char    Is_Numeric(char);
    void    Extract_TrackLen(char *);
    void    Extract_Unknown_Frame(char *);
    void    Extract_Text(char *, char *);
    FrameTypeTag Determine_Frame_Type(FrameHeaderTag);

};  // end of Class CID3Tag

#endif	_AJHMP3TAGDATA
