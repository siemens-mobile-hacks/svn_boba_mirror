#include "../inc/swilib.h"
#include "main.h"
#include "clist_util.h"
#include "history.h"
#include "message_list.h"
#include "jabber_util.h"
#include "string_util.h"

char MsgList_Quit_Required = 0;

TRESOURCE* Resource_Ex = NULL;


int Message_gui_ID;


#pragma inline
void patch_header(HEADER_DESC* head)
{
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}
#pragma inline
void patch_input(INPUTDIA_DESC* inp)
{
  inp->rc.x=0;
  inp->rc.y=HeaderH()+1+YDISP;
  inp->rc.x2=ScreenW()-1;
  inp->rc.y2=ScreenH()-SoftkeyH()-1;
}


WSHDR* ws_eddata = NULL;
int Terminate=0;
//---------------------------------------------------------------------------
// Test edit dialog
//---------------------------------------------------------------------------
int inp_onkey(GUI *gui, GUI_MSG *msg)
{

  if(msg->gbsmsg->submess==GREEN_BUTTON)
  {
    Terminate = 1;
    return 1;
  }

  if (msg->keys==0x18)
  {
    ShowMSG(1,(int)"/part ��� ������ �� ����� :)");
    return(-1); //do redraw
  }
  return(0); //Do standart keys

}

void inp_redraw(void *data)
{
}


void inp_ghook(GUI *gui, int cmd)
{
  static SOFTKEY_DESC sk={0x0018, 0x0000,(int)"������"};
  EDITCONTROL ec;
  if (cmd==2)
  {
    //Called after onCreate
  }
  if (cmd==7)
  {
#ifdef NEWSGOLD
    SetSoftKey(gui,&sk,0);    
#else
    SetSoftKey(gui,&sk,2);
#endif   
    ExtractEditControl(gui,1,&ec);    
    wstrcpy(ws_eddata,ec.pWS);
  }
  
  if(cmd==0xA)
  {
    DisableIDLETMR();   // ��������� ������ ������ �� ��������
  }
  
  if(Terminate) 
 {
   ExtractEditControl(gui,1,&ec);    
   wstrcpy(ws_eddata,ec.pWS);
   size_t xz = wstrlen(ws_eddata)*2;
   if(xz)
   {
    char* body =  utf16_to_utf8((char**)ws_eddata,&xz);
    body[xz]='\0';
   char is_gchat = Resource_Ex->entry_type== T_CONF_ROOT ? 1: 0;
   char part_str[]="/part";
   
   if(!is_gchat)
   {
     char* hist = convUTF8_to_ANSI_STR(body);
     CList_AddMessage(Resource_Ex->full_name, MSG_ME, hist);
     mfree(hist);
   }
   else
   if(strstr(body, part_str)==body)  // ���� � ������
   {
     CLIST* room=CList_FindContactByJID(CList_GetActiveContact()->full_name);
     Leave_Conference(room->JID);
     CList_MakeAllResourcesOFFLINE(room);
     Terminate = 0;
     mfree(body);    
     return;
   }
   IPC_MESSAGE_S *mess = malloc(sizeof(IPC_MESSAGE_S));
   mess->IsGroupChat = is_gchat;
   mess->body = body;
   SUBPROC((void*)SendMessage,Resource_Ex->full_name, mess);  
   }
   else ShowDialog_Error(1,(int)"������ ������� ������ ���������");
   Terminate = 0;
 }
 
 
}

void inp_locret(void){}

SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"�����"},
  {0x0001,0x0000,(int)"�����"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};

INPUTDIA_DESC inp_desc=
{
  1,
  inp_onkey,
  inp_ghook,
  (void *)inp_locret,
  0,
  &menu_skt,
  {0,22,131,153},
  4,
  100,
  101,
  0,
  0,
  0x40000000
};

HEADER_DESC inp_hdr={0,0,131,21,NULL,(int)"�����...",0x7FFFFFFF};



// ������� ���� ��������� ������ ���������
void Init_Message(TRESOURCE* ContEx)
{
  Resource_Ex = ContEx;
  patch_header(&inp_hdr);
  patch_input(&inp_desc);
  ws_eddata = AllocWS(MAX_MSG_LEN);
  EDITCONTROL ec;
  void *ma=malloc_adr();
  void *eq;
  PrepareEditControl(&ec);
  eq=AllocEQueue(ma,mfree_adr());
  ConstructEditControl(&ec,3,0x40,ws_eddata,MAX_MSG_LEN);
  AddEditControlToEditQend(eq,&ec,ma);
  CreateInputTextDialog(&inp_desc,&inp_hdr,eq,1,0);  
}

