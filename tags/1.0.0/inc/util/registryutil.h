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
      enum COMRegistrationType
      { CLASSES_ROOT,
        PER_MACHINE,
        PER_USER,
        AUTO
      };

      static void RegisterComObjectsInTypeLibrary(Registry& reg,
                                                  LPCTSTR szModulePath,
                                                  COMRegistrationType registrationType=CLASSES_ROOT
                                                 );

      static void RegisterCoClass(Registry& reg,
                                  TLIBATTR* pTypeLib,LPCTSTR typelibName,
                                  REFGUID typeGUID,LPCTSTR typeName,LPCTSTR typeDesc,WORD typeVersion,
                                  LPCTSTR modulePath,
                                  ITypeInfo2& rTypeInfo2,
                                  bool isControl=false,
                                  COMRegistrationType registrationType=CLASSES_ROOT,
                                  LPCTSTR threadingModel=_T("Apartment")
                                 );

      static void RegisterInterface(Registry& reg,
                                    REFGUID typelibGUID,
                                    WORD majorVersion,WORD minorVersion,
                                    REFGUID typeGUID,LPCTSTR typeName,LPCTSTR typeDesc,
                                    COMRegistrationType registrationType=CLASSES_ROOT
                                   );

      static void RegisterTypeLib(Registry& registry,
                                  LPCTSTR typelibDesc,
                                  REFGUID typelibGUID,
                                  LCID lcid,USHORT majorVersion,USHORT minorVersion,
                                  LPCTSTR modulePath,LPCTSTR helpPath,
                                  COMRegistrationType registrationType=CLASSES_ROOT
                                 );

      static void GetKeyPrefix(COMRegistrationType registrationType,TString& keyPrefix);

      static void RegisterTypeLib(bool register4User,REFGUID typelibGUID,LCID lcid,USHORT majorVersion,USHORT minorVersion,LPCTSTR modulePath,LPCTSTR helpPath);
  }; // of class RegistryUtil
} // of namespace bvr20983
#endif // REGISTRYUTIL_H
//=================================END-OF-FILE==============================