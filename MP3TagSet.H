#include <vector>
#include "CID3Tag.h"
#include "CProposedValues.h"

#ifndef AJHMP3SetTag 
    #define AJHMP3SetTag

#ifdef MP3SetTag
    #define AJHEXTERN1_
#else
    #define AJHEXTERN1_ extern
#endif

AJHEXTERN1_ Window  CMainWin;
AJHEXTERN1_ Window  CDialogCurrent;
AJHEXTERN1_ Window  CLVCurrent;
AJHEXTERN1_ Window  CLVProposed;
AJHEXTERN1_ Window  CProposedStatic;

typedef std::vector<CID3Tag>    ID3VECTOR;
AJHEXTERN1_ ID3VECTOR           vTag;
AJHEXTERN1_ ID3VECTOR::iterator ID3ind;

typedef std::vector<CProposedValues>    PROPVALVECTOR;
AJHEXTERN1_ PROPVALVECTOR               vProposed;
AJHEXTERN1_ PROPVALVECTOR::iterator     PVind;


#define SPACING         8


//*******************FUNCTION PROTOTYPES***********************
void A100_Program_Genesis(HINSTANCE, HINSTANCE);  
void A200_Register_Windows(void);  
void A300_Create_Main_Window(void);  

void G_Program_Exodus(void);  
//*******************FUNCTION PROTOTYPES***********************
#endif
    
