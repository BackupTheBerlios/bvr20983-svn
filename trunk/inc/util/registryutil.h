/*
 * $Id$
 * 
 * Copyright (C) 2008 Dorothea Wachmann
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */
#if !defined(REGISTRYUTIL_H)
#define REGISTRYUTIL_H

#include "os.h"
#include "util/logstream.h"
#include "util/registry.h"
#include <iostream>
#include <fstream>
#include <stack>

namespace bvr20983
{
  class RegistryUtil
  {
    public:
      static void RegisterCoClass(TLIBATTR* pTypeLib,LPCTSTR typelibName,
                                  REFGUID typeGUID,LPCTSTR typeName,LPCTSTR typeDesc,WORD typeVersion,
                                  LPCTSTR modulePath,
                                  ITypeInfo2& rTypeInfo2,
                                  bool isControl=false,
                                  LPCTSTR threadingModel=_T("Apartment"),
                                  LPCTSTR dumpFileName=NULL
                                 );

      static void UnregisterCoClass(TLIBATTR* pTypeLib,LPCTSTR typelibName,
                                    REFGUID typeGUID,LPCTSTR typeName,WORD typeVersion,
                                    LPCTSTR dumpFileName=NULL
                                   );

      static void RegisterInterface(REFGUID typelibGUID,WORD majorVersion,WORD minorVersion,REFGUID typeGUID,LPCTSTR typeDesc);
      static void UnregisterInterface(REFGUID objGUID);
      
      static void RegisterTypeLib(REFGUID typelibGUID,LCID lcid,LPCTSTR resId,USHORT majorVersion,USHORT minorVersion,LPCTSTR modulePath,LPCTSTR helpPath);

      static void RegisterComObjectsInTypeLibrary(LPCTSTR szModulePath,bool registerTypes,LPCTSTR dumpFileName=NULL);
  }; // of class RegistryUtil
} // of namespace bvr20983
#endif // REGISTRYUTIL_H
//=================================END-OF-FILE==============================