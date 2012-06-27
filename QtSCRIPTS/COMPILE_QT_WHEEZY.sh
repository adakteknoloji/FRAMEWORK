#! /bin/bash


PREFIX=../QtLNX32
if [ "`uname -m`" == "x86_64" ]
then
    PREFIX=../QtLNX64
fi


#sudo apt-get install libx11-xcb-dev libglu1-mesa-dev libxrender-dev libedit-dev flex bison gperf libicu-dev libglib2.0-dev libdbus-1-dev libegl1-mesa-dev libopenvg1-mesa-dev \
#libssl-dev libxv-dev libxrandr-dev x11proto-randr-dev x11proto-video-dev libcups2-dev  libopenvg1-mesa-dev libegl1-mesa-dev libdbus-1-dev libglib2.0-dev \
#libsm-dev  libxcursor-dev libpulse-dev libgstreamer0.10-dev libgstrtspserver-0.10-dev libfontconfig1-dev libxinerama-dev libasound2-dev libgtk2.0-dev \


#
#          vi src/3rdparty/webkit/Source/JavaScriptCore/wtf/gobject/GTypedefs.h
#
#          55.Satir ' da
#          typedef struct _GMutex GMutex --> typedef union  _GMutex GMutex;
#
#



./configure -v -opensource -prefix $PREFIX -release -shared -gtkstyle \
-nomake demos -nomake examples -nomake docs -webkit -cups -fast -glib \
-largefile -exceptions -accessibility -stl -qt-sql-sqlite  -qt3support -phonon -xmlpatterns \
-multimedia -audio-backend -svg -javascript-jit -script -declarative -no-declarative-debug -qt-zlib \
-qt-libtiff -qt-libpng -qt-libmng -qt-libjpeg -optimized-qmake  \
-dbus -egl -opengl -openvg -openssl \
-xrandr -xrender -sm -xshape -xsync -xinerama -xcursor -xfixes



echo "Qt derlenmeye hazir. Lutfen 'make -j 4' ile derleyin"
exit

