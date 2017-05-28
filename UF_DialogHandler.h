#ifndef AJHUF_DialogHandler 
    #define AJHUF_DialogHandler

#ifdef AJHUF_DialogHandler
    #define AJHEXTERN3_
#else
    #define AJHEXTERN3_ extern
#endif



//*******************FUNCTION PROTOTYPES***********************
BOOL CALLBACK UF_Dialog_Handler(HWND, UINT, WPARAM, LPARAM);
BOOL    F100_Handle_NOTIFY(HWND, WPARAM, LPARAM);
LRESULT F150_NOTIFY_CustomDraw(LPARAM);
//*******************FUNCTION PROTOTYPES***********************
#endif
