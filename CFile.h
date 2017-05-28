#ifndef	_AJHCFILE
#define	_AJHCFILE



class CFile {
//******************************************************************************
//* 
//******************************************************************************
public:
	CFile();
	~CFile();

    HANDLE  Map_File(char *);
//    void        GetTag(HWND);
    char *  Get_Pointer_To_Contents(void);

//    void    WriteTag(HWND);
//    void    UnMapFile(HWND);

public:
    
private:
	HANDLE hFile;				// The file...
	HANDLE hFileMap;			// Map for the file...
    DWORD dFileSize;
    char * pFileContents;

    char sID3v2TagFound;


    char sFilename[_MAX_FNAME];
    char sTitle[_MAX_FNAME];

private:
    HANDLE    Create_File_Mapping();

};  // end of Class CFile

#endif	_AJHCFILE
