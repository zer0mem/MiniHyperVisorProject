
SOURCE="$(REL_PATH)\$(SRC).$(SRC_EXT)"
CPP_SWITCHES=$(CPP_PROJ) /Fp"$(INTDIR)\$(DriverName).pch" /Yu"StdAfx.h"

"$(INTDIR)\$(SRC).obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\$(DriverName).pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<

