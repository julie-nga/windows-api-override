/*
Copyright (C) 2004-2016 Jacquelin POTIER <jacquelin.potier@free.fr>
Dynamic aspect ratio code Copyright (C) 2004-2016 Jacquelin POTIER <jacquelin.potier@free.fr>

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

#include "IniFile.h"
#include <stdio.h>

CIniFile::CIniFile(TCHAR* FileName)
{
    // store file name
    _tcsncpy(this->FilePath,FileName,CIniFile_MAX_PATH);
    this->FilePath[CIniFile_MAX_PATH-1]=0;

    this->bInitSuccess = FALSE;
    this->bFileAlreadyExisting = CStdFileOperations::DoesFileExists(this->FilePath);

    if (this->bFileAlreadyExisting)
        this->bInitSuccess = TRUE;
    else
    {
        // directory must exists for IniFile (else WritePrivateProfileString can fail on some systems)
        this->bInitSuccess = CStdFileOperations::CreateDirectoryForFile(this->FilePath);

#if (defined(UNICODE)||defined(_UNICODE))
        // windows sucks by storing unicode data to acsii files (so an implicit data loss conversion is used)
        // and so the WritePrivateProfileStringW store ascii data :'(
        // --> force file encoding to unicode by prepending unicode header to text file
        HANDLE hFile = ::CreateFile(this->FilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        if (hFile!=INVALID_HANDLE_VALUE)
        {
            this->bInitSuccess = TRUE;
            DWORD dwWrittenBytes;
            // in unicode mode, write the unicode little endian header (FFFE)
            BYTE pbUnicodeHeader[2]={0xFF,0xFE};
            ::WriteFile(hFile,pbUnicodeHeader,2,&dwWrittenBytes,NULL);
            ::CloseHandle(hFile);
        }
#ifdef _DEBUG
        else
        {
            if (::IsDebuggerPresent())
                ::DebugBreak();
        }
#endif
#endif

    }

    // set default section
    _tcscpy(this->CurrentSection,_T("Default"));
}
CIniFile::~CIniFile()
{

}

BOOL CIniFile::SetCurrentSectionName(TCHAR* CurrentSection)
{
    // avoid to affect current section name if size mismatch
    if ((_tcslen(CurrentSection)+1)>CIniFile_SECTION_MAX_LEN)
        return FALSE;

    _tcscpy(this->CurrentSection,CurrentSection);
    return TRUE;
}

BOOL CIniFile::GetCurrentSectionName(IN OUT TCHAR* SectionName,IN DWORD SectionNameMaxSize)
{
    _tcsncpy(SectionName,this->CurrentSection,SectionNameMaxSize);
    SectionName[SectionNameMaxSize-1]=0;
    return TRUE;
}

const TCHAR* CIniFile::GetCurrentSectionName()
{
    return (const TCHAR*)this->CurrentSection;
}

BOOL CIniFile::GetFileName(IN OUT TCHAR* FileName,IN DWORD FileNameMaxSize)
{
    _tcsncpy(FileName,this->FilePath,FileNameMaxSize);
    FileName[FileNameMaxSize-1]=0;
    return TRUE;
}

const TCHAR* CIniFile::GetFileName()
{
    return (const TCHAR*)this->FilePath;
}

BOOL CIniFile::Set(TCHAR* Section,TCHAR* Name, TCHAR* Value)
{
    return ::WritePrivateProfileString(Section,Name,Value,this->FilePath);
}
BOOL CIniFile::Set(TCHAR* Section,TCHAR* Name, DWORD Value)
{
    TCHAR szValue[64];
    _ultot(Value,szValue,10);
    return this->Set(Section,Name,szValue);
}
BOOL CIniFile::Set(TCHAR* Name, TCHAR* Value)
{
    return this->Set(this->CurrentSection,Name,Value);
}
BOOL CIniFile::Set(TCHAR* Name, DWORD Value)
{
    return this->Set(this->CurrentSection,Name,Value);
}

BOOL CIniFile::Set(TCHAR* Name, ULONG64 Value)
{
    TCHAR szValue[128];
    _sntprintf(szValue,_countof(szValue),TEXT("%I64u"),Value);
    return this->Set(this->CurrentSection,Name,szValue);
}

// return FALSE if string length is not enough
BOOL CIniFile::Get(TCHAR* Section,TCHAR* Name,TCHAR* DefaultValue,OUT TCHAR* Value,DWORD MaxValueSize)
{
    return (::GetPrivateProfileString(Section,Name,DefaultValue,Value,MaxValueSize,this->FilePath)<(MaxValueSize-2));
}
BOOL CIniFile::Get(TCHAR* Section,TCHAR* Name,DWORD DefaultValue,OUT DWORD* pValue)
{
    *pValue=::GetPrivateProfileInt(Section,Name,DefaultValue,this->FilePath);
    return TRUE;
}
BOOL CIniFile::Get(TCHAR* Name,TCHAR* DefaultValue, OUT TCHAR* Value,DWORD MaxValueSize)
{
    return this->Get(this->CurrentSection,Name,DefaultValue,Value,MaxValueSize);
}
BOOL CIniFile::Get(TCHAR* Name,DWORD DefaultValue, OUT DWORD* pValue)
{
    return this->Get(this->CurrentSection,Name,DefaultValue,pValue);
}

BOOL CIniFile::Get(TCHAR* Name,ULONG64 DefaultValue, OUT ULONG64* pValue)
{
    TCHAR szValue[128];
    TCHAR szDefaultValue[128];
    _sntprintf(szDefaultValue,_countof(szDefaultValue),TEXT("%I64u"),DefaultValue);
    if (!this->Get(this->CurrentSection,Name,szDefaultValue,szValue,_countof(szValue)))
    {
        *pValue = DefaultValue;
        return FALSE;
    }
    if ( _sntscanf(szValue,_countof(szValue),TEXT("%I64u"),pValue) == 1 )
        return TRUE;
    else
    {
        *pValue = DefaultValue;
        return FALSE;
    }

}

BOOL CIniFile::SetBinary(IN TCHAR* Name, IN PBYTE Buffer, IN DWORD BufferSize)
{
    TCHAR* String;
    TCHAR* pString;
    PBYTE pBuffer;
    PBYTE pBufferEnd;
    pBufferEnd = Buffer+BufferSize;

    try
    {
        String = new TCHAR[BufferSize*2+1];
    }
    catch (...)
    {
        return FALSE;
    }
    
    if (String==NULL)
        return FALSE;

    for (pBuffer=Buffer,pString = String;
         pBuffer<pBufferEnd;
         pBuffer++,pString+=2
         )
    {
        _sntprintf(pString,2,TEXT("%02X"),*pBuffer);
    }
    *pString=0;
    BOOL bRetValue = this->Set(Name,String);

    delete[] String;

    return bRetValue;
}

BOOL CIniFile::GetBinary(IN TCHAR* Name,IN OUT PBYTE Buffer,IN DWORD BufferMaxSize,OUT DWORD* pReadSize)
{
    *pReadSize = 0;

    DWORD StringMaxSize = BufferMaxSize*2+1+2;// +1: for \0 
                                              // + 2 for BOOL CIniFile::Get(TCHAR* Section,TCHAR* Name,TCHAR* DefaultValue,OUT TCHAR* Value,DWORD MaxValueSize)
                                              //   which return FALSE if string length is not enough (detection with not 0 at the end of string)
    TCHAR* String;

    try
    {
        String = new TCHAR[StringMaxSize];
    }
    catch (...)
    {
        return FALSE;
    }

    if (String==NULL)
        return FALSE;

    BOOL bRetValue = this->Get(Name,TEXT(""),String,StringMaxSize);
    if (!bRetValue)
        return FALSE;

    SIZE_T BufferSize = __min(_tcslen(String)/2,BufferMaxSize); // buffer overflow protection
    PBYTE pBuffer;
    PBYTE pBufferEnd;
    TCHAR* pString;
    DWORD Tmp;
    pBufferEnd = Buffer+BufferSize;

    *pReadSize = (DWORD)BufferSize;

    for (pBuffer=Buffer,pString = String;
         pBuffer<pBufferEnd;
         pBuffer++,pString+=2
         )
    {
        // if (_stscanf(pString,TEXT("%02hhX"),pBuffer) != 1)
        if (_stscanf(pString,TEXT("%02X"),&Tmp) != 1)
        {
            *pReadSize = (DWORD)(pBuffer-Buffer);
            break;
        }
        *pBuffer = (BYTE)Tmp;
    }

    delete[] String;

    
    return TRUE;
}

BOOL CIniFile::EnumSections(pfEnumSectionsCallback pCallback, PVOID CallbackUserParam)
{
    #define DEFAULT_SECTIONS_STRING_LEN 2048
    TCHAR* lpReturnedStringList = NULL;

    DWORD nSize = DEFAULT_SECTIONS_STRING_LEN;
    DWORD dwRet;

    do
    {
        // free previous allocated buffer if any
        if (lpReturnedStringList)
        {
            delete[] lpReturnedStringList;
            nSize *= 2;
        }

        // allocate buffer for sections
        lpReturnedStringList = new TCHAR[nSize];

        // get sections list
        dwRet = ::GetPrivateProfileSectionNames(lpReturnedStringList,nSize,this->FilePath);

    }while (dwRet == (nSize-2) ); // loop until the buffer is too small to contain the list of all the sections

    TCHAR* pCurrentSection = NULL;
    pCurrentSection = lpReturnedStringList;     
    for (;;)
    {
        // if current section name is empty, that's mean we have reach the end of the list
        if(*pCurrentSection == 0)
            break;

        // call provided callback
        if (pCallback(pCurrentSection,CallbackUserParam) == FALSE)
            break;

        // get next section
        pCurrentSection += _tcslen(pCurrentSection) + 1;// go after \0 of current string
    } 

    // free memory
    if (lpReturnedStringList)
        delete[] lpReturnedStringList;

    return TRUE;
}
