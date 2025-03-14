/*
Copyright (C) 2019-2020 Jacquelin POTIER <jacquelin.potier@free.fr>
Dynamic aspect ratio code Copyright (C) 2019-2020 Jacquelin POTIER <jacquelin.potier@free.fr>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//-----------------------------------------------------------------------------
// Object: provides an interface to write configuration from an already opened section / key
//         avoid that caller use another section / key
//-----------------------------------------------------------------------------


#pragma once

#include <windows.h>
#pragma warning (push)
#pragma warning(disable : 4005)// for '_stprintf' : macro redefinition in tchar.h
#include <TCHAR.h>
#pragma warning (pop)

class IConfigSingleSectionWriter
{
public:
    virtual ~IConfigSingleSectionWriter(){};

    virtual BOOL Set(TCHAR* Name, TCHAR* Value)=0;
    virtual BOOL Set(TCHAR* Name, DWORD Value)=0;
    virtual BOOL Set(TCHAR* Name, ULONG64 Value)=0;
    virtual BOOL SetBinary(IN TCHAR* Name, IN PBYTE Buffer, IN DWORD BufferSize)=0;
};
