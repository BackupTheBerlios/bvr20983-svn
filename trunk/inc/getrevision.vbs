' $Id$
' 
' Smartcard Test Script.
' 
' Copyright (C) 2008 Dorothea Wachmann
' 
' This program is free software: you can redistribute it and/or modify
' it under the terms of the GNU General Public License as published by
' the Free Software Foundation, either version 3 of the License, or
' (at your option) any later version.
' 
' This program is distributed in the hope that it will be useful,
' but WITHOUT ANY WARRANTY; without even the implied warranty of
' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
' GNU General Public License for more details.
' 
' You should have received a copy of the GNU General Public License
' along with this program.  If not, see http://www.gnu.org/licenses/.
option explicit

Dim objArgs,fso,f0,f1,f2,r0
Dim reProdVer,reFileVer,reMakeDate,reRev,reDate,reGlobalRev
Dim svnRevision,svnDate,svnVersion
Dim WshShell, oExec

Const ForReading = 1

'
' Usage
'
Sub Usage()
  WScript.Echo "getrevision <filename>"
End Sub


'
' Init
'
Sub Init
'  on error resume next

  Set reProdVer   = New RegExp
  Set reFileVer   = New RegExp
  Set reRev       = New RegExp
  Set reDate      = New RegExp
  Set reMakeDate  = New RegExp
  Set reGlobalRev = New RegExp
  
  reProdVer.Pattern   = "(^#define.+verProdVer.+" & "[0-9]+[\.,][0-9]+[\.,][0-9]+[\.,])([0-9]+)(.*)"
  reProdVer.Global    = True

  reFileVer.Pattern   = "(^#define.+verFileVer.+" & "[0-9]+[\.,][0-9]+[\.,][0-9]+[\.,])([0-9]+)(.*)"
  reFileVer.Global    = True

  reMakeDate.Pattern  = "(^#define.+verMakeDate.+"")(.+)("")"
  reMakeDate.Global   = True

  reRev.Pattern       = "\$Revision:[ \t]+([0-9]+)[ \t]+\$"
  reRev.Global        = True

  reDate.Pattern      = "\$Date:[ \t]+(.+)[ \t]+\$"
  reDate.Global       = True

  reGlobalRev.Pattern = "([0-9]+[MS]?)(:[0-9]+[MS]?)?"
  reGlobalRev.Global  = True
  
  Set WshShell = CreateObject("WScript.Shell")
  Set oExec    = WshShell.Exec("svnversion")

  svnVersion = ""
  
  Do While True
    If Not oExec.StdOut.AtEndOfStream Then
      svnVersion = svnVersion & oExec.StdOut.Read(1)
    Else
      Exit Do
    End If

    WScript.Sleep 10
  Loop
  
  Do While oExec.Status = 0
    WScript.Sleep 100
  Loop
  
  WScript.Echo "svnVersion=" & svnVersion
  
  svnVersion = reGlobalRev.Execute(svnVersion)(0).SubMatches(0)
End Sub

Set objArgs = WScript.Arguments

If objArgs.Count>=1 Then
  Set fso = CreateObject("Scripting.FileSystemObject")
  
  If fso.FileExists( objArgs(0) ) Then
    
    Init
    
    Set f0 = fso.OpenTextFile(objArgs(0), ForReading)
    Set f1 = fso.CreateTextFile(objArgs(0)&".new",True)
    
    Do While Not f0.AtEndOfStream 
      r0 = f0.ReadLine
      
      If reRev.Test(r0) Then
        svnRevision = reRev.Execute(r0)(0).SubMatches(0)
      End If

      If reDate.Test(r0) Then
        svnDate = reDate.Execute(r0)(0).SubMatches(0)
      End If
      
      r0 = reProdVer.Replace(r0,"$1" & svnVersion & "$3")
      r0 = reFileVer.Replace(r0,"$1" & svnRevision & "$3")
      r0 = reMakeDate.Replace(r0,"$1" & svnDate & "$3")
      
      f1.WriteLine(r0)
    Loop

    f0.Close
    f1.Close
    
    Set f2 = fso.GetFile(objArgs(0))
    f2.Delete(True)
    
    Set f2 = fso.GetFile(objArgs(0) & ".new")
    f2.Move(objArgs(0))
  End If
Else
  Usage
End If
'=======================================END-OF-FILE==========================
