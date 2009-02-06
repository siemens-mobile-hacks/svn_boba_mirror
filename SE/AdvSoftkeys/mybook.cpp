#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\main.h"
#include "header\config_data.h"
#include "header\conf_loader.h"
#include "..\\include\cfg_items.h"
#include "header\iofunc.h"

//--------------------------------------------------------------
#define VERSION_ 0x00AAAA01

#define AuthorInfo L"AdvSoftkeys v1.1\n� UltraShot\n"
//--------------------------------------------------------------
wchar_t myelfpath[512];
MyBOOK *MyBook=0;
bool works=true;
u16 timer2;
int count=0;
//--------------------------------------------------------------
extern DISP_OBJ_ONREDRAW_METHOD DREDRAW;
extern u16 timer;
extern DISP_OBJ_METHOD DREFRESH;
extern UI_SOFTKEY sk[3];
extern RECT rect;
extern DISP_OBJ *soft;
//--------------------------------------------------------------
void DispDraw(DISP_OBJ* DO,int a,int b,int c);
void Init();
void Send_REDRAW_RELEASE();
void csofts_destroy();
void destroy_all_images();
void Init();
int NewKey(int key, int r1 , int mode);
int onkey(int key);
int ourredraw(DISP_OBJ *DO, int a, int b, int c);
int isSoftkeysBook(BOOK *bk);
//--------------------------------------------------------------
int MyBook_OnTerminate(void * ,BOOK* book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
};

int MyBook_OnShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(0x6FFFFFFF,Str2ID(AuthorInfo,0,SID_ANY_LEN), 0, 1 ,5000,msg->book);
  return(1);
};

//--------------------------------------------------------------
void Send_REDRAW_RELEASE()
{
  // �������� ����  ������� �� ������ � ����� �������� Redraw
  REDRAW_RELEASE_MESSAGE * sbm= new REDRAW_RELEASE_MESSAGE;
  memset(sbm,0,sizeof(REDRAW_RELEASE_MESSAGE));
  sbm->SK_OldOnRedraw=DREDRAW;
  sbm->SK_NewOnRedraw=DispDraw;
  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,sbm,(void (*)(void*))mfree_adr());
};

int onElfKilled(void *mess ,BOOK* book)
{
  // ���� ��� ���� ���� �������� �� �� ��� ������ ����� ������������ ������
  REDRAW_RELEASE_MESSAGE * sbm=(REDRAW_RELEASE_MESSAGE*)mess;
  // ��� �� ����� �� ���������� � �������� oldRedraw?
  if (sbm->SK_NewOnRedraw==DREDRAW)
  {
    REDRAW_RELEASE_MESSAGE * ms= new REDRAW_RELEASE_MESSAGE;
    memcpy(ms,sbm,sizeof(REDRAW_RELEASE_MESSAGE));
    //memset(ms,0,sizeof(REDRAW_RELEASE_MESSAGE));
    // ���� �� ��� ����, �� �������� ���� oldRedraw �� ���..
    if (sbm->SK_OldOnRedraw) DREDRAW=sbm->SK_OldOnRedraw;

    // ������ ���� ����� ������
    DISP_DESC_SetOnRedraw(DISP_OBJ_GetDESC(DispObject_SoftKeys_Get()),DispDraw);

    // � ��� ������� �����, ���� ��������� ����� ������� ���� �����
    ms->SK_OldOnRedraw=0;
    ms->SK_NewOnRedraw=DispDraw;
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return 1;
  }
  return 0;
};

void MyBook_OnClose(BOOK * book)
{
  if (book)
  {
    destroy_all_images();
    csofts_destroy();
    ModifyKeyHook(NewKey,0);
    DISP_OBJ *soft=DispObject_SoftKeys_Get();
    DISP_DESC_SetOnRedraw(DISP_OBJ_GetDESC(soft), DREDRAW);
    DISP_DESC_SetonRefresh(DISP_OBJ_GetDESC(soft), DREFRESH);
    SUBPROC(elf_exit);
  }
};
//--------------------------------------------------------------
void onTimer2(u16 timerID, LPARAM lparam)
{
  wchar_t *extConfig=GetDir(MEM_EXTERNAL+DIR_ELFS_CONFIG);
  wchar_t *folder=new wchar_t[wstrlen(extConfig)+1+wstrlen(L"AdvSoftkeys/")+1];
  wstrcpy(folder,extConfig);
  wstrcat(folder,L"/");
  wstrcat(folder,L"AdvSoftkeys/");
  if (cmpfile(L"/tpa/system/settings/",L"current_theme.thm", folder, L"curthm.thm") && count<10)
  {
    count++;
    delete(folder);
    Timer_ReSet(&timer2,100, onTimer2,0);
    return;
  }
  else
  {
    delete(folder);
    destroy_all_images();
    Init();
    Timer_Kill(&timer2);
  }
};

int onThemeChange(void *mess,BOOK *bk)
{
  count=0;
  timer2=Timer_Set(100, onTimer2, 0);
  return 0;
};
//--------------------------------------------------------------
const PAGE_MSG HW_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , MyBook_OnTerminate,
  ELF_SHOW_INFO_EVENT  , MyBook_OnShowAuthorInfo,
  THEME_CHANGED_EVENT_TAG, onThemeChange,
  SBY_REDRAW_RELEASE_EVENT, onElfKilled,
  NIL_EVENT_TAG,            NULL
};

PAGE_DESC base_page ={"SoftkeysPage",0,HW_PageEvents};
//--------------------------------------------------------------
wchar_t* update()
{
  MyBOOK *mbk=(MyBOOK*)FindBook(isSoftkeysBook);
  MyBook_OnTerminate(0,&mbk->bk);
  return myelfpath;
};

void Change(UI_SOFTKEY *s,int x)
{
  memcpy(&sk[x], s, sizeof(UI_SOFTKEY));
  InvalidateRect(soft,0);
};

void SetWorkState(bool state)
{
  works=state;
};
//--------------------------------------------------------------
MyBOOK * MyBook_Create()
{
  MyBook= new MyBOOK;
  memset(MyBook, 0, sizeof(MyBOOK));
  if (CreateBook(&MyBook->bk,MyBook_OnClose,&base_page,"Softkeys",-1,0))
  {
    MyBook->redraw=ourredraw;
    MyBook->onkey=onkey;
    MyBook->change=Change;
    MyBook->upd=update;
    MyBook->SetWorkState=SetWorkState;
    MyBook->version=VERSION_;
    textidname2id(L"GUI_MORE_SK",-1,&MyBook->funcstrid);
    textidname2id(L"GUI_OK_SK",-1,&MyBook->okstrid);
    textidname2id(L"SM_KEYX_BACK_TXT",-1,&MyBook->backstrid);
    return(MyBook);
  }
  return 0;
};
//--------------------------------------------------------------

