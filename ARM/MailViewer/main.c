#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "ctype.h"


extern const unsigned int ICON0;
extern const unsigned int ICON1;
extern const unsigned int ICON2;
extern const unsigned int ICON3;
extern const char HIST_PATH[];

int S_ICONS[5];

int displace_config;
typedef struct
{
  int type;
  char uid[44];
}MAIL_HIST;

#define FULL_MES 0   // normal mes
#define UNFULL_MES 1   // unfull mes
#define MES_DOWN 2   // mes for download
#define MES_DEL 3   // mes for delete

int if_any_change=0;
MAIL_HIST* buf; // buffer for maildb
           //writted on create menu and destroy on it close
unsigned int size; // size of this buffer
// ------------------------------- Defines ----------------------------------------
int ecq(const char *a, const char *b)
{
  register const unsigned char
    *us1 = (const unsigned char *)a,
    *us2 = (const unsigned char *)b;
  
  /* Can't test equality of NOTHING */
  if(us1 == NULL || us2 == NULL)
    return 0;
  while (tolower(*us1) == tolower(*us2++))
    if (*us1++ == '\0')
      return 1;
  return 0;
}

int ecnq(const char *a, const char *b,int n)
{
  register const unsigned char
    *us1 = (const unsigned char *)a,
    *us2 = (const unsigned char *)b;
  
  /* Can't test equality of NOTHING */
  if(n == NULL)
    return 0;  
  
  if(us1 == NULL || us2 == NULL)
    return 0;
  while (tolower(*us1++) == tolower(*us2++))
    if (--n==0)
      return 1;
  return 0;
}

unsigned int win2unicode(int letter)
{
  if (letter<168) goto L_ENG;
  if (letter==168) goto L_BIG_YO;
  letter-=168;
  if (letter<24) goto L_UKR;
  if (letter>87) goto L_ENG;
//L_RUS:
  letter-=8;
  goto L_ADD_400;
L_UKR:
  switch(letter)
  {
  case 0x02:
    letter=0x04;
    break;
  case 10:
    letter=6;
    break;
  case 11:
    letter=0x56;
    break;
  case 16:
    letter=0x51;
    break;
  case 18:
    letter=0x54;
    break;
  case 23:
    letter=0x57;
    break;
  default:
    goto L_ENG;
  }
  goto L_ADD_400;
L_BIG_YO:
  letter=0x01;
L_ADD_400:
  return (letter+0x400);
  
L_ENG: return (letter);
}


void ascii2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    wsAppendChar(ws,win2unicode(c));
  }
}


// --------------------------------------------------------------------------------
extern char * base64_decode(const char *str, size_t *size);
extern char * quoted_printable_decode(const char *str, size_t *size);

// --------------------------------------------------------------------------------
const char mailer_db_name[]="mails.db";
const char _percent_t[]="%t";
const int minus11=-11;
unsigned short maincsm_name_body[140];
extern void kill_data(void *p, void (*func_p)(void *));
char* read_db_file(void);
void remake_mailmenu();
int cur_menu;
// --------------------------------------------------------------------------------
int main_menu_id;
int maillist_menu_id;
int options_menu_id;
int view_mail_id;
volatile int request_remake_mailmenu;
volatile int request_close_mailmenu;

// -------------------------- Global Variables -------------------------------------
typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;



void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}
//-----------------------------------------------------------------------------------

int menusoftkeys[]={0,1,2};
SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Options"},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)"+"}
};

SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};

void get_eml_name(char* dest,int i)
{
  MAIL_HIST* fbuf=buf;
  sprintf(dest,"%s%s.eml",HIST_PATH,fbuf[i].uid);
}

// ----------------------------------------------------------------------------------

