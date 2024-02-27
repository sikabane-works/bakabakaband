## Makefile.am -- Process this file with automake to process Makefile.in

AUTOMAKE_OPTIONS = foreign

visual_studio_files = \
	Bakabakaband/Bakabakaband.sln \
	Bakabakaband/Bakabakaband/curl/include/curl/curl.h \
	Bakabakaband/Bakabakaband/curl/include/curl/curlver.h \
	Bakabakaband/Bakabakaband/curl/include/curl/easy.h \
	Bakabakaband/Bakabakaband/curl/include/curl/mprintf.h \
	Bakabakaband/Bakabakaband/curl/include/curl/multi.h \
	Bakabakaband/Bakabakaband/curl/include/curl/options.h \
	Bakabakaband/Bakabakaband/curl/include/curl/stdcheaders.h \
	Bakabakaband/Bakabakaband/curl/include/curl/system.h \
	Bakabakaband/Bakabakaband/curl/include/curl/typecheck-gcc.h \
	Bakabakaband/Bakabakaband/curl/include/curl/urlapi.h \
	Bakabakaband/Bakabakaband/Bakabakaband.vcxproj \
	Bakabakaband/Bakabakaband/Bakabakaband.vcxproj.filters \
	Bakabakaband/Bakabakaband/packages.config

EXTRA_DIST = \
	autopick.txt \
	autopick_eng.txt \
	bootstrap \
	readme.md \
	readme_angband \
	readme-eng.md \
	hengband.spec \
	$(visual_studio_files)

SUBDIRS = src lib

# Handle some paths in Bakabakaband/Bakabakaband that have spaces in them and would
# be mishandled by make/automake.
dist-hook:
	(cd $(srcdir)/Bakabakaband/Bakabakaband/curl && tar -cf - "x86 Debug" "x86 Release" ) | (cd $(distdir)/Bakabakaband/Bakabakaband/curl && tar -xf -)
