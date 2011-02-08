#include "..\include\Dir.h"
#include "..\include\Types.h"
#include "..\include\dll.h"
#include "..\Dlls\LibraryDLL\export\LibraryDLL.h"
#include "calls.h"
#include "vars.h"
#include "regfile.h"
#include "helper.h"
#include "dll.h"
#include "elfloader.h"

#include "temp\target.h"

typedef void (*USERDATACONSTRUCTOR)(void*, void*);

typedef struct
{
	USERDATACONSTRUCTOR constr;
	void * data;
}UDATA;

void * malloc (int size)
{
#if defined(DB2020)
	return(memalloc(0,size,1,5,"SwiLib",0));
#elif defined(A2)
	return(memalloc(0xFFFFFFFF,size,1,5,"SwiLib",0));
#else
	return(memalloc(size,1,5,"SwiLib",0));
#endif
}

void mfree (void * mem)
{
#if defined(DB2020)
	if (mem) memfree(0, mem,"SwiLib",0);
#elif defined(A2)
	if (mem) memfree(0, mem,"SwiLib",0);
#else
	memfree(mem,"SwiLib",0);
#endif
}

int CmpProc(UDATA* listitem, USERDATACONSTRUCTOR itemtofind)
{
	return listitem->constr != itemtofind;
}

void* GetUserData (int size, USERDATACONSTRUCTOR constr, void* param)
{

	UDATA * ud;
	int index;

	_printf("GetUserData(0x%X,0x%X,0x%X)",size,constr,param);
	index = LIST_FIND(elfpackdata->UserDataList, constr, CmpProc);
	if (index!=0xFFFF)
	{
		ud=List_Get(elfpackdata->UserDataList,index);
		_printf("UserData  @0x%X",ud->data);
	}
	else
	{
		if (constr)
		{
			ud=malloc(sizeof(UDATA));
			ud->constr=constr;
			ud->data=malloc(size);
			List_InsertFirst(elfpackdata->UserDataList,ud);
			constr(ud->data,param);

			_printf("New UserData  @0x%X",ud->data);
			_printf("Total Used Bloks  0x%%X",elfpackdata->UserDataList->FirstFree);
		}
		else
		{
			_printf("Error. GetUserData() without Constructor!!");
			// ��� ������������
			return(0);
		}
	}
	return(ud->data);

};



int RemoveUserData(void (*constr)(void *, void *))
{
	UDATA * ud;
	int index;

	_printf("RemoveUserData(0x%X)",constr);

	index = LIST_FIND(elfpackdata->UserDataList, constr, CmpProc);
	if (index!=0xFFFF)
	{
		ud=List_RemoveAt(elfpackdata->UserDataList,index);
		{
			mfree(ud->data);
			mfree(ud);

			_printf("Removed OK...");

			return(0);
		}
	}
	else
	{
		_printf("0x%X Not in List!!",constr);

		// ��� �������� � ������
		return(1);
	}

}

//============================================================================

wchar_t* GetDir (int DirIndex)
{

	_printf("GetDir(0x%X)",DirIndex);

	switch (DirIndex>>1)
	{
	case  DIR_AUDIO>>1 : return (DirIndex&1)? PATH_AUDIO_INT: PATH_AUDIO_EXT;
	case  DIR_IMAGE>>1 : return (DirIndex&1)? PATH_IMAGE_INT: PATH_IMAGE_EXT;
	case  DIR_VIDEO>>1 : return (DirIndex&1)? PATH_VIDEO_INT: PATH_VIDEO_EXT;
	case  DIR_THEME>>1 : return (DirIndex&1)? PATH_THEME_INT: PATH_THEME_EXT;
	case  DIR_OTHER>>1 : return (DirIndex&1)? PATH_OTHER_INT: PATH_OTHER_EXT;

	case  DIR_ELFS>>1 : return (DirIndex&1)? PATH_ELF_INT: PATH_ELF_EXT;
	case  DIR_ELFS_DAEMONS>>1 : return (DirIndex&1)? PATH_ELF_DAEMONS_INT: PATH_ELF_DAEMONS_EXT;
	case  DIR_ELFS_CONFIG>>1 : return (DirIndex&1)? PATH_ELF_CONFIG_INT: PATH_ELF_CONFIG_EXT;
	case  DIR_INI>>1 : return PATH_INI;
	case  DIR_DLL>>1: return PATH_DLL;

	case  DIR_USER>>1 : return (DirIndex&1)? PATH_USER_INT: PATH_USER_EXT;

	default: return(PATH_DEFAULT);
	}
}

