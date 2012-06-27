#!/bin/bash
export DYLD_FRAMEWORK_PATH="/Applications/CEREN/K9.app/:/Applications/CEREN/K9.app/lib/":$DYLD_FRAMEWORK_PATH
export DYLD_LIBRARY_PATH="/Applications/CEREN/K9.app/:/Applications/CEREN/K9.app/lib/":$DYLD_LIBRARY_PATH
cd /Applications/CEREN/
mv -f K9.app/lib/CEREN K9.app/
./K9.app/CEREN&
