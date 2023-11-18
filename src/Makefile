SOURCES = call.c unicode.c
PYTHON_VERSION = $(shell python3 -c 'import sys; ver=sys.version_info; print("%s.%s" % (ver.major, ver.minor))')
PKG_CONFIG_PYTHON = python-$(PYTHON_VERSION)
CFLAGS = $(shell pkg-config $(PKG_CONFIG_PYTHON) --cflags)
LDFLAGS = $(shell pkg-config $(PKG_CONFIG_PYTHON) --libs)

libdeadparrot.so:
	gcc -shared -o $@ -fPIC $(SOURCES) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f libdeadparrot.so