int fopen (const wchar_t* fname, int mode, int rights)
{
	wchar_t* path;
	wchar_t* name;
	int len;
	name=wstrrchr(fname,0x2F);
	if (!name) return -1;
	path=malloc(((len=name-fname)+1)*sizeof(wchar_t));
	memcpy(path,fname,len*sizeof(wchar_t));
	path[len]=0;
	name++;
	len=_fopen(path,name,mode,rights,NULL);
	mfree(path);
	return len;
}


//===============  KBD_HOOK  ================

typedef struct
{
	KEYHOOKPROC proc;
	void *data;
}KEY_HOOK_ELEM;

int KeyHookCmpProc(KEY_HOOK_ELEM* listitem, KEYHOOKPROC itemtofind)
{
	return listitem->proc != itemtofind;
}

int ModifyKeyHook( KEYHOOKPROC proc, int mode , void *data )
{
	int i=0;

	_printf("ModifyKeyHook PROC@0x%x, mode=0x%x",proc,mode)  ;

	i = LIST_FIND(elfpackdata->gKbdHookList, proc, KeyHookCmpProc);
	switch (mode)
	{
	case 0: //remove
		if (i!=0xFFFF)
		{
			mfree(List_Get(elfpackdata->gKbdHookList,i));
			List_RemoveAt(elfpackdata->gKbdHookList,i);

			_printf("PROC(0x%x) Removed..",proc)  ;

			return(0);
		}
		else
		{
			_printf("PROC(0x%x) Not in List !!!",proc)  ;

			return(-2);
		}
	case 1: //add
		if (i==0xFFFF)
		{
			KEY_HOOK_ELEM *elem = malloc(sizeof(KEY_HOOK_ELEM));
			elem->proc = proc;
			elem->data = data;
			List_InsertFirst(elfpackdata->gKbdHookList,(void*)elem);

			_printf("PROC(0x%x) Added in List",proc)  ;

			return(0);
		}
		else
		{
			_printf("Warning! PROC(0x%x) was in List",proc)  ;

			return(-3);
		}
	default:
		{
			_printf("Error. Wrong mode(0x%x). Use 0/1 for remove/add",proc)  ;


			return(-4);
		}
	}
}


int Keyhandler_Hook(int key,int mode,int repeat_count)
{
	int i=0;
	int result;
	while(i < elfpackdata->gKbdHookList->FirstFree)
	{
		KEY_HOOK_ELEM *elem = (KEY_HOOK_ELEM *)List_Get(elfpackdata->gKbdHookList,i++);
		result = elem->proc(key,repeat_count,mode,elem->data);
		if (result==-1) return KEY_LAST;
		if (result>0) return result;
	}
	return key;
}
//===============  OSE_HOOK  ================
/*int OSEHookCmpProc(OSE_HOOK_ITEM* ud, void (*e2)(void*))
{
	return ud->HOOK != e2;
}

int ModifyOSEHook(int event , void (*proc)(void*),int mode)
{
	int n = LIST_FIND(elfpackdata->OseHookList, proc, OSEHookCmpProc);

	switch (mode)
	{
	case 0: //remove
		if (n!=0xFFFF)
		{
			mfree(List_RemoveAt(elfpackdata->OseHookList, n));
			return(0);
		}
		else
		{
			return(-2);
		}

	case 1:
		if (n==0xFFFF)
		{
			OSE_HOOK_ITEM *item=malloc(sizeof(OSE_HOOK_ITEM));
			item->signo=event;
			item->HOOK = proc;
			List_InsertFirst(elfpackdata->OseHookList,(void*)item);
			return(0);
		}
		else
		{
			return(-3);
		}
	default: return(-4);
	}
}*/

//===============  UI_HOOK  ================

