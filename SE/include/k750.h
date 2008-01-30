
#include "..\\include\Target.h"
#include "..\\include\Types.h"



#define NULL	0

//typedef u16 unsigned short

// Str2ID conversion flag
#define WSTR_2_ID 0

#define NO_ERROR 0
#define NULL_STR 0x6FFFFFFF
#define keySELECT 0xFC3
#define keyESC    0xFC1

#define BOOK_DESTROYED_EVENT 4



//PhoneBook const

#define ALL_AB_ITEMS            0x20

#define AB_NAME                 0x01
#define AB_N_HOME               0x05
#define AB_N_WORK               0x06
#define AB_N_MOBILE             0x07
#define AB_N_FAX                0x08
#define AB_N_OTHER              0x0A
#define AB_EMAIL                0x0B
#define AB_URL                  0x0E
#define AB_INFO                 0x1D

//Key Code

#define KEY_UP                  0x06
#define KEY_DOWN                0x0A
#define KEY_LEFT                0x0C
#define KEY_RIGHT               0x08
#define KEY_ENTER               0x05
#define KEY_ESC                 0x03
#define KEY_DEL                 0x04
#define KEY_LEFT_SOFT           0x01
#define KEY_RIGHT_SOFT          0x02
#define KEY_TASK                0x1C
#define KEY_PLAY                0x20
#define KEY_CAMERA              0x22 // 0x23
#define KEY_VOL_UP              0x0E
#define KEY_VOL_DOWN            0x0F
#define KEY_LENS_COVER_OPEN     0x31
#define KEY_LENS_COVER_CLOSE    0x32
#define KEY_STAR                0x1A
#define KEY_DIEZ                0x1B




extern  __arm void *alloc(int size,int signo);
extern  __arm void start (int pid);
extern	__arm void * recive(void *sigsel);
extern  __arm void send(void **sig,int PID);
extern  __arm int sender(void **sig);
extern  __arm void free_buf(void **sig);
extern  __arm int create_process(int proc_type,char *name,void (*entrypoint)(void),int stack_size,int priority,int timeslice,int pid_block,void *redir_table,int vector,int user);




#ifndef SWI_HOOK__
//??,B5,??,1C,??,1C,??,1C,??,28,??,4F,??,D1,??,21,??,31,??,1C,??,30,??,??,??,??,??,1C,??,D1,??,??,??,00,??,37,??,1C,??,??,??,??,??,88,??,28,??,D0,??,??,??,??
extern __thumb void CreateTimer(void *unk,int time,void *proc,void *book);
//??,B5,??,1C,??,1C,??,1C,??,1C,??,D1,??,21,??,48,??,31,??,30,??,??,??,??,??,??,??,??,??,04,??,0C,??,D0,??,1C,??,1C,??,1C,??,1C,??,??,??,??,??,1C,??,BD
extern __thumb SetTimer(int time, int (*_PROC_)(void *,void *),void* timer);
//extern __thumb SetBookTimer(void *timer,int time,void *proc,void *book);
//??,B5,??,1C,??,D1,??,21,??,48,??,00,??,30,??,??,??,??,??,88,??,28,??,D0,??,??,??,??,??,20,??,80,??,BD
extern __thumb FreeTimer(void *timer);



//memory
#ifdef W700_R1CA021
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2,const char *fname,int fline);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem,const char * fname ,int fline);
#endif

#ifdef K750_R1CA021
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2,const char *fname,int fline);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem,const char * fname ,int fline);
#endif

#ifdef K600_R2BB001
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2,const char *fname,int fline);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem,const char * fname ,int fline);
#endif

#ifdef W800_R1BC002
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2,const char *fname,int fline);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem,const char * fname ,int fline);
#endif

#ifdef W800_R1BD001
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2,const char *fname,int fline);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem,const char * fname ,int fline);
#endif


#ifdef W810_R4DB005
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem);
#endif

#ifdef W810_R4EA031
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem);
#endif

#ifdef Z550_R6CA009
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int size,int f1,int f2);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(void * mem);
#endif

#ifdef W580_R8BA024
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int zero, int size,int f1,int f2,const char *fname,int fline);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(int zero, void * mem,const char * fname ,int fline);
#endif

