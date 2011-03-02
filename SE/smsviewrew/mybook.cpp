#include "mybook.h"

#define MYBOOKNAME "SMSViewRewritten"
#define MYBOOKBASEPAGE "SMSViewRewritten_BOOK_BasePage"
#define ABOUTTEXT "SMSView rewritten.\nGreetz to DuMOHsmol"

#include "softkeyutil.hpp"

int CMyBook::TerminateElf( CBook** pbookmanbook, CMyBook* book )
{
	book->Free();
	return 1;
}


int CMyBook::ShowAuthorInfo( CBook** pbookmanbook, CMyBook* book )
{
	MessageBox( EMPTY_SID, STR(ABOUTTEXT), NOIMAGE, 1 , 5000, *pbookmanbook );
	return 1;
}


DECLARE_PAGE_DESC_BEGIN( CMyBook::base_page, MYBOOKBASEPAGE )
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END


CMyBook::CMyBook()  :CBook( MYBOOKNAME, &base_page )
{
	//���� � ������ ���������
	textidname2id( _T("MSG_UI_VIEW_MESSAGE_SK"), SID_ANY_LEN, &strid_viewmsg );
	textidname2id( _T("GUI_VIEW_SK"), SID_ANY_LEN, &strid_view );
	//���� � ��������� ��������
	textidname2id( _T("MSG_UI_REPLY_MESSAGE_SK"), SID_ANY_LEN, &strid_reply );
	//���� � ��������� ����������
	textidname2id( _T("MSG_UI_FORWARD_MESSAGE_SK"), SID_ANY_LEN, &strid_forward );
	//���� ��� ������������ ���
	textidname2id( _T("SMIL_VIEWER_STOP_SK"), SID_ANY_LEN, &strid_stop );

	ModifyKeyHook( (KEYHOOKPROC)_HookCBKey, KEY_HOOK_ADD, (LPARAM)this );
}


CMyBook::~CMyBook()
{
	ModifyKeyHook( (KEYHOOKPROC)_HookCBKey, KEY_HOOK_REMOVE, NULL );

	if( !blockbooks.empty() )
		ModifyUIPageHook( CANCEL_EVENT, _HookAction, (LPARAM)this, PAGE_HOOK_REMOVE );

	SUBPROC( mfree_adr(), &ELF_BEGIN );
}


int CMyBook::FindBookExCB_Name( BOOK* book , int* param )
{
	return 0 == strcmp( book->xbook->name, (char*)param );
}

int CMyBook::FindBookExCB_Book( BOOK* book , int* param )
{
	return book == (BOOK*)param;
}


CMyBook* CMyBook::GetExisting()
{
	return static_cast<CMyBook*>( FindBookEx( FindBookExCB_Name, (int*)MYBOOKNAME ) );
}


int CMyBook::_HookCBKey( int key, int repeat_count, int mode, LPARAM lparam, DISP_OBJ* hooked_disp )
{
	return reinterpret_cast<CMyBook*>( lparam )->HookCBKey( key, repeat_count, mode, hooked_disp );
}