char*decodestr(char* str)
{
  char * buf=malloc(1024);
  zeromem(buf,1024);
  int i=0;
  int n=0;
  char* first;
  char* decode_start;
  char* decode_end;
  int cod; // 1 - win 1251 2 - koi8
  int type; // 1- base 64 2 - quote printable
  do
  {
    if (str[i]=='=')
    {
      if (str[i+1]=='?')
      {
        i+=2;
        first=str+i;
        if (ecnq(first,"windows-1251",sizeof("windows-1251")-1))
        {
          cod=1;
          i+=sizeof("windows-1251")-1;
          goto L_NEXT_CHECK;
        }
        
        if (ecnq(first,"koi8-r",sizeof("koi8-r")-1))
        {
          cod=2;
          i+=sizeof("koi8-r")-1;
        }
        else goto L_NEXT;
       
      L_NEXT_CHECK:
        
        if (str[i]=='?')
        {
          if (tolower(str[i+1])=='b')
          {
            type=1;
            goto L_FIND_END;
          }
          if (tolower(str[i+1])=='q')
          {
            type=2;
          }
          else goto L_NEXT;
        L_FIND_END:
          i+=3;
          decode_start=str+i;
          
          decode_end=strstr(decode_start,"?=");
          int decode_len=decode_end-decode_start;
          char*decodstr=malloc(decode_len+1);
          char* decoded_str;
          
          decodstr[decode_len]=0;
          strncpy(decodstr,decode_start,decode_len);
          switch(type)
          {
          case 1:
            decoded_str=base64_decode(decodstr,0);
            break;
            
          case 2:
            decoded_str=quoted_printable_decode(decodstr,0);
            break;
          }
          strcat(buf,decoded_str);
          i=decode_end-str+2;
          mfree(decoded_str);
          mfree(decodstr);
        }
        else goto L_NEXT;
        
      }
      else goto L_NEXT;
    }
    else
    {
    L_NEXT:
      buf[n++]=str[i++];
    }
  }
  while(str[i]);
  return buf;
}

int get_file_size(const char * fname)
{
  FSTATS fs;
  unsigned int err;
  int n;
  if ((n=GetFileStats(fname,&fs,&err))!=-1) return (fs.size);
  else return (n);
}

unsigned int open_read_close_file(const char* fname,char **write_to)
{
  int size;
  int f;
  unsigned int err;
  char* buf;
  if ((size=get_file_size(fname))!=-1)
  {
    if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
    {
      buf=malloc(size+1);
      buf[size]=0;
      *write_to=buf;
      fread(f,buf,size,&err);
      fclose(f,&err);
    }
    else 
    {
      write_to=NULL;
      size=-1;
    }
  }
  else 
  {
    write_to=NULL;
    size=-1;
  }
  return size;
}


char* find_str(int i,char * str)
{
  char* eml_buf;
  char* first_in_subj;
  char* sec_in_subj;
  char* decoded_str;
  int subj_len;
  char eml_fname[128];
  char* subj;
  get_eml_name(eml_fname,i);
  if ((open_read_close_file(eml_fname,&eml_buf))!=-0xFFFFFFFF)
  {
    first_in_subj=strstr(eml_buf,str);
    sec_in_subj=strstr(first_in_subj,"\r\n");
    subj_len=sec_in_subj-first_in_subj;
    subj=malloc(subj_len+1);
    subj[subj_len]=0;
    strncpy(subj,first_in_subj,subj_len);
    decoded_str=decodestr(subj);
    mfree(subj);
    mfree(eml_buf);
    return (decoded_str);
  }
  return 0;
}


char* find_full_str(int i,char * str)
{
  char* eml_buf;
  char* first_in_subj;
  char* sec_in_subj;
  int subj_len;
  char eml_fname[128];
  char* subj;
  int size;
  get_eml_name(eml_fname,i);
  if ((size=open_read_close_file(eml_fname,&eml_buf))!=-0xFFFFFFFF)
  {
    if (!(first_in_subj=strstr(eml_buf,str)))  return 0;
    sec_in_subj=eml_buf+size;
    subj_len=sec_in_subj-first_in_subj;
    if (!(subj=malloc(subj_len+1))) return 0;
    subj[subj_len]=0;
    strncpy(subj,first_in_subj,subj_len);
    mfree(eml_buf);
    return (subj);
  }
  return 0;
}
//----------------------------------------------------------------------------------------------
char subj[128];