#ifdef W850_R1KG001
//pattern=??,B5,??,B0,??,E0,??,??,??,??,??,9E,??,B4,??,AC,??,7C,??,AF,??,9B,??,7B,??,98,??,25,??,43,??,??,??,??,??,28,??,B0,??,D0,??,B0,??,BD,??,??,??,??,??,98,??,24,??,43,??,21,??,??,??,??,??,1C,??,1C,??,4C,??,??,??,??,??,68,??,68,??,42,??,D1,??,??,??,??,??,99,??,B4,??,A8,??,AE,??,9B,??,7C,??,7B,??,98,??,??,??,??
extern __thumb void *memalloc(int zero, int size,int f1,int f2,const char *fname,int fline);
//pattern=??,B5,??,1C,??,B0,??,D1,??,E0,??,25,??,43,??,42,??,D0,??,??,??,??,??,28,??,D0,??,1C,??,3D,??,68,??,0F,??,2D,??,D1,??,48,??,4A,??,49,??,18,??,??,??,??,??,68,??,29,??,DA,??,??,??,??,??,68,??,4F,??,42,??,01,??,18,??,2A,??,60,??,91,??,DB,??,9B,??,68,??,42,??,DA
extern __thumb void memfree(int zero, void * mem,const char * fname ,int fline);
#endif

//pattern=??,1C,??,E0,??,70,??,1C,??,1E,??,2A,??,D1,??,47
extern __thumb void memset(void *mem,char chr,int size);
extern __arm memcpy(void *dest,const void *source,int cnt);
extern __thumb char* mess2str(int msg);
//??,B4,??,B5,??,B0,??,1C,??,A8,??,90,??,46,??,1C,??,1C,??,48,??,??,??,??,??,28,??,D4,??,21,??,54,??,B0,??,BC,??,BC,??,B0,??,47
extern __thumb int sprintf(char *buf, char *str, ...);
//??,B4,??,B5,??,B0,??,1C,??,A8,??,90,??,94,??,91,??,46,??,A9,??,48,??,??,??,??,??,28,??,D4,??,21,??,00,??,52,??,B0,??,BC,??,BC,??,B0,??,47
extern __thumb int snwprintf(u16* buffer, int size, const u16* fmt, ...);

//file
//??,B5,??,1C,??,78,??,29,??,D0,??,29,??,D0,??,29,??,D0,??,E0,??,49,??,78,??,23,??,2D,??,4C,??,D0,??,2D,??,D1,??,78,??,40,??,2A,??,D1,??,43,??,E0,??,49,??,E7,??,49,??,E7
//extern __thumb int fopen(const unsigned short *filename, const char *mode);

extern __thumb int _fopen(const unsigned short *filpath, const unsigned short *filname,unsigned int mode , unsigned int rights, unsigned int __0);

//??,B5,??,1E,??,??,??,??,??,28,??,D1,??,20,??,BD,??,20,??,43,??,BD
extern __thumb int fclose(int  file);
//??,B5,??,1C,??,1C,??,1C,??,1E,??,D4,??,2B,??,DA,??,4E,??,00,??,58,??,29,??,D1,??,1C,??,1C,??,1C,??,??,??,??,??,BD,??,68,??,07,??,0F,??,28,??,D1,??,E0,??,43,??,D0,??,1C,??,1C,??,??,??,??,??,28,??,DB,??,E0,??,20,??,BD,??,20,??,BD
extern __thumb int fread(void *ptr, int size, int n, int file);
//??,B5,??,1C,??,1C,??,1C,??,1E,??,D4,??,2B,??,DA,??,4E,??,00,??,58,??,29,??,D1,??,1C,??,1C,??,1C,??,??,??,??,??,BD,??,68,??,07,??,0F,??,28,??,D1,??,E0,??,43,??,D0,??,1C,??,1C,??,??,??,??,??,28,??,DB,??,E0,??,20,??,BD,??,22,??,71,??,20,??,BD
extern __thumb int fwrite(const void *ptr, int size, int n, int file);
//??,B5,??,1C,??,1C,??,22,??,49,??,??,??,??,??,28,??,D1,??,24,??,E0,??,1C,??,1C,??,??,??,??,??,BD
extern __thumb int _wstat(short *path, void *statbuf);
//??,B5,??,1C,??,1E,??,D4,??,2C,??,DA,??,48,??,00,??,59,??,2A,??,D1,??,1C,??,??,??,??,??,BD,??,68,??,25,??,43,??,07,??,0F,??,29,??,D1,??,24,??,??,??,??,??,60,??,1C,??,BD,??,26,??,5D,??,1C,??,2A,??,D1,??,1E,??,1C,??,1C,??,1C,??,??,??,??,??,42,??,D1,??,20,??,BD
extern __thumb int fputs(const char *s, int file);
//??,B5,??,98,??,1C,??,25,??,43,??,28,??,D1,??,24,??,4E,??,??,??,??,??,00,??,52,??,E0,??,99,??,98,??,??,??,??,??,1C,??,D0,??,2C,??,D0,??,22,??,23,??,1F,??,50,??,D1,??,9D,??,B4,??,9B,??,22,??,1C,??,??,??,??,??,49
extern __thumb int isFileExist(u16 * pach, u16 *fname , FSTAT * fstat_stuct);

