/*
CuteTorrent BitTorrenttClient with dht support, user friendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _VERSION_INFO_H
#define _VERSION_INFO_H

#include "Build_Increment.h"



#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VERSION_MAJOR            1
#define VERSION_MINOR            0
#define VERSION_REVISION         REVISION_INCREMENT
#define VERSION_BUILD            BUILD_INCREMENT

#define VER_FILE_DESCRIPTION_STR "BitTorrent client CuteTorrent"
#define VER_FILE_VERSION         VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR     STRINGIZE(VERSION_MAJOR)     \
                                    "." STRINGIZE(VERSION_MINOR) \
                                    "." STRINGIZE(VERSION_REVISION) \
                                    "." STRINGIZE(VERSION_BUILD)  ".rc1"

#define VER_PRODUCTNAME_STR      "CuteTorrent"
#define VER_PRODUVER_FILE_VERSION_STR      VER_FILE_VERSION
#define VER_PRODUVER_FILE_VERSION_STR_STR  VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR   VER_PRODUCTNAME_STR ".exe"
#define VER_INTERNAL_NAME_STR    VER_ORIGINAL_FILENAME_STR
#define VER_COPYRIGHT_STR        "Copyright (C) 2015"

#ifdef _DEBUG
#define VER_VER_DEBUG          VS_FF_DEBUG
#else
#define VER_VER_DEBUG          0
#endif

#define VER_FILEOS               VOS_NT_WINDOWS32
#define VER_FILEFLAGS            VER_VER_DEBUG
#define VER_FILETYPE             VFT_APP

#endif