int CMyBook::HookCBKey( int key, int repeat_count, int mode, DISP_OBJ* hooked_disp )
{
	if( mode != KBD_SHORT_PRESS )
		return 0;

	if( key != KEY_LEFT && key != KEY_RIGHT && key != KEY_STAR && key != KEY_DIEZ )
		return 0;

	//topbook - ������� ����� (������ ���� ����� ���/���)
	BOOK* topbook = Display_GetTopBook( 0 );
	if( !topbook )
		return 0;

	bool ismms = 0==strcmp( topbook->xbook->name, "SmilViewer Book" );

	//��� ��� �� ������ ������ ����/�����
	if( ismms )
		if( key == KEY_LEFT || key == KEY_RIGHT )
			return 0;

	//������� ������� �����
	if( strcmp( topbook->xbook->name, "SMSViewer Book" ) && !ismms )
		return 0;

	//���� ���� "��������" ��� "���������" ��� "����" (���) - ��� ���
	if( !FindSoftkey( hooked_disp, topbook, strid_reply )
		&& !FindSoftkey( hooked_disp, topbook, strid_forward )
		&& !FindSoftkey( hooked_disp, topbook, strid_stop ) )
		return 0;

	//������� ������������� �����
	BOOK* msguibook = FindBookByID( topbook->xbook->parent_BookID );
	if( !msguibook || 0!=strcmp(msguibook->xbook->name, "MSG UI Book") )
		return 0;

	//������� ����� ������ ���������
	BOOK* msguilistbook = FindBookByID( msguibook->xbook->parent_BookID );
	if( !msguilistbook )
		return 0;
	if( 0!=strcmp(msguilistbook->xbook->name, "MSG UI List Book") //a2
		&& 0!=strcmp(msguilistbook->xbook->name, "MSG UI Book") ) //db2020
		return 0;

	//��� ������ ���������
	if( 0 == msguilistbook->xguilist->guilist->FirstFree )
		return 0;
	GUI_LIST* guimsglist = NULL;
	DISP_OBJ* dispmsglist;
	
	for( int i = 0; i < msguilistbook->xguilist->guilist->FirstFree; i++ )
	{
		GUI_LIST* tmpguimsglist = reinterpret_cast<GUI_LIST*>( List_Get( msguilistbook->xguilist->guilist, i ) );
		dispmsglist = GUIObject_GetDispObject( tmpguimsglist );
		if( !strcmp( "ListObject", DispObject_GetName( dispmsglist ) ) )
		{
			guimsglist = tmpguimsglist;
			break;
		}
	}
	//��� � ����� ����� - �����
	if( !guimsglist )
		return 0;

	//�� ������, ����� �� ��� ��������, ��� ���� �� ��������� � ������ ���� "��������"
	//������� ����� ������. ���� ���� "��������", ������ ��� ����
	if( !FindSoftkey( dispmsglist, msguilistbook, strid_viewmsg )
		&& !FindSoftkey( dispmsglist, msguilistbook, strid_view ) )
		return 0;

	int cnt = ListMenu_GetItemCount( guimsglist );
	int cur = ListMenu_GetSelectedItem( guimsglist );

	cur += key == KEY_LEFT || key == KEY_STAR ? -1 : 1;

	if( cur < 0 )
		cur = cnt - 1;

	if( cur >= cnt )
		cur = 0;

	//��������� ����� � ������, � ���� ��� ����� �������� ���� ����� �������� �����
	if( blockbooks.empty() )
		ModifyUIPageHook(CANCEL_EVENT, _HookAction, (LPARAM)this, PAGE_HOOK_ADD_BEFORE);
	blockbooks.insert( msguilistbook );

	BookObj_ReturnPage( topbook, CANCEL_EVENT );

	ListMenu_SetCursorToItem( guimsglist, cur );

	//�������� ���� "view" ������ ���������� ��������
	CSoftKeyDesc* sk = FindSoftkey( dispmsglist, msguilistbook, strid_viewmsg );
	if( !sk )
		sk = FindSoftkey( dispmsglist, msguilistbook, strid_view );
	DispObject_SoftKeys_ExecuteAction( dispmsglist, sk->GetAction() );

	return -1;
}


int CMyBook::_HookAction(void *msg, BOOK* book, PAGE_DESC * page_desc, LPARAM ClientData, u16 event)
{
	return reinterpret_cast<CMyBook*>( ClientData )->HookAction(msg, book, page_desc, event);
}

int CMyBook::HookAction( void *msg, BOOK* book, PAGE_DESC * page_desc, u16 event)
{
	set<BOOK*>::iterator i = blockbooks.find( book );
	if( i != blockbooks.end() )
	{
		if( !strcmp("MSG_UI_ExtendedMessageList_Page", page_desc->name)
		   ||!strcmp("MSG_UI_MessageList_Page", page_desc->name))
		{
			blockbooks.erase( i );
			if( blockbooks.empty() )
				ModifyUIPageHook(CANCEL_EVENT, _HookAction, (LPARAM)this, PAGE_HOOK_REMOVE);

			//��������� CANCEL_EVENT ��� ����� ��������, ���� �� �������������� ����
			return BLOCK_EVENT_IN_THIS_SESSION;
		}
	}
	return 0;
}
