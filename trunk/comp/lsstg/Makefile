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

!include <../../inc/Makefile.inc>

OBJS= \
$(OUTDIR)\os.obj \
$(OUTDIR)\lsstp.obj \
$(OUTDIR)\lsstg.res \
$(SLNDIR)\libbvr20983.lib

!ifdef clean
all: clean
!else
all: $(OUTDIR) $(RESULTDIR) $(SLNDIR) $(SLNDIR)\$(LSSTG_RESULT).exe
!endif

$(OUTDIR)\$(LSSTG_RESULT).exe: $(OBJS)
  $(link) $(ldebug) $(conlflags) $(lvars) -out:$@ -pdb:$(@R).pdb $(olelibsmt) $(OBJS)

$(SLNDIR)\$(LSSTG_RESULT).exe: $(OUTDIR)\$(LSSTG_RESULT).exe
  @copy $(OUTDIR)\$(LSSTG_RESULT).exe $(SLNDIR)

clean:
  if exist $(OUTDIR)\$(NULL) rd /s /q $(OUTDIR)
  if exist $(SLNDIR)\$(LSSTG_RESULT).exe del /q $(SLNDIR)\$(LSSTG_RESULT).exe

!include <../../inc/Makefile.rule>