// ������ ������
DISP_MESSAGE* MessagesList = NULL;
unsigned int MessList_Count = 0;
unsigned int OLD_MessList_Count = 0;


// ����� ������ ���������
void KillDisp(DISP_MESSAGE* messtop)
{
  DISP_MESSAGE* cl=messtop;
  LockSched();
  messtop = NULL;
  while(cl)
  {
    DISP_MESSAGE *p;
    if(cl->mess)FreeWS(cl->mess);
    p=cl;
    cl=cl->next;
    mfree(p);
  }
  MessList_Count = 0;
  OLD_MessList_Count = 0;
  UnlockSched();  
}

//===============================================================================================

char CurrentPage=1;
char lines_on_page;
char MaxPages=1;
unsigned short FontSize;
// ������������ ���������� GUI

// ������ ����������� ������� :)
void Calc_Pages_Data()
{
  MaxPages = sdiv(lines_on_page,MessList_Count);
  if(lines_on_page*MaxPages<MessList_Count)MaxPages++;
}

void mGUI_onRedraw(GUI *data)
{
  
  if(Resource_Ex->total_msg_count!=OLD_MessList_Count)
  {
    //KillDisp(MessagesList);
    ParseMessagesIntoList(Resource_Ex);
    OLD_MessList_Count = Resource_Ex->total_msg_count;
  }
  
  Terminate = 0;
  // ������ ���������� ����� �� ����� ��������
  
  Calc_Pages_Data();
  // ��������� ����
  DrawRoundedFrame(0,0,ScreenW()-1,FontSize*2+1,0,0,0,
		   GetPaletteAdrByColorIndex(0),
		   GetPaletteAdrByColorIndex(MESSAGEWIN_TITLE_BGCOLOR));
  
  DrawRoundedFrame(0,FontSize+2,ScreenW()-1,ScreenH()-1,0,0,0,
		   GetPaletteAdrByColorIndex(0),
		   GetPaletteAdrByColorIndex(MESSAGEWIN_BGCOLOR));

  // ������ ���� �������� ������... :)
  WSHDR* ws_title = AllocWS(256);
  //str_2ws(ws_title, Resource_Ex->full_name,strlen(Resource_Ex->full_name));
  ascii2ws(ws_title, Resource_Ex->full_name);

  DrawString(ws_title,1,1,ScreenW()-1,FontSize+1,SMALL_FONT,0,GetPaletteAdrByColorIndex(MESSAGEWIN_TITLE_FONT),GetPaletteAdrByColorIndex(23));  
  
  DISP_MESSAGE* ml = MessagesList;
  int i_ctrl=0;
  int i = 0;
  char MsgBgClolor;
  while(ml)
  {
    if((i_ctrl>=(CurrentPage-1)*lines_on_page) && (i_ctrl<CurrentPage*lines_on_page))
    {
      //str_2ws(ws_title,ml->mess,strlen(ml->mess));
      
      //str_2ws(ws_title,ml->mess,CHAR_ON_LINE);
      switch(ml->mtype)
      {
      case MSG_ME:{MsgBgClolor=MESSAGEWIN_MY_BGCOLOR;break;}        
      case MSG_CHAT:{MsgBgClolor=MESSAGEWIN_CH_BGCOLOR;break;}                                        
      case MSG_SYSTEM:{MsgBgClolor=MESSAGEWIN_SYS_BGCOLOR;break;}                    
      case MSG_STATUS:{MsgBgClolor=MESSAGEWIN_STATUS_BGCOLOR;break;}                             
      case MSG_GCHAT:
        {
          MsgBgClolor=ml->log_mess_number %2==0? MESSAGEWIN_GHAT_BGCOLOR_1 : MESSAGEWIN_GHAT_BGCOLOR_2;
          break;
        }
      }
      DrawRoundedFrame(0,HIST_DISP_OFS+i*FontSize,ScreenW()-1,HIST_DISP_OFS+(i+1)*FontSize,0,0,0,
		   GetPaletteAdrByColorIndex(MsgBgClolor),
		   GetPaletteAdrByColorIndex(MsgBgClolor));
    
      //DrawString(ws_title,1,20+i*FontSize,ScreenW()-1,20+(i+1)*FontSize,SMALL_FONT,0,GetPaletteAdrByColorIndex(MESSAGEWIN_TITLE_FONT),GetPaletteAdrByColorIndex(23));      
      DrawString(ml->mess,1,HIST_DISP_OFS+i*FontSize,ScreenW()-1,HIST_DISP_OFS+(i+1)*FontSize,SMALL_FONT,0,GetPaletteAdrByColorIndex(MESSAGEWIN_TITLE_FONT),GetPaletteAdrByColorIndex(23));      
      i++;
    }
    ml = ml->next;
    i_ctrl++;
  }  
  FreeWS(ws_title);
  Resource_Ex->has_unread_msg =0; // ������������� ��������� ������ ���
}