typedef struct {
	int (*PROC)(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM ClientData);
	int event;
	LPARAM ClientData;
} PAGE_HOOK_ELEM;


int UIHook1CmpProc(void * e1, void * e2)
{
	PAGE_HOOK_ELEM * ud = (PAGE_HOOK_ELEM*)e1;
	PAGE_HOOK_ELEM *page_elem=(PAGE_HOOK_ELEM *)e2;
	if (ud->PROC==page_elem->PROC && ud->event==page_elem->event) return (0);
	else  return(1);
}


int PageAction_Hook2(ACTION *act,void *msg,BOOK * book)
{
	int res=0;
	if (elfpackdata)
	{
		LIST * UIPageHook_Before = elfpackdata->UIPageHook_Before;
		LIST * UIPageHook_After = elfpackdata->UIPageHook_After;
		int n_before=UIPageHook_Before->FirstFree;
		int n_after=UIPageHook_After->FirstFree;
		while(n_before--)
		{
			PAGE_HOOK_ELEM *my_act=(PAGE_HOOK_ELEM *)List_Get(UIPageHook_Before,n_before);

			if (my_act->event==act->event) res=my_act->PROC(msg,book,act->PAGE_DESC,my_act->ClientData);
			if (res==BLOCK_EVENT_GLOBALLY) return res;
			if (res==BLOCK_EVENT_IN_THIS_SESSION) return res;
		}
		res=act->PROC(msg,book);
		while(n_after--)
		{
			PAGE_HOOK_ELEM *my_act=(PAGE_HOOK_ELEM *)List_Get(UIPageHook_After,n_after);

			if (my_act->event==act->event)
				my_act->PROC(msg,book,act->PAGE_DESC,my_act->ClientData);
		}
	}
	return res;
}


int ModifyUIHook1(int event , int (*PROC)(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM ClientData), LPARAM ClientData ,int mode)
{
	LIST * UIPageHook_Before = elfpackdata->UIPageHook_Before;
	LIST * UIPageHook_After = elfpackdata->UIPageHook_After;
	PAGE_HOOK_ELEM page_elem;
	page_elem.PROC=PROC;
	page_elem.event=event;
	page_elem.ClientData=ClientData;
	int n_before = List_Find(UIPageHook_Before,&page_elem,UIHook1CmpProc);
	int n_after = List_Find(UIPageHook_After,&page_elem,UIHook1CmpProc);
	_printf("ModifyUIHook PROC@0x%x, mode=0x%x",PROC,mode)  ;

	switch (mode)
	{
	case 0: //remove
		if (n_before!=LIST_ERROR)
		{
			mfree(List_RemoveAt(UIPageHook_Before,n_before));
			return(0);
		}
		else if (n_after!=LIST_ERROR)
		{
			mfree(List_RemoveAt(UIPageHook_After,n_after));
			return(0);
		}
		else
		{
			return(-2);
		}

	case 1: //insert (before original callback)
		if (n_before==LIST_ERROR)
		{
			ACTION *item=malloc(sizeof(PAGE_HOOK_ELEM));
			memcpy(item,&page_elem,sizeof(PAGE_HOOK_ELEM));
			List_InsertLast(UIPageHook_Before,item);
			return(0);
		}
		else
		{
			return(-3);
		}

	case 2: //insert (after original callback)
		if (n_after==LIST_ERROR)
		{
			ACTION *item=malloc(sizeof(PAGE_HOOK_ELEM));
			memcpy(item,&page_elem,sizeof(PAGE_HOOK_ELEM));
			List_InsertLast(UIPageHook_After,item);
			return(0);
		}
		else
		{
			return(-3);
		}

	default: return(-4);
	}
}

// ===============================================================


extern void* Library;