extern __thumb int  lseek(int file,int offset,int mode);


//Data Browser
//??,B5,??,4C,??,48,??,1C,??,B4,??,33,??,22,??,21,??,20,??,??,??,??,??,28,??,B0,??,D1,??,20,??,BD,??,21,??,43,??,60,??,23,??,1C,??,31,??,22,??,71,??,77,??,71,??,71,??,73,??,73,??,1C,??,33,??,34
extern __thumb void * CreateDataBrowser(void);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,FF,??,73,??,20,??,BD
extern __thumb void DataBrowser_SetStyle(void * db , int style);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,FF,E5,61,??,20,??,BD
extern __thumb void DataBrowser_SetHeaderText(void * db , int StrID);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,FF,65,61,??,20,??,BD
extern __thumb void DataBrowser_SetFolders(void * db , int * FolderList);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,FF,A5,61,??,20,??,BD
extern __thumb void DataBrowser_SetFoldersCount(void * db , int FolderCount);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,FF,??,62,??,20,??,BD
extern __thumb void DataBrowser_SetOKSoftKeyText(void * db , int StrID);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,F8,A5,62,??,20,??,BD
extern __thumb void DataBrowser_SetFilters(void * db ,unsigned short * filtres);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,FF,??,2D,??,D0,??,64,??,64,??,20,??,BD
extern __thumb void DataBrowser_SetFiltersBy(void * db ,const unsigned short * xx);
//??,B5,??,1C,??,1C,??,28,??,D1,??,48,??,49,??,30,??,??,??,F8,65,62,??,20,??,BD
extern __thumb void DataBrowser_SetonNewPath(void * db ,void * proc);
//??,B5,??,B0,??,1C,??,20,??,46,??,72,??,??,??,FB,??,2C,??,90,??,4D,??,D1,??,1C,??,49,??,30,??,??,??,FE,??,20,??,??,??,F9,??,98,??,26,??,28,??,D1,??,46,??,72,??,E3
extern __thumb void DataBrowser_Show(void * db);
//??,B5,??,1C,??,4D,??,D1,??,1C,??,49,??,30,??,??,??,FE,??,6B,??,28,??,D0,??,??,??,FF,??,63,??,20,??,63,??,35,??,4A,??,1C,??,1C,??,??,??,FF,??,20,??,BD
extern __thumb void DataBrowser_XXX(void * db);
//??,B5,??,1C,??,D1,??,24,??,4D,??,??,??,??,??,00,??,52,??,48,??,BD,??,21,??,??,??,??,??,28,??,D0,??,1C,??,BD,??,1C,??,??,??,??,??,00,??,18,??,BD
extern __thumb unsigned short * getFileExtention(unsigned short * fnane);
//??,B5,??,1C,??,??,??,FE,??,28,??,D0,01,20,??,BD,00,20,??,BD
extern __thumb int DataBrowser_isFileInListExt(unsigned short * ext_table,unsigned short * patch ,unsigned short * fname );