void mGUI_onCreate(GUI *data, void *(*malloc_adr)(int))
{
  data->state=1;
  MsgList_Quit_Required=0;
}

void mGUI_onClose(GUI *data, void (*mfree_adr)(void *))
{
  KillDisp(MessagesList);
  if(ws_eddata)
  {
    FreeWS(ws_eddata);
    ws_eddata = NULL;
  }  
  data->state=0;
}

void mGUI_onFocus(GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  DisableIDLETMR();   // ��������� ������ ������ �� ��������
  data->state=2;
}

void mGUI_onUnfocus(GUI *data, void (*mfree_adr)(void *))
{
  if (data->state!=2) return;
  data->state=1;
}

void DbgInfo()
{
  char q[200];
  sprintf(q,"MCnt=%d; CP=%d, MP=%d",MessList_Count,CurrentPage, MaxPages);
  ShowMSG(1,(int)q);
}

int mGUI_onKey(GUI *data, GUI_MSG *msg)
{
  if(MsgList_Quit_Required)return 1; //���������� ����� GeneralFunc ��� ���. GUI -> �������� GUI

  //DirectRedrawGUI();
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
      case '0':
      {
        // ����� ������ ���������
        //KillDisp(MessagesList);
        KillMsgList(Resource_Ex->log);
        Resource_Ex->log = NULL;
        Resource_Ex->has_unread_msg=0;
        Resource_Ex->total_msg_count=0;
        return 1;
      }
      
      case RIGHT_SOFT:
      {
        return 1;
      }
      
    case LEFT_SOFT:
      {
        //DbgInfo();
        break;
      }
      
      case UP_BUTTON:
        {
          if(CurrentPage>1)CurrentPage--;
          REDRAW();
          break;         
        }
    case DOWN_BUTTON:
      {
          if(CurrentPage<MaxPages)CurrentPage++;
          REDRAW();
          break;         
      }
    case GREEN_BUTTON:
      {
        Init_Message(Resource_Ex);
        break;
      }
    }    
  }
  return(0);
}


extern void kill_data(void *p, void (*func_p)(void *));

// ���
int mGUI_method8(void){return(0);}

int mGUI_method9(void){return(0);}

const void * const mgui_methods[11]={
  (void *)mGUI_onRedraw,	//Redraw
  (void *)mGUI_onCreate,	//Create
  (void *)mGUI_onClose,	//Close
  (void *)mGUI_onFocus,	//Focus
  (void *)mGUI_onUnfocus,	//Unfocus
  (void *)mGUI_onKey,	//OnKey
  0,
  (void *)kill_data, //onDestroy,	//Destroy
  (void *)mGUI_method8,
  (void *)mGUI_method9,
  0
};
////////////////////////////////////////////////////////////////////////////////
#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}


const RECT mCanvas={0,0,0,0};

/////////////////////////////////////////// ������ ����� ��� ������ ������������

// ���������, �����������, ��� �� ��� ������
typedef struct
{
  char mask[7];
  char replace;
}REPL_ARRAY;

// ���� ������ � �� ����������
const int Repl_chars_count = 5;
REPL_ARRAY Repl_chars[] = {"&apos;\0",0x27,
                           "&quot;\0",'"',
                           "&lt;\0\0\0", '<',
                           "&gt;\0\0\0", '>',
                           "&amp;\0\0", '&',                   
};

