@echo off

configure -opensource -release -shared -fast -no-qt3support -multimedia -audio-backend -openssl -L C:\OpenSSL-Win32\lib -I C:\OpenSSL-Win32\include -script -declarative -no-declarative-debug -qt-sql-sqlite -qt-zlib -qt-libtiff -qt-libpng -qt-libmng -qt-libjpeg -nomake demos -nomake examples -nomake docs

echo "Qt derlenmeye hazir. Lutfen yeniden girip `nmake` yazin"
echo Press any key to exit this shell
pause
exit 0


This is the Qt for Windows Open Source Edition.

You are licensed to use this software under the terms of
the GNU Lesser General Public License (LGPL) version 2.1.

Type 'L' to view the Lesser GNU General Public License version 2.1 (LGPLv2.1).
Type 'y' to accept this license offer.
Type 'n' to decline this license offer.

Do you accept the terms of the license?
yes
Environment:
    INCLUDE=
      c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\INCLUDE
      C:\Program Files\Microsoft SDKs\Windows\v6.0A\include

    LIB=
      c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\LIB
      C:\Program Files\Microsoft SDKs\Windows\v6.0A\lib

    PATH=
      c:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE
      c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\BIN
      c:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools
      c:\Windows\Microsoft.NET\Framework\v3.5
      c:\Windows\Microsoft.NET\Framework\v2.0.50727
      c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\VCPackages
      C:\Program Files\Microsoft SDKs\Windows\v6.0A\bin
      C:\Windows\system32
      C:\Windows
      C:\Windows\System32\Wbem
      C:\Windows\System32\WindowsPowerShell\v1.0\
      c:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\
      c:\Program Files (x86)\Microsoft SQL Server\100\DTS\Binn\
      C:\Program Files (x86)\Git\cmd
      C:\strawberry\c\bin
      C:\strawberry\perl\site\bin
      C:\strawberry\perl\bin
      Z:\qt_win32\bin
You are licensed to use this software under the terms of the GNU GPL version 3.You are licensed to use this software under the terms of the Lesser GNU
ersion 2.1.
See C:/Qt3

 or C:/QtL

Configuration:
    minimal-config
    small-config
    medium-config
    large-config
    full-config
    release
Qt Configuration:
    release
    zlib
    png
    accessibility
    qt3support
    opengl
    s60
    ipv6
    script
    scripttools
    xmlpatterns
    phonon
    phonon-backend
    multimedia
    audio-backend
    declarative
    native-gestures
    svg
    concurrent
    minimal-config
    small-config
    medium-config
    large-config
    full-config

QMAKESPEC...................win32-msvc2008 (detected)
Architecture................windows
Maketool....................nmake
Debug symbols...............no
Link Time Code Generation...no
Accessibility support.......yes
STL support.................yes
Exception support...........yes
RTTI support................yes
MMX support.................yes
3DNOW support...............yes
SSE support.................yes
SSE2 support................yes
IWMMXT support..............no
OpenGL support..............yes
OpenVG support..............no
OpenSSL support.............no
QtDBus support..............no
QtXmlPatterns support.......yes
Phonon support..............yes
QtMultimedia support........yes
WebKit support..............yes
Declarative support.........yes
Declarative debugging.......no
QtScript support............yes
QtScriptTools support.......yes
Graphics System.............raster
Qt3 compatibility...........yes
DirectWrite support.........no

Third Party Libraries:
    ZLIB support............qt
    GIF support.............plugin
    TIFF support............plugin
    JPEG support............plugin
    PNG support.............yes
    MNG support.............plugin
    FreeType support........no

Styles:
    Windows.................yes
    Windows XP..............yes
    Windows Vista...........yes
    Plastique...............yes
    Cleanlooks..............yes
    Motif...................yes
    CDE.....................yes
    Windows CE..............no
    Windows Mobile..........no
    S60.....................no

Sql Drivers:
    ODBC....................no
    MySQL...................no
    OCI.....................no
    PostgreSQL..............no
    TDS.....................no
    DB2.....................no
    SQLite..................yes (qt)
    SQLite2.................no
    InterBase...............no

Sources are in..............C:\Qt
Build is done in............C:\Qt
Install prefix..............C:\Qt
Headers installed to........C:\Qt\include
Libraries installed to......C:\Qt\lib
Plugins installed to........C:\Qt\plugins
Imports installed to........C:\Qt\imports
Binaries installed to.......C:\Qt\bin
Docs installed to...........C:\Qt\doc
Data installed to...........C:\Qt
Translations installed to...C:\Qt\translations
Examples installed to.......C:\Qt\examples
Demos installed to..........C:\Qt\demos


