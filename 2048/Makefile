#name: Game6-2048
#description: a tile game 2048 implementation for Win-i386/amd64 command-prompt
#platform: it is tested under MinGW-w64, MSYS2, and/or gtk+ 2.22

TARGET := main_gtk
all: $(TARGET)

EXE = g6-2048.exe
ICO = g6-2048.svg
REL_DIR = g6-2048
REL_PKG = g6-2048
REL_TITLE = 2048
REL_VERSION := 1.0.0
REL_DESCRIPTION = $(REL_TITLE), a tile-based game; from Game6 project
REL_AR = $(REL_DIR)-v$(REL_VERSION).deb

ifeq ($(TARGET),main_gtk)
TERMINAL = false
else
TERMINAL = true
endif

DPKG_DEB = dpkg-deb
DPKG = dpkg
INSTALL = install
GIT = git
ECHO = echo
TOUCH = touch
MKDIR = mkdir
RM = rm
CC = gcc

ifeq ($(OS),Windows_NT)
CFLAGS = -mms-bitfields -I"C:/mingw64/include/gtk-2.0" \
 -I"C:/mingw64/lib/gtk-2.0/include" -I"C:/mingw64/include/atk-1.0" \
 -I"C:/mingw64/include/cairo" -I"C:/mingw64/include/pango-1.0" \
 -I"C:/mingw64/include/glib-2.0" -I"C:/mingw64/lib/glib-2.0/include" \
 -I"C:/mingw64/include/libpng14" -I"C:/mingw64/include/gdk-pixbuf-2.0"

LIBS = -L"C:/mingw64/lib" -lgtk-win32-2.0 -lgdk-win32-2.0 -latk-1.0 \
 -lgdk_pixbuf-2.0 -lpangowin32-1.0 -lpangocairo-1.0 -lpango-1.0 -lcairo \
 -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -lintl
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CFLAGS = -w -pthread -I/usr/include/gtk-2.0\
 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/pango-1.0\
 -I/usr/include/atk-1.0 -I/usr/include/gdk-pixbuf-2.0\
 -I/usr/include/libmount -I/usr/include/blkid\
 -I/usr/include/pango-1.0 -I/usr/include/fribidi\
 -I/usr/include/cairo -I/usr/include/pixman-1\
 -I/usr/include/harfbuzz -I/usr/include/glib-2.0\
 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/uuid\
 -I/usr/include/freetype2 -I/usr/include/libpng16

LIBS = -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0\
 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0\
 -lgobject-2.0 -lglib-2.0 -lharfbuzz -lfontconfig -lfreetype

RELEASE = rel-deb
endif
endif

rel-deb-clr:
	@$(RM) -rf $(REL_DIR)
	@$(RM) -rf $(REL_AR)

rel-deb: rel-deb-clr
	@$(ECHO) "releasing debian package"
	@$(MKDIR) -p $(REL_DIR)
	@$(MKDIR) -p $(REL_DIR)/DEBIAN
	@$(MKDIR) -p $(REL_DIR)/usr/local/bin
	@$(INSTALL) -m 755 -D $(EXE) $(REL_DIR)/usr/local/bin/$(EXE)
	@$(TOUCH) $(REL_DIR)/DEBIAN/control
	@$(ECHO) "Package: ${REL_PKG}" >> $(REL_DIR)/DEBIAN/control
	@$(ECHO) "Version: ${REL_VERSION}" >> $(REL_DIR)/DEBIAN/control
	@$(ECHO) "Maintainer: $(shell $(GIT) config --get user.name)" >> $(REL_DIR)/DEBIAN/control
	@$(ECHO) "Architecture: $(shell $(DPKG) --print-architecture)" >> $(REL_DIR)/DEBIAN/control
	@$(ECHO) "Description: ${REL_DESCRIPTION}" >> $(REL_DIR)/DEBIAN/control
ifeq ($(TERMINAL),false)
	@$(MKDIR) -p $(REL_DIR)/usr/share/applications
	@$(TOUCH) $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "[Desktop Entry]" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Name=${REL_TITLE}" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Comment=${REL_TITLE}" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Type=Application" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Keywords=${REL_TITLE}" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Exec=${EXE}" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Terminal=${TERMINAL}" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Icon=${ICO}" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(ECHO) "Categories=Utility;" >> $(REL_DIR)/usr/share/applications/g6-2048.desktop
	@$(MKDIR) -p $(REL_DIR)/usr/share/pixmaps
	@$(INSTALL) -m 644 -D $(ICO) $(REL_DIR)/usr/share/pixmaps/$(ICO)
endif
	@$(DPKG_DEB) --build $(REL_DIR) $(REL_AR)

release: $(RELEASE)
	@$(ECHO) "released"

main_gtk:
	$(CC) -o $(EXE) game_2048.c main_gtk.c $(CFLAGS) $(LIBS)

main_gnu:
	$(CC) -o $(EXE) game_2048.c main_gnu.c

main_ansi:
	$(CC) -o $(EXE) game_2048.c main_ansi.c

.PHONY: clean all release

clean:
	$(RM) $(EXE)