//GUI
//pattern=??,B5,??,1C,??,4D,??,??,??,00,??,1C,??,B4,??,33,??,22,??,21,??,20,??,??,??,F8,??,26,??,27,??,28,??,60,??,B0,??,D0,??,20,??,46,??,70,??,1C,??,??,??,FF,??,28,??,61,??,D0,??,98,??,60,??,98,??,61,??,60,??,48
extern __thumb int CreateBook(void * mem, void * onClose, const PAGE_DESC * bp, const char * name, int __0xFFFFFFFF,int __0);
//??,B5,??,1C,??,4D,??,D1,??,??,??,00,??,1C,??,30,??,??,??,FF,??,1C,??,??,??,F9,??,28,??,D0,??,??,??,00,??,18,??,??,??,FF,??,1C,??,49,??,30,??,??,??,FF,??,21,??,1C,??,??,??,F8,??,68,??,26,??,29,??,D0,??,22,??,1C,??,??,??,FD,??,60,??,E0
extern __thumb void BookObj_KillBook(void *Book);
//??,B5,??,1C,??,1C,??,4E,??,??,??,FA,??,28,??,D0,??,1C,??,30,??,??,??,F9,??,??,??,00,??,1C,??,30,??,??,??,F9,??,1C,??,??,??,FF,??,2D,??,D0,??,1C,??,??,??,FF,??,68,??,36,??,1C,??,??,??,FD,??,E0
extern __thumb void BookObj_GotoPage(BOOK * book, const PAGE_DESC * page);

extern __thumb void BookObj_CallSubroutine(void * , const PAGE_DESC * page);

//??,B5,??,1C,??,1C,??,4E,??,??,??,FA,??,28,??,D0,??,??,??,00,??,18,??,??,??,F8,??,??,??,00,??,1C,??,30,??,??,??,F8,??,AA,??,46,??,1C,??,??,??,F8,??,1C,??,??,??,FE,??,??,??,00,??,18,??,98,??,28,??,D1,??,20,??,E0
extern __thumb void BookObj_ReturnPage(BOOK * ,int);
//??,B5,??,1C,??,1C,??,4E,??,20,??,B4,??,1C,??,22,??,21,??,20,??,??,??,FC,??,1C,??,B0,??,D0,??,1C,??,48,??,B4,??,1C,??,4A,??,49,??,1C,??,??,??,F9,??,28,??,B0,??,D1,??,22,??,1C,??,1C,??,??,??,FD,??,20,??,BD
extern __thumb void * CreateListObject (void * book, int unk);
extern __thumb void SetOnMessages (void * obj, void * onMessage);
//??,B5,??,1C,??,28,??,D0,??,??,??,??,??,1C,{??,B5,??,1C,??,1C,??,20,??,4F,??,00,??,18,??,1C,??,??,??,??,??,1C,??,D1,??,21,??,31,??,??,??,??,??,18,??,??,??,??,??,1C,??,??,??,??,??,28,??,D0,??,??,??,??,??,49,??,18,??,42,??,D0,??,68,??,89,??,42,??,DB,??,21,??,E0,??,21,??,68,??,68,??,??,??,??,??,1C,??,04,??,0C,??,1C,??,??,??,??,??,BD},??,BD
extern __thumb void SetNumOfMenuItem (void * obj, int num);


