#ifndef AJHUG_ListView 
    #define AJHUG_ListView

#ifdef UG_ListView
    #define AJHEXTERN4_
#else
    #define AJHEXTERN4_ extern
#endif


#define COLTEXTWIDTH    35  
#define NUMCOLS         5 

struct ColumnDetails {
    char    Title[COLTEXTWIDTH];
    int     Width; }; 

AJHEXTERN4_ struct ColumnDetails gColumn[NUMCOLS];

//*******************FUNCTION PROTOTYPES***********************
void G100_Setup_Header_Details(void);
HWND G100_Create_LV_For_Current(HWND);
HWND G100_Create_LVControl_For_Proposed(HWND);
void G300_Insert_LV_Current_Data(HWND);
void G400_Insert_LV_Proposed_Data(HWND);
void E320_Populate_LV_Current(HWND);

void G200_Create_LV_ColumnHeaders(HWND, int);
//*******************FUNCTION PROTOTYPES***********************
#endif