void CreateLists(void)
{
	elfpackdata = malloc(sizeof(EP_DATA));
	memset(elfpackdata,0,sizeof(EP_DATA));

	_printf("EP_DATA @%x",elfpackdata)  ;

	elfpackdata->UserDataList=List_Create();
	elfpackdata->gKbdHookList=List_Create();
	elfpackdata->UIHookList=List_Create();
	//elfpackdata->OseHookList=List_Create();
	elfpackdata->DLLList=List_Create();
	elfpackdata->UIPageHook_Before=List_Create();
	elfpackdata->UIPageHook_After=List_Create();
	elfpackdata->LibraryCache = NULL;

	_printf("   epd->UserDataList @%x",elfpackdata->UserDataList)  ;
	_printf("   epd->gKbdHookList @%x",elfpackdata->gKbdHookList)  ;
	_printf("   epd->UIHookList @%x",elfpackdata->UIHookList)  ;
	//_printf("   epd->OseHookList @%x",elfpackdata->OseHookList)  ;
	//  _printf("   epd->elflist @%x",elfpackdata->elflist)  ;

	elfpackdata->DBExtList=CreateDBExtList();
	elfpackdata->CreateDbExt = CreateDbExt;
	elfpackdata->IconSmall = NOIMAGE;
	elfpackdata->IconBig = NOIMAGE;
	elfpackdata->LibraryDLL = NULL;
	elfpackdata->LastKey = KEY_LAST;

	set_envp(get_bid(current_process()),"elfpackdata",(OSADDRESS)elfpackdata);

	ELFExtrRegister(elfpackdata);
}


void Init()
{
	FILELISTITEM* mem = malloc(sizeof(FILELISTITEM));

	FILELISTITEM* fli;

	_printf("     Entered Init...")  ;

	// ��������� �������

	_printf("     StartHelper....")  ;

	StartHelper();

	_printf("     StartHelper OK. PID=%x",elfpackdata->HPID)  ;

	// ������ DLL ����������

	_printf("     Load LibraryDLL....")  ;

	elfpackdata->LibraryDLL = LoadDLL_int(L"LibraryDLL.dll");
	if ((INVALID(elfpackdata->LibraryDLL)))
	{
		_printf("     Load LibraryDLL Error")  ;
		elfpackdata->LibraryDLL = 0;
	}
	else _printf("     Load LibraryDLL OK")  ;

	// ������ ��� ����

	if (elfpackdata->LibraryDLL)
	{
		_printf("     Patching LibraryCache....")  ;

		void** lib = malloc(0x4000);
		memcpy(lib, &Library, 0x4000);

		const LIBRARY_DLL_FUNCTIONINFO* fns = ( (const LIBRARY_DLL_DATA*)elfpackdata->LibraryDLL )->functions;
		while (fns->functionnum)
		{
			lib[ fns->functionnum - 0x100 ] = fns->functionptr;
			fns++;
		}

		elfpackdata->LibraryCache = lib;

		_printf("     Patching LibraryCache OK")  ;
	}

	// ��������� �������

	_printf("     StartDaemons....")  ;
	_printf("     ------Begin List-------")  ;

#ifdef DAEMONS_INTERNAL
	DIR_HANDLE* handle=AllocDirHandle(GetDir(DIR_ELFS_DAEMONS | MEM_INTERNAL));
#else
	DIR_HANDLE* handle=AllocDirHandle(GetDir(DIR_ELFS_DAEMONS | MEM_EXTERNAL));
#endif

	do
	{
		if(fli=GetFname(handle,mem))
		{
			wchar_t* filename = malloc((wstrlen(fli->path)+wstrlen(fli->fname)+2)*2);
			wstrcpy(filename,fli->path);
			wstrcat(filename,L"/");
			wstrcat(filename,fli->fname);

			_printf("     ->...")  ;
#ifdef DBG
			_printf("     elfload Result=0x%X",elfload_int(filename,0,0,0));
#else
			elfload_int(filename,0,0,0);
#endif
			mfree(filename);
		}
	}
	while(fli);

	_printf("     ------End List-------")  ;


	if (handle) DestroyDirHandle(handle);
	mfree(mem);

	_printf("     Exit Init")  ;
}

__root  const int PATCH_AUTO_RUN @ "PATCH_AUTO_RUN" =(int)PAGE_ENTER_EVENT;
__root  const int PATCH_AUTO_RUN1 @ "PATCH_AUTO_RUN1" =(int)Init;

int GetExtTable()
{
	return((int)elfpackdata->DBExtList->listdata);
}