//pattern=??,B5,??,4C,??,48,??,1C,??,B4,??,33,??,22,??,21,??,20,??,??,??,??,??,1C,??,B0,??,D1,??,20,??,BD,??,98,??,49,??,B4,??,??,??,??,??,18,??,48,??,18,??,49,??,1C,??,??,??,??,??,28,??,B0,??,D1,??,34,??,4A,??,1C,??,1C,??,??,??,??
extern __thumb void * CreateGUI(int unk);
//pattern=??,B5,??,1C,??,??,??,FD,??,1C,{??,B5,??,1C,??,1C,??,28,??,B0,??,D0,??,6E,??,42,??,D0,??,??,??,FE,??,66,??,7F,??,28,??,D0,??,28,??,D1,??,46,??,1C,??,??,??,F9,??,46,??,1C,??,??,??,F9,??,21,??,46,??,72,??,A9,??,1C,??,95,??,??,??,FD,??,B0,??,BD},??,BD
extern __thumb void SetMenuHeaderText (void * gui , int StrID);
//pattern=??,B5,??,1C,??,??,??,F9,??,28,??,D0,??,1C,??,??,??,F9,??,BD,??,1C,??,??,??,F8,??,28,??,D0,??,1C,??,??,??,FF,??,BD,??,1C,??,??,??,FD,??,21,??,1C,??,??,??,F8,??,BD
extern __thumb void ShowWindow (void * gui);
//pattern=
//extern __thumb void  CreatePage (void * gui, const PAGE_DESC * menu_desc);
//pattern=
//extern __thumb void * CreateMenu ( void * page_menu_desc , void * gui);
//??,B5,??,1C,??,D0,??,??,??,00,??,4B,??,B4,??,33,??,22,??,21,??,20,??,??,??,FD,??,1C,??,1C,??,??,??,F8,??,60,??,1C,??,??,??,FD,??,4A,??,1C,??,20,??,??,??,FD,??,B0,??,BD
extern __thumb void FreeBook ( void *book);
//??,B5,??,1C,??,28,??,D0,??,??,??,??,??,1C,{??,B5,??,1C,??,1C,??,21,??,48,??,00,??,18,??,1C,??,??,??,??,??,1C,??,1C,??,??,??,??,??,28,??,D0,??,48,??,42,??,D0,??,1E,??,42,??,D1,??,25,??,E0,??,1C,??,04,??,0C,??,1C,??,??,??,??,??,BD},??,BD
extern __thumb void SetCursorToItem ( void *obj , int item);
//??,B5,??,1C,??,1C,??,??,??,??,??,1C,??,??,??,??,??,06,??,0E,??,42,??,D0,??,1C,??,??,??,??,??,28,??,D0,??,1C,??,??,??,??,??,28,??,D0,??,1C,??,??,??,??,??,??,??,??,??,26,??,06,??,0E,??,28,??,D1,??,26
extern __thumb void SetMenuStyle ( void *obj , int style);
//extern __thumb void DrawString ( int StrID,int algin , int x1,int y1, int x2 , int y2,int unk1,int unk2,int unk3,int unk4);

extern __thumb void * GetListObjectParrent ( void *msg);
//??,B5,??,68,{??,B5,??,78,??,29,??,D1,??,68,??,??,??,??,??,04,??,0C,??,BD,??,48,??,BD},??,04,??,0C,??,BD
extern __thumb int GetCreatedListObjectItemNum ( void *msg);
extern __thumb char GetGUI_msg ( void *msg);
//??,B5,??,68,??,1C,00,21,{??,B5,??,1C,??,1C,??,1C,??,78,??,2A,??,4F,??,D0,??,21,??,31,??,1C,??,??,??,??,??,78,??,2B,??,D1,??,2C,??,D8,??,00,??,44,??,46,??,E0,??,E0,??,E0,??,E0},??,BD
extern __thumb char SetMenuItemText0 ( void *msg , int StrID);
//??,B5,??,68,??,1C,01,21,{??,B5,??,1C,??,1C,??,1C,??,78,??,2A,??,4F,??,D0,??,21,??,31,??,1C,??,??,??,??,??,78,??,2B,??,D1,??,2C,??,D8,??,00,??,44,??,46,??,E0,??,E0,??,E0,??,E0},??,BD
extern __thumb char SetMenuItemText1 ( void *msg , int StrID);
//??,B5,??,68,??,1C,02,21,{??,B5,??,1C,??,1C,??,1C,??,78,??,2A,??,4F,??,D0,??,21,??,31,??,1C,??,??,??,??,??,78,??,2B,??,D1,??,2C,??,D8,??,00,??,44,??,46,??,E0,??,E0,??,E0,??,E0},??,BD
extern __thumb char SetMenuItemText2 ( void *msg , int StrID);
//??,B5,??,68,??,1C,03,21,{??,B5,??,1C,??,1C,??,1C,??,78,??,2A,??,4F,??,D0,??,21,??,31,??,1C,??,??,??,??,??,78,??,2B,??,D1,??,2C,??,D8,??,00,??,44,??,46,??,E0,??,E0,??,E0,??,E0},??,BD
extern __thumb char SetMenuItemText3 ( void *msg , int StrID);
//??,B5,??,1C,??,1C,??,26,??,29,??,D8,??,00,??,44,??,46,??,E0,??,E0,??,E0,??,E0,??,E0,??,E0,??,26,??,E0,??,26,??,E0,??,26,??,E0,??,26,??,E0,??,26,??,E0,??,21,??,48,??,??,??,??,??,68,??,1C,??,06,??,0E,??,??,??,??,??,BD
extern __thumb void SetListObjectItemIcon ( void *msg , int align , int iconID);
//??,B5,??,1C,??,1C,??,28,??,B0,??,D0,??,6E,??,42,??,D0,??,??,??,??,??,66,??,7F,??,28,??,D0,??,28,??,D1,??,46,??,1C,??,??,??,??,??,46,??,1C,??,??,??,??,??,21,??,46,??,72,??,A9,??,1C,??,95,??,??,??,??,??,B0,??,BD
extern __thumb void SetHeaderText( void *obj , int StrID);
extern __thumb int GetFocusetListObjectItem( void *obj);
extern __thumb void SetMenuItemStyle( void *obj , int style);
extern __thumb void * getxxx(void);
extern __thumb void putchar( void *xx , int x, int y, int _zero, int zero1 , short wchar);
extern __thumb void IconToTray(u16 icon , int show);