HEADER_DESC ed1_hdr={0,0,0,0,NULL,(int)subj,LGP_NULL};

void ed1_locret(void){}

int ed1_onkey(GUI *data, GUI_MSG *msg)
{
  return(0); 
}
void ed1_ghook(GUI *data, int cmd)
{
}
INPUTDIA_DESC ed1_desc=
{
  1,
  ed1_onkey,
  ed1_ghook,
  (void *)ed1_locret,
  0,
  &menu_skt,
  {0,0,0,0},
  4,
  100,
  101,
  0,
  0,
  0
};


int create_view(int item)
{
  void *ma=malloc_adr();
  void *eq;
  WSHDR* ews=AllocWS(1024);
  char* str;
  EDITCONTROL ec;

  PrepareEditControl(&ec);
  eq=AllocEQueue(ma,mfree_adr());
  
  ascii2ws(ews,"From:");
  ConstructEditControl(&ec,1,0x40,ews,1024);
  AddEditControlToEditQend(eq,&ec,ma);
  
  str=find_str(item,"From: ");
  ascii2ws(ews,str+6);
  mfree(str);
  ConstructEditControl(&ec,3,0x40,ews,1024);
  AddEditControlToEditQend(eq,&ec,ma);
  
  ascii2ws(ews,"Subject:");
  ConstructEditControl(&ec,1,0x40,ews,1024);
  AddEditControlToEditQend(eq,&ec,ma); 
    
  
  str=find_str(item,"Subject: ");
  int len=strlen(str);
  strncpy(subj,str,128);
  subj[len]=0;
  ascii2ws(ews,str+9);
  mfree(str);
  ConstructEditControl(&ec,3,0x40,ews,1024);
  AddEditControlToEditQend(eq,&ec,ma);
  FreeWS(ews);
  
  ascii2ws(ews,"Body:");
  ConstructEditControl(&ec,1,0x40,ews,1024);
  AddEditControlToEditQend(eq,&ec,ma);   
  
  str=find_full_str(item,"\r\n\r\n");
  ascii2ws(ews,str+4);
  mfree(str);
  ConstructEditControl(&ec,3,0x40,ews,1024);
  AddEditControlToEditQend(eq,&ec,ma);
  
  int width=ScreenW();
  int heigth=ScreenH();
  int head=HeaderH();
  ed1_desc.rc.y=head+1;
  ed1_desc.rc.x2=width-1;
  ed1_desc.rc.y2=heigth-SoftkeyH();
  ed1_hdr.rc.x2=width-1;
  ed1_hdr.rc.y2=head;
  return CreateInputTextDialog(&ed1_desc,&ed1_hdr,eq,1,0);
}  
  
//----------------------------------------------------------------------------------------------
      




void write_db_file()
{
  char db_fname[128];
  snprintf(db_fname,sizeof(db_fname),"%s%s",HIST_PATH,mailer_db_name);
  int f;
  unsigned int err;
  if ((f=(fopen(db_fname,A_ReadWrite+A_Truncate+A_BIN,P_READ+P_WRITE,&err)))==-1) return;
  {
    fwrite(f,buf,size,&err);
    fclose(f,&err);
    return;
  }
}

int GetIconIndex(i)
{
  MAIL_HIST* fbuf=buf;
  int l=fbuf[i].type;
  
  switch (l)
  {
  case 0:
    return 0;
  case 1:
    return 1;
  case 2:
    return 2;
  case 3:
    return 3;
  }
  return 0;
}

// ---------------------------------------------------------------------------------

void setstate(int item,int state)
{
  if_any_change=1;
  MAIL_HIST* fbuf=buf;
  fbuf[item].type=state;
}


int options_menu_onkey(void)
{
  return 0;
}

void options_menu_ghook(void)
{
}