(libs tools examples demos docs translations
qmake vars .......... cat: .qmake.vars: Böyle bir dosya ya da dizin yok
qmake switches ......... 
Build .................. libs tools translations
Configuration ..........  release shared dll largefile stl precompile_header mmx 3dnow sse sse2 sse3 ssse3 sse4_1 sse4_2 avx  minimal-config small-config medium-config large-config full-config qt3support phonon phonon-backend accessibility egl openvg opengl reduce_exports ipv6 clock-gettime clock-monotonic mremap getaddrinfo ipv6ifname getifaddrs inotify png system-freetype zlib nis cups iconv glib gstreamer dbus openssl x11sm xshape xvideo xsync xinerama xcursor xfixes xrandr xrender mitshm fontconfig xkb alsa pulseaudio icu concurrent xmlpatterns multimedia audio-backend svg script scripttools declarative release
Debug .................. no
Qt 3 compatibility ..... yes
QtDBus module .......... yes (run-time)
QtConcurrent code ...... yes
QtGui module ........... yes
QtScript module ........ yes
QtScriptTools module ... yes
QtXmlPatterns module ... yes
Phonon module .......... yes
Multimedia module ...... yes
SVG module ............. yes
WebKit module .......... yes
JavaScriptCore JIT ..... yes
Declarative module ..... yes
Declarative debugging ...no
Support for S60 ........ no
Symbian DEF files ...... no
STL support ............ yes
PCH support ............ yes
MMX/3DNOW/SSE/SSE2/SSE3. yes/yes/yes/yes/yes
SSSE3/SSE4.1/SSE4.2..... yes/yes/yes
AVX..................... yes
Graphics System ........ default
IPv6 support ........... yes
IPv6 ifname support .... yes
getaddrinfo support .... yes
getifaddrs support ..... yes
Accessibility .......... yes
NIS support ............ yes
CUPS support ........... yes
Iconv support .......... yes
Glib support ........... yes
GStreamer support ...... yes
PulseAudio support ..... yes
Large File support ..... yes
GIF support ............ plugin
TIFF support ........... plugin (qt)
JPEG support ........... plugin (qt)
PNG support ............ yes (qt)
MNG support ............ plugin (qt)
zlib support ........... yes
Session management ..... yes
OpenGL support ......... yes (Desktop OpenGL)
EGL support ............ yes <EGL/egl.h>
OpenVG support ......... yes
NAS sound support ...... no
XShape support ......... yes
XVideo support ......... yes
XSync support .......... yes
Xinerama support ....... yes
Xcursor support ........ yes
Xfixes support ......... yes
Xrandr support ......... yes
Xrender support ........ yes
Xi support ............. runtime
MIT-SHM support ........ yes
FontConfig support ..... yes
XKB Support ............ yes
immodule support ....... yes
GTK theme support ...... yes
SQLite support ......... qt (qt)
OpenSSL support ........ yes (run-time)
Alsa support ........... yes
ICD support ............ no
libICU support ......... yes


Usage:  configure [-h] [-prefix <dir>] [-prefix-install] [-bindir <dir>] [-libdir <dir>]
        [-docdir <dir>] [-headerdir <dir>] [-plugindir <dir> ] [-importdir <dir>] [-datadir <dir>]
        [-translationdir <dir>] [-sysconfdir <dir>] [-examplesdir <dir>]
        [-demosdir <dir>] [-buildkey <key>] [-release] [-debug]
        [-debug-and-release] [-developer-build] [-shared] [-static] [-no-fast] [-fast] [-no-largefile]
        [-largefile] [-no-exceptions] [-exceptions] [-no-accessibility]
        [-accessibility] [-no-stl] [-stl] [-no-sql-<driver>] [-sql-<driver>]
        [-plugin-sql-<driver>] [-system-sqlite] [-no-qt3support] [-qt3support]
        [-platform] [-D <string>] [-I <string>] [-L <string>] [-help]
        [-qt-zlib] [-system-zlib] [-no-gif] [-no-libtiff] [-qt-libtiff] [-system-libtiff]
        [-no-libpng] [-qt-libpng] [-system-libpng] [-no-libmng] [-qt-libmng]
        [-system-libmng] [-no-libjpeg] [-qt-libjpeg] [-system-libjpeg] [-make <part>]
        [-nomake <part>] [-R <string>]  [-l <string>] [-no-rpath]  [-rpath] [-continue]
        [-verbose] [-v] [-silent] [-no-nis] [-nis] [-no-cups] [-cups] [-no-iconv]
        [-iconv] [-no-pch] [-pch] [-no-dbus] [-dbus] [-dbus-linked] [-no-gui]
        [-no-separate-debug-info] [-no-mmx] [-no-3dnow] [-no-sse] [-no-sse2]
        [-no-sse3] [-no-ssse3] [-no-sse4.1] [-no-sse4.2] [-no-avx] [-no-neon]
        [-qtnamespace <namespace>] [-qtlibinfix <infix>] [-separate-debug-info] [-armfpa]
        [-no-optimized-qmake] [-optimized-qmake] [-no-xmlpatterns] [-xmlpatterns]
        [-no-multimedia] [-multimedia] [-no-phonon] [-phonon] [-no-phonon-backend] [-phonon-backend]
        [-no-media-backend] [-media-backend] [-no-audio-backend] [-audio-backend] 
        [-no-openssl] [-openssl] [-openssl-linked]
        [-no-gtkstyle] [-gtkstyle] [-no-svg] [-svg] [-no-webkit] [-webkit] [-webkit-debug]
        [-no-javascript-jit] [-javascript-jit]
        [-no-script] [-script] [-no-scripttools] [-scripttools] 
        [-no-declarative] [-declarative] [-no-declarative-debug] [-declarative-debug]
        [additional platform specific options (see below)]


Installation options:

    -qpa ................ This will enable the QPA build.
                          QPA is a window system agnostic implementation of Qt.

 These are optional, but you may specify install directories.

    -prefix <dir> ...... This will install everything relative to <dir>
                         (default /usr/local/Trolltech/Qt-4.8.2)

  * -prefix-install .... Force a sandboxed "local" installation of
                         Qt. This will install into
                         /usr/local/Trolltech/Qt-4.8.2, if this option is
                         disabled then some platforms will attempt a
                         "system" install by placing default values to
                         be placed in a system location other than
                         PREFIX.

 You may use these to separate different parts of the install:

    -bindir <dir> ......... Executables will be installed to <dir>
                            (default PREFIX/bin)
    -libdir <dir> ......... Libraries will be installed to <dir>
                            (default PREFIX/lib)
    -docdir <dir> ......... Documentation will be installed to <dir>
                            (default PREFIX/doc)
    -headerdir <dir> ...... Headers will be installed to <dir>
                            (default PREFIX/include)
    -plugindir <dir> ...... Plugins will be installed to <dir>
                            (default PREFIX/plugins)
    -importdir <dir> ...... Imports for QML will be installed to <dir>
                            (default PREFIX/imports)
    -datadir <dir> ........ Data used by Qt programs will be installed to <dir>
                            (default PREFIX)
    -translationdir <dir> . Translations of Qt programs will be installed to <dir>
                            (default PREFIX/translations)
    -sysconfdir <dir> ..... Settings used by Qt programs will be looked for in <dir>
                            (default PREFIX/etc/settings)
    -examplesdir <dir> .... Examples will be installed to <dir>
                            (default PREFIX/examples)
    -demosdir <dir> ....... Demos will be installed to <dir>
                            (default PREFIX/demos)

 You may use these options to turn on strict plugin loading.

    -buildkey <key> .... Build the Qt library and plugins using the specified
                         <key>.  When the library loads plugins, it will only
                         load those that have a matching key.

Configure options:

 The defaults (*) are usually acceptable. A plus (+) denotes a default value
 that needs to be evaluated. If the evaluation succeeds, the feature is
 included. Here is a short explanation of each option:

 *  -release ........... Compile and link Qt with debugging turned off.
    -debug ............. Compile and link Qt with debugging turned on.
    -debug-and-release . Compile and link two versions of Qt, with and without
                         debugging turned on (Mac only).

    -developer-build ... Compile and link Qt with Qt developer options (including auto-tests exporting)

    -opensource ........ Compile and link the Open-Source Edition of Qt.
    -commercial ........ Compile and link the Commercial Edition of Qt.


 *  -shared ............ Create and use shared Qt libraries.
    -static ............ Create and use static Qt libraries.

 *  -no-fast ........... Configure Qt normally by generating Makefiles for all
                         project files.
    -fast .............. Configure Qt quickly by generating Makefiles only for
                         library and subdirectory targets.  All other Makefiles
                         are created as wrappers, which will in turn run qmake.

    -no-largefile ...... Disables large file support.
 +  -largefile ......... Enables Qt to access files larger than 4 GB.

    -no-exceptions ..... Disable exceptions on compilers that support it.
 *  -exceptions ........ Enable exceptions on compilers that support it.

    -no-accessibility .. Do not compile Accessibility support.
 *  -accessibility ..... Compile Accessibility support.

    -no-stl ............ Do not compile STL support.
 *  -stl ............... Compile STL support.

    -no-sql-<driver> ... Disable SQL <driver> entirely.
    -qt-sql-<driver> ... Enable a SQL <driver> in the QtSql library, by default
                         none are turned on.
    -plugin-sql-<driver> Enable SQL <driver> as a plugin to be linked to
                         at run time.

                         Possible values for <driver>:
                         [  db2 ibase mysql oci odbc psql sqlite sqlite2 sqlite_symbian symsql tds ]

    -system-sqlite ..... Use sqlite from the operating system.

    -no-qt3support ..... Disables the Qt 3 support functionality.
 *  -qt3support ........ Enables the Qt 3 support functionality.

    -no-xmlpatterns .... Do not build the QtXmlPatterns module.
 +  -xmlpatterns ....... Build the QtXmlPatterns module.
                         QtXmlPatterns is built if a decent C++ compiler
                         is used and exceptions are enabled.

    -no-multimedia ..... Do not build the QtMultimedia module.
 +  -multimedia ........ Build the QtMultimedia module.

    -no-audio-backend .. Do not build the platform audio backend into QtMultimedia.
 +  -audio-backend ..... Build the platform audio backend into QtMultimedia if available.

    -no-phonon ......... Do not build the Phonon module.
 +  -phonon ............ Build the Phonon module.
                         Phonon is built if a decent C++ compiler is used.
    -no-phonon-backend.. Do not build the platform phonon plugin.
 +  -phonon-backend..... Build the platform phonon plugin.

    -no-svg ............ Do not build the SVG module.
 +  -svg ............... Build the SVG module.

    -no-webkit ......... Do not build the WebKit module.
 +  -webkit ............ Build the WebKit module.
                         WebKit is built if a decent C++ compiler is used.
    -webkit-debug ...... Build the WebKit module with debug symbols.

    -no-javascript-jit . Do not build the JavaScriptCore JIT compiler.
 +  -javascript-jit .... Build the JavaScriptCore JIT compiler.

    -no-script ......... Do not build the QtScript module.
 +  -script ............ Build the QtScript module.

    -no-scripttools .... Do not build the QtScriptTools module.
 +  -scripttools ....... Build the QtScriptTools module.

    -no-declarative ..... Do not build the declarative module.
 +  -declarative ....... Build the declarative module.

    -no-declarative-debug ..... Do not build the declarative debugging support.
 +  -declarative-debug ....... Build the declarative debugging support.

    -platform target ... The operating system and compiler you are building
                         on (linux-g++-64).

                         See the README file for a list of supported
                         operating systems and compilers.
    -graphicssystem <sys> Sets an alternate graphics system. Available options are:
                           raster - Software rasterizer
                           opengl - Rendering via OpenGL, Experimental!
                           openvg - Rendering via OpenVG, Experimental!


    -no-mmx ............ Do not compile with use of MMX instructions.
    -no-3dnow .......... Do not compile with use of 3DNOW instructions.
    -no-sse ............ Do not compile with use of SSE instructions.
    -no-sse2 ........... Do not compile with use of SSE2 instructions.
    -no-sse3 ........... Do not compile with use of SSE3 instructions.
    -no-ssse3 .......... Do not compile with use of SSSE3 instructions.
    -no-sse4.1.......... Do not compile with use of SSE4.1 instructions.
    -no-sse4.2.......... Do not compile with use of SSE4.2 instructions.
    -no-avx ............ Do not compile with use of AVX instructions.
    -no-neon ........... Do not compile with use of NEON instructions.

    -qtnamespace <name>  Wraps all Qt library code in 'namespace <name> {...}'.
    -qtlibinfix <infix>  Renames all libQt*.so to libQt*<infix>.so.

    -D <string> ........ Add an explicit define to the preprocessor.
    -I <string> ........ Add an explicit include path.
    -L <string> ........ Add an explicit library path.

    -help, -h .......... Display this information.

Third Party Libraries:

    -qt-zlib ........... Use the zlib bundled with Qt.
 +  -system-zlib ....... Use zlib from the operating system.
                         See http://www.gzip.org/zlib

    -no-gif ............ Do not compile GIF reading support.

    -no-libtiff ........ Do not compile TIFF support.
    -qt-libtiff ........ Use the libtiff bundled with Qt.
 +  -system-libtiff .... Use libtiff from the operating system.
                         See http://www.libtiff.org

    -no-libpng ......... Do not compile PNG support.
    -qt-libpng ......... Use the libpng bundled with Qt.
 +  -system-libpng ..... Use libpng from the operating system.
                         See http://www.libpng.org/pub/png

    -no-libmng ......... Do not compile MNG support.
    -qt-libmng ......... Use the libmng bundled with Qt.
 +  -system-libmng ..... Use libmng from the operating system.
                         See http://www.libmng.com

    -no-libjpeg ........ Do not compile JPEG support.
    -qt-libjpeg ........ Use the libjpeg bundled with Qt.
 +  -system-libjpeg .... Use libjpeg from the operating system.
                         See http://www.ijg.org

    -no-openssl ........ Do not compile support for OpenSSL.
 +  -openssl ........... Enable run-time OpenSSL support.
    -openssl-linked .... Enabled linked OpenSSL support.

    -ptmalloc .......... Override the system memory allocator with ptmalloc.
                         (Experimental.)

Additional options:

    -make <part> ....... Add part to the list of parts to be built at make time.
                         (libs tools examples demos docs translations)
    -nomake <part> ..... Exclude part from the list of parts to be built.

    -R <string> ........ Add an explicit runtime library path to the Qt
                         libraries.
    -l <string> ........ Add an explicit library.

    -no-rpath .......... Do not use the library install path as a runtime
                         library path.
 +  -rpath ............. Link Qt libraries and executables using the library
                         install path as a runtime library path. Equivalent
                         to -R install_libpath

    -continue .......... Continue as far as possible if an error occurs.

    -verbose, -v ....... Print verbose information about each step of the
                         configure process.

    -silent ............ Reduce the build output so that warnings and errors
                         can be seen more easily.

 *  -no-optimized-qmake ... Do not build qmake optimized.
    -optimized-qmake ...... Build qmake optimized.

    -no-gui ............ Don't build the Qt GUI library

    -no-nis ............ Do not compile NIS support.
 *  -nis ............... Compile NIS support.

    -no-cups ........... Do not compile CUPS support.
 *  -cups .............. Compile CUPS support.
                         Requires cups/cups.h and libcups.so.2.

    -no-iconv .......... Do not compile support for iconv(3).
 *  -iconv ............. Compile support for iconv(3).

    -no-pch ............ Do not use precompiled header support.
 *  -pch ............... Use precompiled header support.

    -no-dbus ........... Do not compile the QtDBus module.
 +  -dbus .............. Compile the QtDBus module and dynamically load libdbus-1.
    -dbus-linked ....... Compile the QtDBus module and link to libdbus-1.

    -reduce-relocations ..... Reduce relocations in the libraries through extra
                              linker optimizations (Qt/X11 and Qt for Embedded Linux only;
                              experimental; needs GNU ld >= 2.18).

 *  -no-separate-debug-info . Do not store debug information in a separate file.
    -separate-debug-info .... Strip debug information into a separate .debug file.


Qt/X11 only:

    -no-gtkstyle ....... Do not build the GTK theme integration.
 +  -gtkstyle .......... Build the GTK theme integration.

 *  -no-nas-sound ...... Do not compile in NAS sound support.
    -system-nas-sound .. Use NAS libaudio from the operating system.
                         See http://radscan.com/nas.html

    -egl ............... Use EGL instead of GLX to manage contexts.
                         When building for desktop OpenGL, this option will
                         make Qt use EGL to manage contexts rather than the
                         GLX, which is the default. Note: For OpenGL ES, EGL
                         is always used.

    -no-opengl ......... Do not support OpenGL.
 +  -opengl <api> ...... Enable OpenGL support.
                         With no parameter, this will auto-detect the "best"
                         OpenGL API to use. If desktop OpenGL is available, it
                         will be used. Use desktop, es1, or es2 for <api>
                         to force the use of the Desktop (OpenGL 1.x or 2.x),
                         OpenGL ES 1.x Common profile, or 2.x APIs instead.

     -no-openvg ........ Do not support OpenVG.
 +   -openvg ........... Enable OpenVG support.
                         Requires EGL support, typically supplied by an OpenGL
                         or other graphics implementation.

    -no-sm ............. Do not support X Session Management.
 *  -sm ................ Support X Session Management, links in -lSM -lICE.

    -no-xshape ......... Do not compile XShape support.
 *  -xshape ............ Compile XShape support.
                         Requires X11/extensions/shape.h.

    -no-xvideo ......... Do not compile XVideo support.
 *  -xvideo ............ Compile XVideo support.
                         Requires X11/extensions/Xv.h & Xvlib.h.

    -no-xsync .......... Do not compile XSync support.
 *  -xsync ............. Compile XSync support.
                         Requires X11/extensions/sync.h.

    -no-xinerama ....... Do not compile Xinerama (multihead) support.
 *  -xinerama .......... Compile Xinerama support.
                         Requires X11/extensions/Xinerama.h and libXinerama.
			 By default, Xinerama support will be compiled if
                         available and the shared libraries are dynamically
                         loaded at runtime.

    -no-xcursor ........ Do not compile Xcursor support.
 *  -xcursor ........... Compile Xcursor support.
                         Requires X11/Xcursor/Xcursor.h and libXcursor.
			 By default, Xcursor support will be compiled if
                         available and the shared libraries are dynamically
                         loaded at runtime.

    -no-xfixes ......... Do not compile Xfixes support.
 *  -xfixes ............ Compile Xfixes support.
                         Requires X11/extensions/Xfixes.h and libXfixes.
			 By default, Xfixes support will be compiled if
                         available and the shared libraries are dynamically
                         loaded at runtime.

    -no-xrandr ......... Do not compile Xrandr (resize and rotate) support.
 *  -xrandr ............ Compile Xrandr support.
                         Requires X11/extensions/Xrandr.h and libXrandr.

    -no-xrender ........ Do not compile Xrender support.
 *  -xrender ........... Compile Xrender support.
                         Requires X11/extensions/Xrender.h and libXrender.

    -no-mitshm ......... Do not compile MIT-SHM support.
 *  -mitshm ............ Compile MIT-SHM support.
                         Requires sys/ipc.h, sys/shm.h and X11/extensions/XShm.h

    -no-fontconfig ..... Do not compile FontConfig (anti-aliased font) support.
 *  -fontconfig ........ Compile FontConfig support.
                         Requires fontconfig/fontconfig.h, libfontconfig,
                         freetype.h and libfreetype.

    -no-xinput ......... Do not compile Xinput support.
 *  -xinput ............ Compile Xinput support. This also enabled tablet support
                         which requires IRIX with wacom.h and libXi or
                         XFree86 with X11/extensions/XInput.h and libXi.

    -no-xkb ............ Do not compile XKB (X KeyBoard extension) support.
 *  -xkb ............... Compile XKB support.

    -no-glib ........... Do not compile Glib support.
 +  -glib .............. Compile Glib support.

