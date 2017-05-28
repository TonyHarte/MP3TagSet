#ifndef	_AJHCERROR
#define	_AJHCERROR

#define MESSAGELENGTH 512
int CRITICAL_ERROR = 1001;
int DISPLAY_AND_CONTINUE = 1002;


class CError {
//******************************************************************************
//* 
//******************************************************************************
public:
    CError(int, char *);

    
private:
    char cErrorNumber[6];
    char cSystemError[MESSAGELENGTH];
    char cFullMessage[MESSAGELENGTH];
    char cContinuationMessage[MESSAGELENGTH];
    char cWarningType[20];


private:
    void   Z_Abend_With_MessageBox (char *);
    void   Z_Show_MessageBox_And_Continue (char *);

};  // end of Class CError

#endif	_AJHCERROR