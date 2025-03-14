/*
Copyright (C) 2004-2016 Jacquelin POTIER <jacquelin.potier@free.fr>
Dynamic aspect ratio code Copyright (C) 2004-2015 Jacquelin POTIER <jacquelin.potier@free.fr>

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

#pragma once

#include <windows.h>
#pragma warning (push)
#pragma warning(disable : 4005)// for '_stprintf' : macro redefinition in tchar.h
#include <TCHAR.h>
#pragma warning (pop)

#include "../File/StdFileOperations.h"

#include "IConfigReader.h"
#include "IConfigWriter.h"

#define CIniFile_MAX_PATH           4096
#define CIniFile_SECTION_MAX_LEN    512

class CIniFile: public virtual IConfigReader, public virtual IConfigWriter
{
protected:
    TCHAR FilePath[CIniFile_MAX_PATH];
    TCHAR CurrentSection[CIniFile_SECTION_MAX_LEN];
    BOOL bInitSuccess;
    BOOL bFileAlreadyExisting;
public:
    // IConfigSection
    virtual BOOL SetCurrentSectionName(TCHAR* CurrentSection);
    virtual BOOL GetCurrentSectionName(IN OUT TCHAR* SectionName,IN DWORD SectionNameMaxSize);
    virtual const TCHAR* GetCurrentSectionName();

    // IConfigSingleSectionReader
    virtual BOOL Get(TCHAR* Name,TCHAR* DefaultValue, OUT TCHAR* Value,DWORD MaxValueSize);
    virtual BOOL Get(TCHAR* Name,DWORD DefaultValue,OUT DWORD* pValue);
    virtual BOOL Get(TCHAR* Name,ULONG64 DefaultValue, OUT ULONG64* pValue);
    virtual BOOL GetBinary(IN TCHAR* Name,IN OUT PBYTE Buffer,IN DWORD BufferMaxSize,OUT DWORD* pReadSize);

    // IConfigReader
    virtual BOOL Get(TCHAR* Section,TCHAR* Name,TCHAR* DefaultValue,OUT TCHAR* Value,DWORD MaxValueSize);
    virtual BOOL Get(TCHAR* Section,TCHAR* Name,DWORD DefaultValue,OUT DWORD* pValue);

    // IConfigWriter
    virtual BOOL Set(TCHAR* Name, TCHAR* Value);
    virtual BOOL Set(TCHAR* Name, DWORD Value);
    virtual BOOL Set(TCHAR* Name, ULONG64 Value);
    virtual BOOL SetBinary(IN TCHAR* Name, IN PBYTE Buffer, IN DWORD BufferSize);

    // IConfigWriter
    virtual BOOL Set(TCHAR* Section,TCHAR* Name, TCHAR* Value);
    virtual BOOL Set(TCHAR* Section,TCHAR* Name, DWORD Value);

    // CIniFile
    CIniFile(TCHAR* FileName);
    virtual ~CIniFile();
    FORCEINLINE BOOL IsFileOpenSuccessfull(){return this->bInitSuccess;}
    FORCEINLINE BOOL DoesIniFileAlreadyExists(){return this->bFileAlreadyExisting;}
    virtual BOOL GetFileName(IN OUT TCHAR* FileName,IN DWORD FileNameMaxSize);
    virtual const TCHAR* GetFileName();

    typedef BOOL (*pfEnumSectionsCallback)(TCHAR* SectionName,PVOID CallbackUserParam);
    virtual BOOL EnumSections(pfEnumSectionsCallback pCallback, PVOID CallbackUserParam);
};