C:\qt>configure -help
Usage: configure [-buildkey <key>]
       [-release] [-debug] [-debug-and-release] [-shared] [-static]
       [-no-fast] [-fast] [-no-exceptions] [-exceptions]
       [-no-accessibility] [-accessibility] [-no-rtti] [-rtti]
       [-no-stl] [-stl] [-no-sql-<driver>] [-qt-sql-<driver>]
       [-plugin-sql-<driver>] [-system-sqlite] [-arch <arch>]
       [-D <define>] [-I <includepath>] [-L <librarypath>]
       [-help] [-no-dsp] [-dsp] [-no-vcproj] [-vcproj]
       [-no-qmake] [-qmake] [-dont-process] [-process]
       [-no-style-<style>] [-qt-style-<style>] [-redo]
       [-saveconfig <config>] [-loadconfig <config>]
       [-qt-zlib] [-system-zlib] [-no-gif] [-no-libpng]
       [-qt-libpng] [-system-libpng] [-no-libtiff] [-qt-libtiff]
       [-system-libtiff] [-no-libjpeg] [-qt-libjpeg] [-system-libjpeg]
       [-no-libmng] [-qt-libmng] [-system-libmng] [-no-qt3support] [-mmx]
       [-no-mmx] [-3dnow] [-no-3dnow] [-sse] [-no-sse] [-sse2] [-no-sse2]
       [-no-iwmmxt] [-iwmmxt] [-openssl] [-openssl-linked]
       [-no-openssl] [-no-dbus] [-dbus] [-dbus-linked] [-platform <spec>]
       [-qtnamespace <namespace>] [-qtlibinfix <infix>] [-no-phonon]
       [-phonon] [-no-phonon-backend] [-phonon-backend]
       [-no-multimedia] [-multimedia] [-no-audio-backend] [-audio-backend]
       [-no-script] [-script] [-no-scripttools] [-scripttools]
       [-no-webkit] [-webkit] [-webkit-debug]
       [-graphicssystem raster|opengl|openvg]
       [-no-directwrite] [-directwrite]

Installation options:

 You may use these options to turn on strict plugin loading:

    -buildkey <key> .... Build the Qt library and plugins using the specified
                         <key>.  When the library loads plugins, it will only
                         load those that have a matching <key>.

Configure options:

 The defaults (*) are usually acceptable. A plus (+) denotes a default value
 that needs to be evaluated. If the evaluation succeeds, the feature is
 included. Here is a short explanation of each option:

    -release ........... Compile and link Qt with debugging turned off.
 *  -debug ............. Compile and link Qt with debugging turned on.
 +  -debug-and-release . Compile and link two Qt libraries, with and without
                         debugging turned on.

    -opensource ........ Compile and link the Open-Source Edition of Qt.
    -commercial ........ Compile and link the Commercial Edition of Qt.

    -developer-build ... Compile and link Qt with Qt developer options
                         (including auto-tests exporting)

 *  -shared ............ Create and use shared Qt libraries.
    -static ............ Create and use static Qt libraries.

    -ltcg .............. Use Link Time Code Generation. (Release builds only)
 *  -no-ltcg ........... Do not use Link Time Code Generation.

 *  -no-fast ........... Configure Qt normally by generating Makefiles for all
                         project files.
    -fast .............. Configure Qt quickly by generating Makefiles only for
                         library and subdirectory targets.  All other Makefiles
                         are created as wrappers which will in turn run qmake

    -no-exceptions ..... Disable exceptions on platforms that support it.
 *  -exceptions ........ Enable exceptions on platforms that support it.

    -no-accessibility .. Do not compile Windows Active Accessibility support.
 *  -accessibility ..... Compile Windows Active Accessibility support.

    -no-stl ............ Do not compile STL support.
 *  -stl ............... Compile STL support.

    -no-sql-<driver> ... Disable SQL <driver> entirely, by default none are
                         turned on.
    -qt-sql-<driver> ... Enable a SQL <driver> in the Qt Library.
    -plugin-sql-<driver> Enable SQL <driver> as a plugin to be linked to at run
                         time.
                         Available values for <driver>:
                           mysql
                           psql
                           oci
                           odbc
                           tds
                           db2
 +                         sqlite
                           sqlite2
                           ibase
                         (drivers marked with a '+' have been detected as
                         available on this system)

    -system-sqlite ..... Use sqlite from the operating system.

    -no-qt3support ..... Disables the Qt 3 support functionality.

    -no-opengl ......... Disables OpenGL functionality

    -opengl <api> ...... Enable OpenGL support with specified API version.
                         Available values for <api>:
 *                         desktop - Enable support for Desktop OpenGL
                           es1 - Enable support for OpenGL ES Common Profile
                           es2 - Enable support for OpenGL ES 2.0
 *  -no-openvg ......... Disables OpenVG functionality

    -openvg ............ Enables OpenVG functionality
                         Requires EGL support, typically supplied by an OpenGL
                         or other graphics implementation

    -platform <spec> ... The operating system and compiler you are building on.
                         (default %QMAKESPEC%)

    -xplatform <spec> .. The operating system and compiler you are cross
                         compiling to.

                         See the README file for a list of supported operating
                         systems and compilers.

    -qtnamespace <namespace> Wraps all Qt library code in 'namespace name {...}
    -qtlibinfix <infix> Renames all Qt* libs to Qt*<infix>

    -D <define> ........ Add an explicit define to the preprocessor.
    -I <includepath> ... Add an explicit include path.
    -L <librarypath> ... Add an explicit library path.
    -l <libraryname> ... Add an explicit library name, residing in a
                         librarypath.

    -graphicssystem <sys> Specify which graphicssystem should be used.
                          Available values for <sys>:
 *                         raster - Software rasterizer
                           opengl - Using OpenGL acceleration, experimental!
                           openvg - Using OpenVG acceleration, experimental!

    -help, -h, -? ...... Display this information.