/*
    �������� ���������� �� ��� �����
IN: mask_begin - ������ ��������
    out_ofs - �����, � �������� ���������� ����� ������������ ������������������
OUT: out_ofs - �������� � ������, ������ ���������� �������������� ������
    <return> - ����������
*/
char GetSpecialSym(char *mask_begin, int *out_ofs)
{
  int i=0;
  int replen;
  char rep_ex[10];
  if(*mask_begin!='&')return *(mask_begin);
  for(i=0;i<Repl_chars_count;i++)
  {
    replen = strlen(Repl_chars[i].mask);  // ���������� ����� ��������� �����
    zeromem(rep_ex,10);
    strncpy(rep_ex,mask_begin,replen);    // �������� ������ ����� ����� � ������� �������
    if(!strcmp(rep_ex,Repl_chars[i].mask))// ���� ������� � ��������� ������
    {
      *out_ofs+=replen-1;                   // ����������� ������������ ����� �� ����� �����
      return Repl_chars[i].replace;       // ���������� ������ ��� ������
    }
  }
  return *(mask_begin);       //  ����� �� �������, ���������� ��� ����
}

// �������� ��������� ��������� DISP_MESSAGE ��� ����������� �� ������
void ParseMessagesIntoList(TRESOURCE* ContEx)
{
  if(!ContEx)return;
  int parsed_counter = 0; // ������� ��� ���� ���������� (=OLD_MessList_Count)
  LOG_MESSAGE* MessEx= ContEx->log;
  int cnt=0;
  char IsCaret = 0; // �������� �� ������ ��������� ������
//  int chars;
  DISP_MESSAGE* Disp_Mess_Ex, *tmp;  
  if(!MessEx)return;
  LockSched();

  char* msg_buf = malloc(CHAR_ON_LINE+2);
  zeromem(msg_buf,CHAR_ON_LINE+2);
  // ���� �� ���� ����������
  while(MessEx)
  {
    
    if(parsed_counter>=OLD_MessList_Count)
    {
    int l=strlen(MessEx->mess);
    
    int i=0;
    char symb;
    cnt=0;
    for(i=0;i<=l;i++)
    {
      symb = GetSpecialSym(MessEx->mess+i,&i);
      //symb = *(MessEx->mess+i);
      IsCaret = symb==0x0A || symb==0x0D || symb==0xA0 ? 1 : 0;
      if(!IsCaret && symb!=0x0 && cnt<CHAR_ON_LINE)
      {
        *(msg_buf + cnt) = symb;
        cnt++;
      }
      if(IsCaret || cnt>=CHAR_ON_LINE || symb==0x0) // ������� ������
      {
        Disp_Mess_Ex = malloc(sizeof(DISP_MESSAGE));
        Disp_Mess_Ex->mess = AllocWS(cnt);
        ascii2ws(Disp_Mess_Ex->mess, msg_buf);
        zeromem(msg_buf,CHAR_ON_LINE+1);
        Disp_Mess_Ex->mtype = MessEx->mtype;
        Disp_Mess_Ex->log_mess_number=parsed_counter;
        if(!MessagesList){MessagesList =Disp_Mess_Ex;Disp_Mess_Ex->next=NULL;}
        else
        {
          tmp= MessagesList;
          while(tmp->next)
          {
            tmp = tmp->next;
          }
          tmp->next = Disp_Mess_Ex;
          Disp_Mess_Ex->next=NULL;
        }
        cnt=0;
        MessList_Count++;
      }
    }  
    }
    MessEx = MessEx->next;
    parsed_counter++;
  }
  mfree(msg_buf);
  UnlockSched();
}


// ���������� ������ ���������
void Display_Message_List(TRESOURCE* ContEx)
{
  if(!ContEx)return;
// �������������
  OLD_MessList_Count = 0;
  MessagesList = NULL;
  MessList_Count = 0;
  Resource_Ex = ContEx;
  FontSize = GetFontYSIZE(SMALL_FONT);
  lines_on_page = sdiv(FontSize, ScreenH() - HIST_DISP_OFS);
  ParseMessagesIntoList(Resource_Ex);
  Calc_Pages_Data();
  CurrentPage = MaxPages;
  OLD_MessList_Count = Resource_Ex->total_msg_count;

// ������� ���
  GUI *mess_gui=malloc(sizeof(GUI));
  zeromem(mess_gui, sizeof(GUI));
  patch_rect((RECT*)&mCanvas,0,0,ScreenW()-1,ScreenH()-1);
  mess_gui->canvas=(void *)(&mCanvas);
  mess_gui->flag30=2;
  mess_gui->methods=(void *)mgui_methods;
  mess_gui->item_ll.data_mfree=(void (*)(void *))mfree_adr();
  Message_gui_ID = CreateGUI(mess_gui);
}
