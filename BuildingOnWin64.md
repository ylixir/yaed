# Introduction #

This page contains step by step instructions for getting yaed built on Windows 7 64 bit. If someone is interested in putting together a Win32 guide, [let me know](http://code.google.com/u/ylixir@gmail.com/)

# Compiler Setup #

First we will need a compiler (duh).

  1. We will be using the Mingw-w64 compiler. [You can download it here.](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/sezero_4.5_20111101/mingw-w64-bin_x86_64-mingw_20111101_sezero.zip/download)
    * Make sure you get a native 64 bit compiler to avoid headaches later. Avoid the 32 bit compilers that compile 64 bit code.
    * Extract the contents of the mingw-w64 to a folder. We will use `c:\mingw64` for the purposes of this guide, but you can put it anywhere.
  1. We also need MSYS. [You can get it here.](http://sourceforge.net/projects/mingw-w64/files/External%20binary%20packages%20%28Win64%20hosted%29/MSYS%20%2832-bit%29/)
    * Extract and merge the contents of MSYS with your mingw-w64 folder.
    * Don't overwrite anything when you merge.
    * I had trouble unzipping this with the built in zip handler in windows. If you have trouble try using 7zip or something similar.
  1. Setup your environment variables.
    * Start->right click "Computer"->Advanced system settings->Advanced->Environment Variables...
    * If you don't already have a PATH user variable, make one by clicking `New...`, otherwise `Edit...`
    * The path should look something like this: `%PATH%;c:\mingw64\bin`

# Getting gtk3 #

As of now there don't appear to be any prebuilt gtk3 binaries available so we will have to build them ourselves.

  1. Get the sources to build gtk. [They are here.](http://ftp.gnome.org/pub/gnome/sources/)
  1. Note that odd numbers are unstable and even are stable. So get gtk 3.2 rather than 3.3, to avoid problems.
  1. The following is an attempt at a dependency graph. You will need all the following source packages.
    * gtksourceview [Available here.](http://ftp.acc.umu.se/pub/gnome/sources/gtksourceview/)
      * libxml2 [Available here.](ftp://xmlsoft.org/libxml2/)
        * zlib [Available here.](http://zlib.net/)
      * glib
        * gettext [Available here.](http://www.gnu.org/software/gettext/gettext.html)
          * libiconv [Available here.](http://www.gnu.org/s/libiconv/)
        * zlib
        * pkg-config [Available here.](http://pkgconfig.freedesktop.org/releases/)
        * libffi [Available here.](http://sourceware.org/libffi/)
        * Python [Available here.](http://www.python.org/download/)
      * gtk
        * gdk-pixbuf
        * pango
          * cairo [Available here, along with pixman.](http://cairographics.org/releases/)
            * pixman
            * libpng [Available here.](http://www.libpng.org/pub/png/libpng.html)
            * ~~poppler [Available here.](http://poppler.freedesktop.org/)~~
        * atk
        * glib
        * gettext
        * libiconv
  1. Download and install Python (the 64 bit version).
    * Add your `python` and `python/dlls` directories to your `PATH`
  1. Start up a bash session by running `msys.bat` from your `msys` directory.
    * `export CFLAGS='-I/usr/include -I/usr/local/include'`
    * `export LDFLAGS='-L/usr/lib -L/usr/local/lib'`
  1. Install libiconv.
    1. Extract the source and change to that directory in your bash session.
    1. `./configure --host=x86_64-w64-mingw32`
    1. `make && make install`
  1. Install gettext.
    1. Extract the source and change to that directory in your bash session.
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. You will need to edit `gettext-tools/woe32dll/export.h` with the following patch [(reference)](http://sourceforge.net/projects/mingw-w64/forums/forum/723797/topic/3732248):
```
--- gettext-tools/woe32dll/export.h.origin	2011-07-31 08:41:27.000000000 +0800
+++ gettext-tools/woe32dll/export.h	2011-07-31 08:50:46.000000000 +0800
@@ -90,7 +90,11 @@
    --export-all-symbols is used.  */
 
  /* IMP(x) is a symbol that contains the address of x.  */
+#ifdef _WIN64
+#define IMP(x) __imp_##x
+#else
 #define IMP(x) _imp__##x
+#endif
 
  /* Ensure that the variable x is exported from the library, and that a
     pseudo-variable IMP(x) is available.  */
```
    1. `make && make install`
  1. Install zlib.
    1. Extract the source and change to that directory in your bash session.
    1. `make -f win32/Makefile.gcc`
    1. `cp zlib1.dll /usr/bin`
    1. `cp zconf.h zlib.h /usr/include`
    1. `cp libz.a /usr/lib`
    1. `cp libzdll.a /usr/lib/libz.dll.a`
    1. Download http://www.gaia-gis.it/spatialite-2.5.0/libz.la and save it to `c:\mingw64\lib`
    1. Create a `c:\mingw64\local\lib\pkgconfig\zlib.pc` and populate it as follows:
```
prefix=/usr
exec_prefix=${prefix}
libdir=${prefix}/lib
sharedlibdir=${prefix}/lib
includedir=${prefix}/include

Name: zlib
Description: zlib compression library
Version: 1.2.5

Requires:
Libs: -L${libdir} -L${sharedlibdir} -lz
Cflags: -I${includedir}
```
  1. Install libxml2.
    1. Extract the source for libxml2 change to the created directory in your bash session.
    1. ~~Edit the `include/libxml/xmlexports.h` with [these changes](http://git.gnome.org/browse/libxml2/commit/?id=a194ccb8d19ddde94c2c04ddf197e6a629f7cc9b).~~
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. We libffi to bootstrap glib since it has a circular dependency with pkg-config.
    1. Extract the source for libbffi and change to the directory created in you bash session.
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Now we bootstrap glib.
    1. Change to the glib directory in bash.
    1. `export LIBFFI_CFLAGS='-I/usr/local/lib/libffi-3.0.10/include'`
    1. `export LIBFFI_LIBS=-lffi`
    1. `export lt_cv_deplibs_check_method="pass_all"`
      * I don't know why, but whenever this environment variable is not set, libtool or something fails. This needs to be investigated.
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32` ~~`--disable-static`~~
    1. `make && make install`
  1. Extract pkg-config and switch to that directory in your bash session.
    1. `export GLIB_CFLAGS='-I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include'`
    1. `export GLIB_LIBS=-lglib-2.0`
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Now make a fresh glib. Delete your glib source directory, and close your bash session.
  1. Create a new bash session as in step 5.
    * `export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig`
  1. Extract glib and switch to that directory in your bash session.
    1. `export lt_cv_deplibs_check_method="pass_all"`
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Extract atk and switch to that directory in your bash session.
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Extract libpng and switch to that directory in your bash session.
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Extract pixman and switch to that directory in your bash session.
    1. Make a symbolic link from lib/libz.la to local/lib/libz.la
      1. Open cmd.exe as administater
      1. `mklink c:\mingw64\local\lib\libz.la c:\mingw64\lib\libz.la`
      1. `mklink c:\mingw64\local\lib\libz.dll.a c:\mingw64\lib\libz.dll.a`
      1. `mklink d:\mingw64\local\include\zlib.h d:\mingw64\include\zlib.h`
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. ~~Extract poppler and switch to that directory in your bash session.~~
    1. ~~`./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32 --enable-zlib`~~
    1. ~~`make && make install`~~
  1. Extract cairo and switch to that directory in your bash session.
    1. Edit `src/cairo.c` and add the following somewhere near the top of the file ([reference](http://lists.cairographics.org/archives/cairo-bugs/2010-September/003846.html)):
```
#ifdef __MINGW32__
#define ffs __builtin_ffs
#endif
```
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Extract libpng and switch to that directory in your bash session.
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Extract pango and switch to that directory in your bash session.
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
  1. Extract libpng and switch to that directory in your bash session.
    1. Create a `c:\wingw64\pkg-config.sh` as follows ([reference](http://mingw-users.1079350.n2.nabble.com/Smiley-argument-in-INCLUDES-when-using-in-make-only-in-c-td6410528.html)):
```
#!/bin/sh 
if pkg-config "$@" > /dev/null 2>&1 ; then 
   res=true 
else 
   res=false 
fi 
pkg-config "$@" | tr -d \\r && $res 
```
    1. `export PKG_CONFIG=/usr/bin/pkg-config.sh`
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`
    1. `make && make install`
    1. `unset PKG_CONFIG`
  1. Extract gtk3 and switch to that directory in your bash session.
    1. `export PKG_CONFIG=/usr/bin/pkg-config.sh`
    1. `./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32 --disable-modules`
    1. `make && make install`
    1. `unset PKG_CONFIG`
  1. Get intltool [from here.](http://freedesktop.org/wiki/Software/intltool)
    1. Extract it, switch to it's directory in bash.
    1. configure, make and install
  1. reinstall gettext, use --enable-threads=win32 --enable-relocatable [and this patch](http://svn.clazzes.org/svn/mingw-pkg/trunk/mingw-w64-deb/mingw-w64-gettext-0.18/patches/gettext-0.18-gnulib-dll-variable-fix.patch)
  1. install pthreads from the binary part of the mingw-w64 site