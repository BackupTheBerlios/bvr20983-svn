# $Id$
# 
# Copyright (C) 2008 Dorothea Wachmann
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
!include <./inc/Makefile.inc>

!IF "$(NODEBUG)" == ""
SLNDIR  = .\debug
DISTDIR = .\dist-debug
!ELSE
SLNDIR  = .\release
DISTDIR = .\dist-release
!ENDIF

SIGNDIR       = .\sign
INCDIR        = .\inc
HTMLDIR       = .\html
SCRIPTSDIR    = .\scripts
SIGNPUBKEYTOK = 93425facf1ef717a
signvars      = sign /f $(signkey) /d "$(BVR20983DESC)" /du "https://bvr20983.berlios.de/" /t http://timestamp.verisign.com/scripts/timstamp.dll
patch         = cscript //nologo //job:patch $(SCRIPTSDIR)\patch.wsf /file:$(INCDIR)\ver\versions.xml /select:"/v:versions/" 

PROJECTS =     \
libbvr20983\~  \
bvr20983msgs\~ \
bvr20983cc\~   \
bvr20983sc\~   \
digiclock\~    \
lsstg\~        \
lstypeinfo\~

CABCONTENT = \
$(INCDIR)\$(BVR20983_RESULT).inf \
$(SIGNDIR)\$(BVR20983MSGS_RESULT).dll \
$(SIGNDIR)\$(BVR20983SC_RESULT).dll \
$(SIGNDIR)\$(BVR20983CC_RESULT).dll \
$(SIGNDIR)\$(DIGICLOCK_RESULT).exe \
$(SIGNDIR)\$(LSSTG_RESULT).exe \
$(SIGNDIR)\$(LSTYPEINFO_RESULT).exe

CABRESULT = $(SIGNDIR)\$(BVR20983_RESULT).cab

!ifdef clean
all: $(PROJECTS) clean
!else
all: $(PROJECTS) 
!endif

distribute: $(PROJECTS) $(SIGNDIR) $(DISTDIR) $(CABRESULT)
  @copy $(HTMLDIR)\led.*                  $(DISTDIR)
  @copy $(HTMLDIR)\*.jpg                  $(DISTDIR)
  @copy $(SIGNDIR)\$(BVR20983_RESULT).cab $(DISTDIR)

patch:
  @$(patch) signpubkeytok $(SIGNPUBKEYTOK) crtlib $(MSVCRTLIB) debugver $(DEBUGVER)

$(CABRESULT): $(CABCONTENT)
	@$(cab) -s 6144 N $@ $**
	@$(sign) $(signvars) /p $(signpwd) $@

!include <./inc/bvr.inc>
