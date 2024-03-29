

#include "ywin.h"


int GetWindowText( HWND hWnd, LPTSTR pszDest, int iMaxLen ) {

    ywWindow * pWnd;

    if ( ! ( pWnd = IsWnd( hWnd ) ) ) {

        return false;

    }

    _tcsncpy( pszDest, pWnd->Text.c_str(), iMaxLen );

    return _tcslen( pszDest );

}


int GetWindowTextLength( HWND hWnd ) {

    ywWindow * pWnd;

    if ( ! ( pWnd = IsWnd( hWnd ) ) ) {

        return -1;

    }

    return pWnd->Text.size();

}