//pattern=
extern __thumb void ShowMessage(int unk,int StrID,int unk1,int lifetime,int unk2);


//pattern=
extern __thumb void AddMSGHook(void * menu,short msg,void (*proc)(BOOK *,void*));
extern __thumb void AddCommand(void * menu,int cmd , int StrID);
extern __thumb void SoftKey_SetEnable(void *gui,int msg,int ena);
extern __thumb void SoftKey_AddErrorStr(void *gui,int msg,int StrID);
extern __thumb void SoftKey_RemoveItem(void *gui,u16 action);
extern __thumb void SoftKey_SetVisible(void *gui,int action,int visible);
extern __thumb void SoftKey_SuppressDefaultAction(void *gui,int action);

// RadioButton
//pattern=
extern __thumb void * CreateRadioButtons(void * gui);
//pattern=
extern __thumb void SetRadioButtonsCount(void * gui , int count);
//pattern=
extern __thumb void SetRadioButtonsChecked(void * gui , int checked);
//pattern=
extern __thumb void SetRadioButtonsProc(void * gui , void * proc);
//pattern=
extern __thumb void SetRadioButtonsText(void * gui , int * textTable , int rbCount);
//pattern=
extern __thumb void RadioButtonsClose(void * gui);
//pattern=
extern __thumb int GetSelectedRadioButton(void * rb);


//pattern=
//extern __arm void SetLampLevel(int level);


//pattern=
extern __thumb int Str2ID(const void * wstr , int  flag , int len);
extern __thumb int chr2ID(const void * str , int  flag , int len);

//wide strings
//pattern=??,B5,??,1C,??,1C,??,1C,??,4F,??,D1,??,21,??,1C,??,30,??,??,??,??,??,21,??,1C,??,30,??,??,??,??,??,2D,??,D1,??,21,??,37,??,1C,??,??,??,??,??,E0,??,80,??,1C,??,1C,??,88,??,28,??,D1,??,21,??,80,??,1C,??,BD
extern __thumb u16* wstrcpy(u16 * dest, u16 * source);
extern __thumb u16* wstrrchr(const u16* wstr,u16 wchar);
//pattern=??,B5,??,B0,??,1C,??,1C,??,1C,??,28,??,90,??,4F,??,D1,??,21,??,1C,??,30,??,??,??,??,??,21,??,1C,??,30,??,??,??,??,??,2D,??,D1,??,21,??,37,??,1C,??,??,??,??,??,2E,??,D0,??,88,??,28,??,D0,??,80,??,1C,??,1C,??,1E,??,E7,??,22,??,80,??,1C,??,1E,??,2E,??,D1,??,98,??,B0,??,BD
extern __thumb short* wstrncpy(short * dest, short * source, int maxlen);
//pattern=??,B5,??,1C,??,1C,??,1C,??,4F,??,D1,??,21,??,1C,??,30,??,??,??,??,??,21,??,1C,??,30,??,??,??,??,??,2D,??,D1,??,21,??,37,??,1C,??,??,??,??,??,E0,??,1C,??,88,??,29,??,D1,??,88,??,1C,??,80,??,1C,??,28,??,D1,??,1C,??,BD
extern __thumb u16* wstrcat(u16 * wstr, u16 * subwstr);
//pattern=??,B5,??,9E,??,1C,??,1C,??,2E,??,4F,??,D1,??,21,??,1C,??,30,??,??,??,??,??,21,??,1C,??,30,??,??,??,??,??,2C,??,D1,??,21,??,37,??,1C,??,??,??,??,??,88,??,29,??,D0,??,1C,??,E7,??,80,??,1C,??,1C,??,1E,??,2D,??,D0,??,88,??,28,??,D1,??,22,??,80,??,98,??,B0,??,BD
extern __thumb short* wstrncat(short * wstr, short * subwstr , int maxlen);
//pattern=??,B5,??,1C,??,1C,??,28,??,4E,??,D1,??,21,??,1C,??,30,??,??,??,??,??,2D,??,D1,??,21,??,36,??,1C,??,??,??,??,??,E0,??,1C,??,1C,??,88,??,20,??,43,??,29,??,D0,??,88,??,42,??,DB,??,42,??,DA,??,20,??,BD,??,88,??,2B,??,D1,??,20,??,BD
extern __thumb int wstrcmp(unsigned short * wstr1, unsigned short * wstr2);
//pattern=??,B5,??,1C,??,25,??,28,??,D1,??,48,??,21,??,30,??,??,??,??,??,E0,??,1C,??,1C,??,88,??,1C,??,29,??,D1,??,1C,??,BD
extern __thumb int wstrlen(u16 * wstr);
extern __thumb u16 * str2wstr(u16 * wstr,char * str);
extern __thumb int strcmp(char * str1, char * str2);
extern __thumb int strlen(char *str);
extern __thumb char * wstr2str (char * str , u16 * wstr);

