#ifndef	_AJHPROPVALUES
#define	_AJHPROPVALUES


class CProposedValues {
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
	CProposedValues();
	~CProposedValues();

    void    Extract(char *); 
    char *  File_Name(void);

    char *  Track(void); 
    char *  Title(void); 
    char *  Artist(void); 
    char *  Album(void); 


//========= Public Static Data ==============
//    static int nMaxFileNameLen;			
//    static int nMaxTitleLen;			
//    static int nMaxArtistLen;			
//    static int nMaxAlbumLen;			
	static int     nTrackNumber; 
 
  
//--------------------------------------------------------------
private:    //Data
	char    sFileName[MAX_PATH];	

	char    sTitle[TEXT_LEN];            			
	char    sArtist[TEXT_LEN];            			
	char    sAlbum[TEXT_LEN];            			
	char    sTrack[TRACK_TEXT_LEN];           
	char    sGenre[GENRE_LEN];

	int     nTrackTotal; 


private:    //functions
    void    Extract_FileName(char *);
    void    Extract_Album(char *);
    void    Extract_Title(char *);
    void    Extract_Artist(char *);
    void    Extract_Track(char *);
    char    Is_Numeric(char);


};  // end of Class CProposedValues

#endif	_AJHPROPVALUES
