

#include "ywin.h"


int Button::OnGetCheck( HWND hWnd ) {

    if ( BS_AUTOCHECKBOX & dwStyle ) {

        if ( BST_INDETERMINATE == iButtonstate ) {

            return BST_UNCHECKED;

        } else {

            return iButtonstate;

        }

    } else {

        /* Button */
        return BST_INDETERMINATE;

    }

}


void Button::AutoCheck() {

    switch( iButtonstate ) {

        case BST_CHECKED:

            iButtonstate = BST_UNCHECKED;
            break;

        case BST_INDETERMINATE:
        case BST_UNCHECKED:
        default:

            iButtonstate = BST_CHECKED;
            break;

    }

}


void Button::OnKeydown( HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags ) {

    /* Check for special keys */
    switch( vk ) {

        case VK_SPACE:

            if ( ( BS_AUTOCHECKBOX & dwStyle ) == BS_AUTOCHECKBOX ) {

                AutoCheck();

            }

            if ( ( BS_OWNERDRAW & dwStyle ) == BS_OWNERDRAW ) {

                /* Notify parent */
                DRAWITEMSTRUCT dis;

                dis.CtlID      = (UINT)(unsigned long)hMenu;
                dis.CtlType    = ODT_BUTTON;
                dis.hDC        = &ClientDC;
                dis.hwndItem   = hWnd;
                dis.itemAction = ODA_DRAWENTIRE | ODA_SELECT;
                dis.itemData   = 0;
                dis.itemID     = -1;
                dis.itemState  = GetFocus() == hWnd ? ODS_FOCUS : 0;

                GetClientRect( hWnd, &dis.rcItem );

                FORWARD_WM_DRAWITEM( pParentWnd, &dis, SendMessage );

            }

            RedrawWindow( hWnd, NULL, NULL, RDW_ERASE | RDW_ERASENOW | RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_UPDATENOW );

            /* Notify parent */
            FORWARD_WM_COMMAND( pParentWnd, (unsigned long)hMenu, hWnd, BN_CLICKED, PostMessage );
            break;

    }

}


void Button::OnKeyup( HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags ) {

    /* Check for special keys */
    switch( vk ) {

        case VK_SPACE:

            if ( ( BS_OWNERDRAW & dwStyle ) == BS_OWNERDRAW ) {

                /* Notify parent */
                DRAWITEMSTRUCT dis;

                dis.CtlID      = (UINT)(unsigned long)hMenu;
                dis.CtlType    = ODT_BUTTON;
                dis.hDC        = &ClientDC;
                dis.hwndItem   = hWnd;
                dis.itemAction = ODA_DRAWENTIRE;
                dis.itemData   = 0;
                dis.itemID     = -1;
                dis.itemState  = GetFocus() == hWnd ? ODS_FOCUS : 0;

                GetClientRect( hWnd, &dis.rcItem );

                FORWARD_WM_DRAWITEM( pParentWnd, &dis, SendMessage );

            }
            break;

    }

}


void Button::OnLButtonDown( HWND hWnd, BOOLEAN bDblClick, int /* userx */, int /* usery */, UINT uiKeyFlags ) {

    if ( WS_DISABLED & dwStyle ) {

        return;

    }

    if ( ( BS_GROUPBOX & dwStyle ) == BS_GROUPBOX ) {

        return;

    }

    if ( ( BS_AUTOCHECKBOX & dwStyle ) == BS_AUTOCHECKBOX ) {

        AutoCheck();

    }

    RedrawWindow( hWnd, NULL, NULL, RDW_ERASE | RDW_ERASENOW | RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_UPDATENOW );

    if ( NULL == pParentWnd ) {

        DBG_MSG( DBG_ERROR, TEXT( "NOTIFICATION LOST" ) );
        return;

    }

    FORWARD_WM_COMMAND( pParentWnd, (unsigned long)hMenu, hWnd, BN_CLICKED, PostMessage );

}