void set_state_for_all(int state)
{
  int n=size/sizeof(MAIL_HIST);
  for (int i=0;i!=n;i++)
  {
    setstate(i,state);
  }
}

void set_state_download()
{
  setstate(cur_menu,MES_DOWN);
  GeneralFuncF1(1);
}

void set_state_delete()
{
  setstate(cur_menu,MES_DEL);
  GeneralFuncF1(1);
}

void set_state_download_all()
{
  set_state_for_all(MES_DOWN);
  GeneralFuncF1(1);
}

void set_state_delete_all()
{
  set_state_for_all(MES_DEL);
  GeneralFuncF1(1);
}
void delete_record()
{
  
  MAIL_HIST* fbuf=buf;
  int num_rec;
  MAIL_HIST* endbuf;
  
  num_rec=size/sizeof(MAIL_HIST);
  
  endbuf=fbuf+num_rec;
  memcpy(&fbuf[cur_menu],&fbuf[cur_menu+1],(endbuf-&fbuf[cur_menu+1])*sizeof(MAIL_HIST));
  size-=sizeof(MAIL_HIST);
  if_any_change=1;
  GeneralFuncF1(1);
  remake_mailmenu();
}

void back()
{
  GeneralFuncF1(1);
}



HEADER_DESC options_menuhdr={0,0,0,0,NULL,(int)"�����",LGP_NULL};

MENUITEM_DESC options_menu_ITEMS[6]=
{
  {NULL,(int)"���������",     LGP_NULL, 0, NULL, 3, 0x578},
  {NULL,(int)"�������",       LGP_NULL, 0, NULL, 3, 0x578},
  {NULL,(int)"������� ���",   LGP_NULL, 0, NULL, 3, 0x578},
  {NULL,(int)"������� ������",LGP_NULL, 0, NULL, 3, 0x578},
  {NULL,(int)"��������� ���", LGP_NULL, 0, NULL, 3, 0x578},  
  {NULL,(int)"�����",         LGP_NULL, 0, NULL, 3, 0x578},
};

void *options_menu_HNDLS[6]=
{
  (void *)set_state_download,
  (void *)set_state_delete,
  (void *)set_state_delete_all,
  (void *)delete_record,
  (void *)set_state_download_all,
  (void *)back,  
};

MENU_DESC options_menu_STRUCT=
{
  8,(void *)options_menu_onkey,(void*)options_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt,
  0x11,
  NULL,
  options_menu_ITEMS,   //Items
  options_menu_HNDLS,   //Procs
  6 //n
};

void create_options_menu(int i)
{
  cur_menu=i;
  options_menuhdr.rc.x=3;
  options_menuhdr.rc.y=0x18;
  options_menuhdr.rc.x2=ScreenW()-6;
  options_menuhdr.rc.y2=0x18+HeaderH();
  
  options_menu_id=CreateMenu(1,0,&options_menu_STRUCT,&options_menuhdr,0,6,0,0);
}

// ----------------------------------------------------------------------------------

HEADER_DESC maillist_menuhdr={0,0,131,21,NULL,(int)"��������",LGP_NULL};


int maillist_menu_onkey(void *data, GUI_MSG *msg)
{
  int i;
  int keycode;
  keycode=msg->keys;
  i=GetCurMenuItem(data);
  switch(keycode)
  {
  case 0x18:
    //    GeneralFunc_F1(1);
    create_options_menu(i);
    return(-1);
    
  case 0x3D:
    view_mail_id=create_view(i);
    return(-1);
  } 
  return(0);
}

void maillist_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
    if (request_close_mailmenu)
    {
      request_close_mailmenu=0;
      GeneralFunc_flag1(maillist_menu_id,1);
    }
  }
}

void maillist_menu_iconhndl(void *data, int curitem, int *unk)
{
  //MAIL_HIST* fbuf=buf;
  char* subj;
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  ws=AllocMenuWS(data,50);
  
  if ((subj=find_str(curitem,"Subject: ")))
  {
    ws=AllocMenuWS(data,strlen(subj));
    wsprintf(ws,"%t",subj+sizeof("Subject: ")-1);
    mfree(subj);
  }
  else 
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"%t","������");
  }
  SetMenuItemIconArray(data,item,S_ICONS);
  SetMenuItemText(data,item,ws,curitem);
  SetMenuItemIcon(data,curitem,GetIconIndex(curitem));
}


