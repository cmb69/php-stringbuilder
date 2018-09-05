dnl config.m4 for extension stringbuilder

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(stringbuilder, for stringbuilder support,
dnl Make sure that the comment is aligned:
dnl [  --with-stringbuilder             Include stringbuilder support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(stringbuilder, whether to enable stringbuilder support,
dnl Make sure that the comment is aligned:
[  --enable-stringbuilder          Enable stringbuilder support], no)

if test "$PHP_STRINGBUILDER" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, STRINGBUILDER_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-stringbuilder -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/stringbuilder.h"  # you most likely want to change this
  dnl if test -r $PHP_STRINGBUILDER/$SEARCH_FOR; then # path given as parameter
  dnl   STRINGBUILDER_DIR=$PHP_STRINGBUILDER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for stringbuilder files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       STRINGBUILDER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$STRINGBUILDER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the stringbuilder distribution])
  dnl fi

  dnl # --with-stringbuilder -> add include path
  dnl PHP_ADD_INCLUDE($STRINGBUILDER_DIR/include)

  dnl # --with-stringbuilder -> check for lib and symbol presence
  dnl LIBNAME=STRINGBUILDER # you may want to change this
  dnl LIBSYMBOL=STRINGBUILDER # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $STRINGBUILDER_DIR/$PHP_LIBDIR, STRINGBUILDER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_STRINGBUILDERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong stringbuilder lib version or lib not found])
  dnl ],[
  dnl   -L$STRINGBUILDER_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(STRINGBUILDER_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_STRINGBUILDER, 1, [ Have stringbuilder support ])

  PHP_NEW_EXTENSION(stringbuilder, stringbuilder.c, $ext_shared)
fi