Third Party Libraries:

    -qt-zlib ........... Use the zlib bundled with Qt.
 +  -system-zlib ....... Use zlib from the operating system.
                         See http://www.gzip.org/zlib

    -no-gif ............ Do not compile GIF reading support.
    -no-libpng ......... Do not compile PNG support.
    -qt-libpng ......... Use the libpng bundled with Qt.
 +  -system-libpng ..... Use libpng from the operating system.
                         See http://www.libpng.org/pub/png

    -no-libmng ......... Do not compile MNG support.
    -qt-libmng ......... Use the libmng bundled with Qt.
 +  -system-libmng ..... Use libmng from the operating system.
                         See See http://www.libmng.com

    -no-libtiff ........ Do not compile TIFF support.
    -qt-libtiff ........ Use the libtiff bundled with Qt.
 +  -system-libtiff .... Use libtiff from the operating system.
                         See http://www.libtiff.org

    -no-libjpeg ........ Do not compile JPEG support.
    -qt-libjpeg ........ Use the libjpeg bundled with Qt.
 +  -system-libjpeg .... Use libjpeg from the operating system.
                         See http://www.ijg.org

Qt for Windows only:

    -no-dsp ............ Do not generate VC++ .dsp files.
 *  -dsp ............... Generate VC++ .dsp files, only if spec "win32-msvc".

    -no-vcproj ......... Do not generate VC++ .vcproj files.
 *  -vcproj ............ Generate VC++ .vcproj files, only if platform
                         "win32-msvc.net".

    -no-incredibuild-xge Do not add IncrediBuild XGE distribution commands to
                         custom build steps.
 +  -incredibuild-xge .. Add IncrediBuild XGE distribution commands to custom
                         build steps. This will distribute MOC and UIC steps,
                         and other custom buildsteps which are added to the
                         INCREDIBUILD_XGE variable.
                         (The IncrediBuild distribution commands are only added
                         to Visual Studio projects)

    -no-plugin-manifests Do not embed manifests in plugins.
 *  -plugin-manifests .. Embed manifests in plugins.

    -no-qmake .......... Do not compile qmake.
 *  -qmake ............. Compile qmake.

    -dont-process ...... Do not generate Makefiles/Project files. This will
                         override -no-fast if specified.
 *  -process ........... Generate Makefiles/Project files.

    -no-rtti ........... Do not compile runtime type information.
 *  -rtti .............. Compile runtime type information.

    -no-mmx ............ Do not compile with use of MMX instructions
 +  -mmx ............... Compile with use of MMX instructions
    -no-3dnow .......... Do not compile with use of 3DNOW instructions
 +  -3dnow ............. Compile with use of 3DNOW instructions
    -no-sse ............ Do not compile with use of SSE instructions
 +  -sse ............... Compile with use of SSE instructions
    -no-sse2 ........... Do not compile with use of SSE2 instructions
 +  -sse2 .............. Compile with use of SSE2 instructions
    -no-openssl ........ Do not compile in OpenSSL support
 +  -openssl ........... Compile in run-time OpenSSL support
    -openssl-linked .... Compile in linked OpenSSL support
    -no-dbus ........... Do not compile in D-Bus support
 +  -dbus .............. Compile in D-Bus support and load libdbus-1 dynamicall
                         y
    -dbus-linked ....... Compile in D-Bus support and link to libdbus-1
    -no-phonon ......... Do not compile in the Phonon module
 +  -phonon ............ Compile the Phonon module (Phonon is built if a decent
                         C++ compiler is used.)
    -no-phonon-backend . Do not compile the platform-specific Phonon backend-pl
                         ugin
 *  -phonon-backend .... Compile in the platform-specific Phonon backend-plugin
    -no-multimedia ..... Do not compile the multimedia module
 *  -multimedia ........ Compile in multimedia module
    -no-audio-backend .. Do not compile in the platform audio backend into QtMu
                         ltimedia
 +  -audio-backend ..... Compile in the platform audio backend into QtMultimedi
                         a
    -no-webkit ......... Do not compile in the WebKit module
 +  -webkit ............ Compile in the WebKit module (WebKit is built if a
                         decent C++ compiler is used.)
    -webkit-debug ...... Compile in the WebKit module with debug symbols.
    -no-script ......... Do not build the QtScript module.
 +  -script ............ Build the QtScript module.
    -no-scripttools .... Do not build the QtScriptTools module.
 +  -scripttools ....... Build the QtScriptTools module.
    -no-declarative .... Do not build the declarative module
 +  -declarative ....... Build the declarative module
    -no-declarative-debug Do not build the declarative debugging support
 *  -declarative-debug . Build the declarative debugging support
 *  -no-directwrite .... Do not build support for DirectWrite font rendering
    -directwrite ....... Build support for DirectWrite font rendering
                         (experimental, requires DirectWrite availability on
                         target systems, e.g. Windows Vista with Platform
                         Update, Windows 7, etc.)
    -arch <arch> ....... Specify an architecture.
                         Available values for <arch>:
 *                         windows
                           windowsce
                           symbian
                           boundschecker
                           generic

    -no-style-<style> .. Disable <style> entirely.
    -qt-style-<style> .. Enable <style> in the Qt Library.
                         Available styles:
 *                         windows
 +                         windowsxp
 +                         windowsvista
 *                         plastique
 *                         cleanlooks
 *                         motif
 *                         cde
                           windowsce
                           windowsmobile
                           s60

    -no-native-gestures Do not use native gestures on Windows 7.
 *  -native-gestures ... Use native gestures on Windows 7.
 *  -no-mp ............. Do not use multiple processors for compiling with MSVC
    -mp ................ Use multiple processors for compiling with MSVC (-MP)
    -loadconfig <config> Run configure with the parameters from file configure_
                         <config>.cache.
    -saveconfig <config> Run configure and save the parameters in file
                         configure_<config>.cache.
    -redo .............. Run configure with the same parameters as last time.

Qt for Windows CE only:

    -no-iwmmxt ......... Do not compile with use of IWMMXT instructions
 +  -iwmmxt ............ Do compile with use of IWMMXT instructions (Qt for
                         Windows CE on Arm only)
 *  -no-crt ............ Do not add the C runtime to default deployment rules
    -qt-crt ............ Qt identifies C runtime during project generation
    -crt <path> ........ Specify path to C runtime used for project generation.
    -no-cetest ......... Do not compile Windows CE remote test application
 +  -cetest ............ Compile Windows CE remote test application
    -signature <file> .. Use file for signing the target project
 *  -phonon-wince-ds9 .. Enable Phonon Direct Show 9 backend for Windows CE
Qt for Symbian OS only:

 *  -no-freetype ....... Do not compile in Freetype2 support.
    -qt-freetype ....... Use the libfreetype bundled with Qt.
    -fpu <flags> ....... VFP type on ARM, supported options: softvfp(default) |
                         vfpv2 | softvfp+vfpv2
    -no-s60 ............ Do not compile in S60 support.
 *  -s60 ............... Compile with support for the S60 UI Framework
    -no-usedeffiles .... Disable the usage of DEF files.
    -usedeffiles ....... Enable the usage of DEF files.