void Button::DrawGroupBox( HWND hWnd, RECT * pr, PAINTSTRUCT * pPS ) {

    /* Calculate the text's line size */
    SIZE s;
    GetTextExtentPoint( pPS->hdc, Text.c_str(), Text.size(), &s );

    TEXTMETRIC tm;
    GetTextMetrics( pPS->hdc, &tm );

    RECT   r;

    r.left   = pr->left + tm.tmAveCharWidth;
    r.top    = pr->top;
    r.right  = r.left + s.cx;
    r.bottom = r.top + s.cy;

    /* Draw it on the first line, on character over from the top-left corner */
    DrawText( pPS->hdc, Text.c_str(), Text.size(), &r, DT_SINGLELINE );

    /* Exclude that rectangle from being drawn over by DrawEdge */
    ExcludeClipRect( pPS->hdc, r.left, r.top, r.right, r.bottom );

    pr->top += ( tm.tmHeight / 2 );

#ifdef YOU_WIN_GRAPHICAL
    DrawEdge( pPS->hdc, pr, BDR_RAISEDOUTER, BF_RECT );
#endif

    SelectClipRegion( pPS->hdc, NULL );

}


void Button::DrawCheckBox( HWND hWnd, RECT * pr, PAINTSTRUCT * pPS ) {

    /*
     * Get the font metrics
     * define the checkbox-check area as the height of the font by the average width of the font characters.
     * draw the checkbox-check area
     * shrink the client area by the checkbox-check's width
     * draw the accompagnying text in the remaining rectangle
     */

    TEXTMETRIC TM;

    GetTextMetrics( pPS->hdc, &TM );

    RECT r;

    unsigned BoxSide = TM.tmAveCharWidth * 2;

    r.left  = pr->left;
    r.right = r.left + BoxSide;

    r.top = pr->top + ( ( pr->bottom - pr->top ) / 2 ) - ( BoxSide / 2 );

    r.bottom = r.top + BoxSide;

    FillRect( pPS->hdc, &r, GetSysColorBrush( COLOR_WINDOW ) );

#ifndef YOU_WIN_TXT
    if ( BST_CHECKED == OnGetCheck( hWnd ) ) {

        MoveToEx( pPS->hdc, r.left, r.top, NULL );
        LineTo( pPS->hdc, r.right, r.bottom );

        MoveToEx( pPS->hdc, r.right, r.top, NULL );
        LineTo( pPS->hdc, r.left, r.bottom );

    }

    DrawEdge( pPS->hdc, &r, BDR_SUNKENINNER, BF_RECT );
#else

#endif

    r.left   = r.right + ( TM.tmAveCharWidth / 2 );
    r.top    = pr->top;
    r.right  = pr->right;
    r.bottom = pr->bottom;

#ifdef YOU_WIN_GRAPHICAL

//    if ( BST_CHECKED == OnGetCheck( hWnd ) ) {

//        DrawEdge( pPS->hdc, pr, BDR_SUNKENINNER, BF_RECT );

//    } else {

//        DrawEdge( pPS->hdc, pr, BDR_RAISEDOUTER, BF_RECT );

//    }

    if ( GetFocus() == hWnd ) {

        DrawFocusRect( pPS->hdc, pr );

    }

#else
#endif

    DrawText( pPS->hdc, Text.c_str(), Text.size(), &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
//              ( dwStyle & BS_CENTER    ? DT_CENTER : DT_LEFT       )
//              DT_CENTER
//              |
//              ( dwStyle & BS_MULTILINE ? 0         : DT_SINGLELINE )
//    );

}


void Button::DrawIcon( HWND hWnd, RECT * pr, PAINTSTRUCT * pPS ) {
}


void Button::DrawBitmap( HWND hWnd, RECT * pr, PAINTSTRUCT * pPS ) {

    if ( ( BS_OWNERDRAW & dwStyle ) == BS_OWNERDRAW ) {

        DRAWITEMSTRUCT dis;

        dis.CtlID      = (UINT)(unsigned long)hMenu;
        dis.CtlType    = ODT_BUTTON;
        dis.hDC        = pPS->hdc;
        dis.hwndItem   = hWnd;
        dis.itemAction = ODA_DRAWENTIRE;
        dis.itemData   = 0;
        dis.itemID     = -1;
        dis.itemState  = GetFocus() == hWnd ? ODS_FOCUS : 0;
        dis.rcItem     = *pr;

        FORWARD_WM_DRAWITEM( GetParent( hWnd ), &dis, SendMessage );
        return;

    }


}


void Button::DrawOwnerDraw( HWND hWnd, RECT * pr, PAINTSTRUCT * pPS ) {

    DRAWITEMSTRUCT di;

    HWND hParentWnd = GetParent( hWnd );
    RECT rcItem;
    rcItem( 0, 0, 0, 0 );

    GetClientRect( hWnd, &rcItem );

    di.CtlType    = ODT_BUTTON;
    di.CtlID      = GetDlgCtrlID( hWnd );
    di.itemID     = 0;

    di.itemAction = ODA_DRAWENTIRE;

    if ( GetFocus() == hWnd ) {

        di.itemAction |= ODA_FOCUS;

    }

    di.itemState  = 0;
    di.hwndItem   = hWnd;
    di.hDC        = pPS->hdc;
    di.rcItem     = rcItem;
    di.itemData   = 0;

    FORWARD_WM_DRAWITEM( hParentWnd, &di, SendMessage );

}


void Button::OnPaint( HWND hWnd, PAINTSTRUCT * pPS ) {

    COLORREF oldbkcr = GetBkColor( pPS->hdc );
    COLORREF oldfgcr = GetTextColor( pPS->hdc );
    int iOldBkMode   = SetBkMode( pPS->hdc, TRANSPARENT );

    SetTextColor( pPS->hdc, ( WS_DISABLED & dwStyle ) ? SysColorGrayText.GetColor() : SysColorBtnText.GetColor() );

    RECT r;
    GetClientRect( hWnd, &r );

    COLORREF bkcr;

#ifdef YOU_WIN_TXT
    SolidBrush br( COLORREF_COLOR_GRAY, true );
    FillRect( pPS->hdc, &r, (HBRUSH)&br );
    bkcr = COLORREF_COLOR_GRAY;
#else
    bkcr = SysColor3DFace.GetColor();
#endif

    SetBkColor( pPS->hdc, bkcr );

    /* Types of buttons */
    switch( 0xFF & dwStyle ) {

        case BS_CHECKBOX:

            DrawCheckBox( hWnd, &r, pPS );
            return;

        case BS_AUTOCHECKBOX:

            DrawCheckBox( hWnd, &r, pPS );
            return;

        case BS_3STATE:

            DrawCheckBox( hWnd, &r, pPS );
            return;

        case BS_AUTO3STATE:

            DrawCheckBox( hWnd, &r, pPS );
            return;

        case BS_GROUPBOX:

            DrawGroupBox( hWnd, &r, pPS );
            return;

        case BS_USERBUTTON:

            return;

        case BS_OWNERDRAW:

            DrawOwnerDraw( hWnd, &r, pPS );
            return;

        case BS_ICON:

            DrawIcon( hWnd, &r, pPS );
            return;

        case BS_BITMAP:

            DrawBitmap( hWnd, &r, pPS );
            return;

        default:

            /* By process of elimination, all that is left is a normal button */
            DrawText( pPS->hdc, Text.c_str(), Text.size(), &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

            SetBkMode( pPS->hdc, iOldBkMode );
            SetBkColor( pPS->hdc, oldbkcr );
            SetTextColor( pPS->hdc, oldfgcr );
            break;

    }

    /* Formatting styles */
    switch( 0xFF00 & dwStyle ) {

        case BS_LEFT:

            break;

        case BS_CENTER:

            break;

        case BS_VCENTER:

            break;

        case BS_MULTILINE:

            break;

        case BS_FLAT:

            DrawEdge( pPS->hdc, &r, BDR_SUNKENINNER, BF_RECT | BF_FLAT );
            break;

        default:

            DrawEdge( pPS->hdc, &r, BDR_RAISEDOUTER, BF_RECT );
            break;

    }

#ifdef YOU_WIN_GRAPHICAL

    if ( GetFocus() == hWnd ) {

        DrawFocusRect( pPS->hdc, &r );

    }

#endif

}


void staticstyles( DWORD dwStyle, TSTRING & styles ) {

    /* Types of buttons */
    switch( 0xFF & dwStyle ) {

        case BS_CHECKBOX:

            styles += TEXT( " BS_CHECKBOX" );
            break;

        case BS_AUTOCHECKBOX:

            styles += TEXT( " BS_AUTOCHECKBOX" );
            break;

        case BS_3STATE:

            styles += TEXT( " BS_3STATE" );
            break;

        case BS_AUTO3STATE:

            styles += TEXT( " BS_AUTO3STATE" );
            break;

        case BS_GROUPBOX:

            styles += TEXT( " BS_GROUPBOX" );
            break;

        case BS_USERBUTTON:

            styles += TEXT( " BS_USERBUTTON" );
            break;

        case BS_OWNERDRAW:

            styles += TEXT( " BS_OWNERDRAW" );
            break;

        case BS_ICON:

            styles += TEXT( " BS_ICON" );
            break;

        case BS_BITMAP:

            styles += TEXT( " BS_BITMAP" );
            break;

    }

    /* Formatting styles */
    switch( 0xFF00 & dwStyle ) {

        case BS_LEFT:

            styles += TEXT( " BS_LEFT" );
            break;

        case BS_CENTER:

            styles += TEXT( " BS_CENTER" );
            break;

        case BS_VCENTER:

            styles += TEXT( " BS_VCENTER" );
            break;

        case BS_MULTILINE:

            styles += TEXT( " BS_MULTILINE" );
            break;

        case BS_FLAT:

            styles += TEXT( " BS_FLAT" );
            break;

    }

}


void Button::OnChar( HWND hWnd, TCHAR ch, int cRepeat ) {

    switch( ch ) {

        case ' ':

            /* For a button, receiving a space is the same as being clicked/toggled */
            FORWARD_WM_LBUTTONDOWN( hWnd, false, 0, 0, 0, SendMessage );
            break;

    }

}


LRESULT Button::WndProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam ) {

    Button * pWnd = reinterpret_cast<Button *>( hWnd );

    switch( uiMsg ) {

        case BM_GETCHECK:

            return HANDLE_BM_GETCHECK( hWnd, wParam, lParam, pWnd->OnGetCheck );

        case WM_CHAR:

            return HANDLE_WM_CHAR( hWnd, wParam, lParam, pWnd->OnChar );

        case WM_CREATE: {

                TSTRING styles;
                staticstyles( pWnd->dwStyle, styles );
                DBG_MSG( DBG_WINDOW_MESSAGES, TEXT( "BUTTON - WM_CREATE:%s" ), styles.c_str() );

            }
            break;

        case WM_KEYDOWN:

            HANDLE_WM_KEYDOWN( hWnd, wParam, lParam, pWnd->OnKeydown );
            break;

        case WM_KEYUP:

            HANDLE_WM_KEYUP( hWnd, wParam, lParam, pWnd->OnKeyup );
            break;

        case WM_KILLFOCUS:
        case WM_SETFOCUS:

            RedrawWindow( hWnd, NULL, NULL, RDW_ERASE | RDW_ERASENOW | RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_UPDATENOW );
            break;

        case WM_LBUTTONDOWN:

            HANDLE_WM_LBUTTONDOWN( hWnd, wParam, lParam, pWnd->OnLButtonDown );
            break;

        case WM_PAINT: {

                if ( ! IsWindowVisible( hWnd ) ) {

                    return true;

                }

                PAINTSTRUCT ps;

                ZeroMemory( &ps, sizeof( ps ) );

                BeginPaint( hWnd, &ps );

                pWnd->OnPaint( hWnd, &ps );

                EndPaint( hWnd, &ps );

            }

            return true;

    }

    return DefWindowProc( hWnd, uiMsg, wParam, lParam );

}
