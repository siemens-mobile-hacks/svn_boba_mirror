#include "..\..\..\\include\Types.h"
#include "..\classes.h"

#define OrangeLED_ID 4

void (*CoCreateInstance)(char * guid1,char * guid2,void *pInterface)=(void(*)(char * guid1,char * guid2,void *pInterface))0x144619AD;
void (*DisplayGC_AddRef)(GC*,void *)=(void(*)(GC*,void *))0x143D0681;
void (*TextObject_SetText)(IRichText * pTextObject,STRID strid)=(void(*)(IRichText * pTextObject,STRID strid))0x14170009;
void (*TextObject_SetFont)(IRichText * pTextObject,void * pFont,const int _0x8000000A,const int _0x7FFFFFF5)=(void(*)(IRichText * pTextObject,void * pFont,const int _0x8000000A,const int _0x7FFFFFF5))0x14172F05;
int (*Illumination_LedID_SetLevel_Gradually)(int LED_ID,int level,int fade_time)=(int(*)(int LED_ID,int level,int fade_time))0x143E9CFD;
int (*Illumination_LedID_SetLevel)(int LED_ID,int level)=(int(*)(int LED_ID,int level))0x144C7E79;
int (*Illumination_LedID_Off)(int LED_ID)=(int(*)(int LED_ID))0x14339081;
int (*RichTextLayout_GetTextWidth)(STRID strid,IRichTextLayout * pRichTextLayout,IUnknown * pFont)=(int(*)(STRID strid,IRichTextLayout * pRichTextLayout,IUnknown * pFont))0x144232E1;    
int (*REQUEST_HPHONEBOOK_READSTRING)(const int* sync,void * buf)=(int(*)(const int* sync,void * buf))0x141AD49D;
void (*AB_STR_ITEM_Copy)(AB_STR_ITEM * dest,AB_STR_ITEM * source)=(void(*)(AB_STR_ITEM * dest,AB_STR_ITEM * source))0x1536C365;
void (*Display_SetBrightnessLevel)(int level)=(void(*)(int level))0x15504C79;
STRID (*int2strID_int)(int num,char * filename,int line_num)=(STRID(*)(int num,char * filename,int line_num))0x143D67E5;
STRID (*Str2ID_int)(const void* wstr,int flag,int len,char * filename,int line_num)=(STRID(*)(const void* wstr,int flag,int len,char * filename,int line_num))0x142D8069;
int (*TextID2wstr_int)(STRID strid,wchar_t * dest,int maxlen,char * filename,int line_num)=(int(*)(STRID strid,wchar_t * dest,int maxlen,char * filename,int line_num))0x1416F325;
int (*TextGetLength_int)(STRID strid,char * filename,int line_num)=(int(*)(STRID strid,char * filename,int line_num))0x1416FED1;
void (*TextFree_int)(STRID strid,char * filename,int line_num)=(void(*)(STRID strid,char * filename,int line_num))0x1416F6B9;
STRID (*TextCopyId_int)(STRID strid,char * filename,int line_num)=(int(*)(STRID strid,char * filename,int line_num))0x1416EDF1;