MENU_DESC maillist_menu=
{
  8,(void *)maillist_menu_onkey,(void*)maillist_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt,
  0x11,
  (void *)maillist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};




//------------------------------------------------------------------------------------

void main_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

int main_menu_onkey(void *data, GUI_MSG *msg)
{
  if (msg->keys==0x18)
  {
    return(-1);
  }

  return(0);
}


void CreateMailList(void)
{
  int mails_num;
  char db_fname[128];
  snprintf(db_fname,sizeof(db_fname),"%s%s",HIST_PATH,mailer_db_name);
  if ((size=open_read_close_file(db_fname,(char**)&buf))!=0xFFFFFFFF)
  {
    mails_num=size/sizeof(MAIL_HIST);
    maillist_menu_id=CreateMenu(0,0,&maillist_menu,&maillist_menuhdr,0,mails_num,0,0);
  }  
  else
  {
    size=0;
  }
}

void remake_mailmenu(void)
{
  if (maillist_menu_id)
  {
    request_remake_mailmenu=1;
    if (IsGuiOnTop(maillist_menu_id))
    {
      GeneralFunc_flag1(maillist_menu_id,1);
    }
    else
    {
      request_close_mailmenu=1;
    }
  }
  else
  {
    CreateMailList();
  }
}


void Incoming(void)
{
  CreateMailList();
}

void Outgoing(void)
{
}


void Options(void)
{
  WSHDR* ws;
  ws=AllocWS(128);
  wsprintf(ws,"%u:\\ZBin\\etc\\EMailClient.bcfg",displace_config);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
}

void Exit(void)
{
  GeneralFuncF1(1);
}

HEADER_DESC mainmenu_HDR={0,0,0,0,NULL,(int)"MailViewer",LGP_NULL};


MENUITEM_DESC mainmenu_ITEMS[4]=
{
  {NULL,(int)"��������",LGP_NULL,0,NULL,0,0x59D},
  {NULL,(int)"���������",LGP_NULL,0,NULL,0,0x59D},
  {NULL,(int)"���������",LGP_NULL,0,NULL,0,0x59D},
  {NULL,(int)"�����",LGP_NULL,0,NULL,0,0x59D},
};

void *mainmenu_HNDLS[6]=
{
  (void *)Incoming,
  (void *)Outgoing,
  (void *)Options,
  (void *)Exit,
};


MENU_DESC mainmenu_STRUCT=
{
  8,(void *)main_menu_onkey,(void*)main_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt,
  0x11,
  NULL,
  mainmenu_ITEMS,   //Items
  mainmenu_HNDLS,   //Procs
  4   //n
};

// ------------------------  Creating CSM -------------------------------- //
void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  mainmenu_HDR.rc.x2=ScreenW();
  mainmenu_HDR.rc.y2=HeaderH();
  csm->gui_id=main_menu_id=CreateMenu(0,0,&mainmenu_STRUCT,&mainmenu_HDR,0,4,0,0);
}

void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)ElfKiller);
}


int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    if ((int)msg->data0==maillist_menu_id)
    {
      if (if_any_change)
      {
        write_db_file();
        mfree(buf);
        if_any_change=0;
      }
      maillist_menu_id=0;
      if (request_remake_mailmenu)
      {
        request_remake_mailmenu=0;
        CreateMailList();
      }
    }
  }
  return(1);
}



const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  maincsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};


void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"MailViewer");
}


int main()
{
  InitConfig();
  
  S_ICONS[0]=ICON0;
  S_ICONS[1]=ICON1;
  S_ICONS[2]=ICON2;
  S_ICONS[3]=ICON3;
  
  LockSched();
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched();
}
