#ifndef AJHUD_MainWindowHandler 
    #define AJHUD_MainWindowHandler

#ifdef UD_MainWindowHandler
    #define AJHEXTERN2_
#else
    #define AJHEXTERN2_ extern
#endif

#define NUMENTRIES  6

#define COLUMNMINWIDTH    50
#define DEFWIDTH    100


//*******************FUNCTION PROTOTYPES***********************
LRESULT CALLBACK UD_Main_Handler(HWND, UINT, WPARAM, LPARAM);

void D100_Handle_CREATE(HWND, WPARAM, LPARAM);
void D150_Create_Static_Controls(HWND, WPARAM, LPARAM);
void D200_Handle_COMMAND(HWND, WPARAM, LPARAM);
void D300_Handle_SIZE(HWND, WPARAM, LPARAM);
LRESULT D400_Handle_NOTIFY(HWND, WPARAM, LPARAM);
void D500_Handle_PAINT(HWND);

void D000_Extract_ID3_Data(void);

//*******************FUNCTION PROTOTYPES***********************
#endif