extern __thumb void SetGuiItemText(void *gui,int item,int StrID);
extern __thumb void StrID2Str(int StrID,char * str,int maxlen);
extern __thumb void TextID2wstr(int StrID,u16 * str,int maxlen);
extern __thumb int TextGetLength(int StrID);
extern __thumb void TextFree(int StrID);
extern __thumb void sub_44D30D64(void *menu,short msg,int unk);

extern __thumb int Gif2ID(u16 IMAGEHANDLE,u16 * path, u16 * fname,u16 * ID);
extern __thumb int REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE(const int * __zero,u16 * IMAGEHANDLE,u16 * unk);
extern __thumb void REQUEST_DATEANDTIME_GET(const int * __zero , DATETIME * dt);

extern __thumb void IndicationDevice_Backlight_FadeToLevel(int unk_zero ,int bl_level);
extern __thumb void LLRS232_Ena(void);
extern __thumb int GetFreeBytesOnHeap(void);
extern __thumb int int2strID (int num);

extern __thumb void Hide(void * udata);
extern __thumb void Show(void * udata);

extern __thumb void * ListElement_GetByIndex(LIST *lst,int num_element);
extern __thumb void ListElement_Add(LIST *lst,void * element);

extern __thumb int ListElement_Find(LIST *lst,void *element, int (*cmp_proc)(void *,void *));
extern __thumb void * ListElement_Remove(LIST *lst,int index);
extern __thumb LIST *List_New(void);
extern __thumb void List_Free(LIST *lst);
extern __thumb void ListElement_AddtoTop(LIST *lst,void *newElement);
extern __thumb int ListElement_Prt2NumElement(LIST *lst,void *ptr);

extern __thumb void StatusIndication_Item8_SetText(int StrID);

extern __thumb int AB_DEFAULTNBR_GET(int rec_num, void * mem_0x30, int * unk);
extern __thumb int AB_READSTRING(void * mem_0x30,int rec_num, int rec_item_num);
extern __thumb int AB_READPHONENBR(void * mem_0x30,int rec_num, int rec_item_num);
extern __thumb int AB_GETNBROFITEMS(int get_from,int unk_0);
extern __thumb int PNUM_len(void *pnum);
extern __thumb void PNUM2str(char *str,void *pnum,int len, int max_len);

extern __thumb void StartAPP (u16 * appname);
extern __thumb void CreateEvent(int event);
extern __thumb void * FindBook(int (* proc)(BOOK * ));
extern __thumb BOOK * Find_StandbyBook(void);
extern __thumb void BookObj_SetFocus(void *,int focus);
extern __thumb char * manifest_GetParam(char *buf,char *param_name,int unk);
extern __thumb void StatusIndication_SetItemText(void *gui,int item,int StrID);
extern __thumb void StatusIndication_ShowNotes(int StrID);

