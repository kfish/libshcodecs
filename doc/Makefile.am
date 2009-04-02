docdir=$(prefix)/share/doc/@PACKAGE@

man_MANS =

EXTRA_DIST = $(man_MANS) Doxyfile.in

# ensure doxygen-build.stamp is included,  or else the documentation will
# not be built or distributed
noinst_DATA = doxygen-build.stamp

if HAVE_DOXYGEN
#doxygen-build.stamp: Doxyfile $(top_srcdir)/include/shcodecs/*.h $(top_srcdir)/src/examples/*.c
doxygen-build.stamp: Doxyfile $(top_srcdir)/include/shcodecs/*.h
	doxygen
	touch doxygen-build.stamp
else
doxygen-build.stamp:
	echo "*** Warning: Doxygen not found; documentation will not be built."
	touch doxygen-build.stamp
endif

dist_docdir = $(distdir)/libshcodecs

dist-hook:
	if test -d libshcodecs; then \
	  mkdir $(dist_docdir); \
	  for dir in libshcodecs/*; do \
            b=`basename $$dir`; \
            if test $$b != "CVS"; then \
              if test -d $$dir; then \
                mkdir $(dist_docdir)/$$b; \
	        for f in $$dir/*; do \
                  cp -p $$f $(dist_docdir)/$$b; \
                done \
              fi \
            fi \
          done \
	fi


install-data-local: doxygen-build.stamp
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	if test -d libshcodecs; then \
	  for dir in libshcodecs/*; do \
	    if test -d $$dir; then \
	      b=`basename $$dir`; \
	      $(mkinstalldirs) $(DESTDIR)$(docdir)/$$b; \
	      for f in $$dir/*; do \
		$(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/$$b; \
              done \
	    fi \
	  done \
	fi

uninstall-local:
	rm -rf $(DESTDIR)$(docdir)

clean-local:
	if test -d libshcodecs; then rm -rf libshcodecs; fi
	if test -f doxygen-build.stamp; then rm -f doxygen-build.stamp; fi