extern __thumb int isVolumeControllerBook(BOOK * book);
extern __thumb int isRightNowBook(BOOK * book);

extern __thumb void player_create_image_skin(void * );
extern __thumb void player_set_gui_item_color(void* , int item , int color);


extern __thumb int FSX_OpenDir(u16 * dir);
extern __thumb int FC_listingDir(int file_handle, void * buf);
extern __thumb void * FileListCreate (void *);

extern  __thumb int GetCurrentPID(void);


extern __thumb void * CreateStringInput(int,...);


extern __thumb void * AllocDirHandle(u16 * path);
extern __thumb void *GetFname(void *, void*);

extern __arm void *_GetFname(void *, void*);

extern __thumb void debug_printf(char * fmt,...);

extern __thumb void  DestroyDirHandle(void *handle);
#endif




#define fwputs(_WSput_,_FILEput_) fwrite((u16*)_WSput_,wstrlen((u16*)_WSput_),2,_FILEput_)
#define fwnputs(_WSput_,_MAXLENpu_,_FILEput_) fwrite((u16*)_WSput_,_MAXLENpu_,2,_FILEput_)

#ifdef K750_R1CA021
#ifndef SWI_HOOK
#pragma optimize=no_inline
  void *malloc(int size){return memalloc(size,1,5,__BASE_FILE__,0);}
  #pragma optimize=no_inline
  void mfree(void *p){memfree(p,__BASE_FILE__,0);}
#endif

/*
#ifdef SWI_HOOK
extern __arm int ListElement_Prt2NumElement(LIST *lst,void *ptr);

extern __arm void * AllocDirHandle(u16 * path);
extern __arm void *GetFname(void *, void*);
extern __arm void memset(void *mem,char chr,int size);
extern __arm LIST *List_New(void);
extern __arm BOOK * Find_StandbyBook(void);
extern __arm int ListElement_Find(LIST *lst,void *element, int (*cmp_proc)(void *,void *));
extern __arm void * ListElement_GetByIndex(LIST *lst,int num_element);
extern __arm void ListElement_AddtoTop(LIST *lst,void *newElement);
extern __arm void * ListElement_Remove(LIST *lst,int index);
extern __arm u16* wstrrchr(const u16* wstr,u16 wchar);
extern __arm int _fopen(const unsigned short *filpath, const unsigned short *filname,unsigned int mode , unsigned int rights, unsigned int __0);
extern __arm memcpy(void *dest,const void *source,int cnt);
extern __arm void BookObj_CallSubroutine(void * , const PAGE_DESC * page);
extern __arm int wstrlen(u16 * wstr);
extern __arm u16* wstrcpy(u16 * dest, u16 * source);
extern __arm u16* wstrcat(u16 * wstr, u16 * subwstr);
extern __arm void BookObj_ReturnPage(BOOK * ,int);
extern __arm void ListElement_Add(LIST *lst,void * element);
extern __thumb void *memalloc(int size,int f1,int f2,const char *fname,int fline);
extern __thumb void memfree(void * mem,const char * fname ,int fline);
#endif
*/
#endif

#ifdef W810_R4DB005
#ifndef SWI_HOOK
  #pragma optimize=no_inline
  void *malloc(int size){return memalloc(size,1,5);}
  #pragma optimize=no_inline
  void mfree(void *p){memfree(p);}
#endif
#endif

#ifdef W810_R4EA031
#ifndef SWI_HOOK
  #pragma optimize=no_inline
  void *malloc(int size){return memalloc(size,1,5);}
  #pragma optimize=no_inline
  void mfree(void *p){memfree(p);}
#endif
#endif

#ifdef NEED_MACRO_MEM
  #define malloc(_SZ_) memalloc((_SZ_),1,5,__func__,__LINE__)
  #define mfree(_PP_) memfree((_PP_),__func__,__LINE__)
#else
//  void *malloc(int size);
//  void mfree(void *p);
#endif

__arm void * test(void * a, void  * c);



void SoftKey_Add(void *_L0_, int _ACTION_ , void (*_PROC_)(BOOK *,void*), int _STR_, int _HLPSTR_, int _ERRSTR_ ,int _PARENT_);


void Draw_ScreenSaver(int,int,int,int,int,int,int,int,int,int,int,int,int,int);

