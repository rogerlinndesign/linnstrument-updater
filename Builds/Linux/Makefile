# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION=2.0.1 -DJUCE_APP_VERSION_HEX=0x20001 $(shell pkg-config --cflags freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../JuceLibraryCode/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs freetype2 libcurl x11 xext xinerama) -ldl -lpthread -lrt 

  TARGET := LinnStrumentUpdater
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION=2.0.1 -DJUCE_APP_VERSION_HEX=0x20001 $(shell pkg-config --cflags freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../JuceLibraryCode/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -Os
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ $(shell pkg-config --libs freetype2 libcurl x11 xext xinerama) -ldl -lpthread -lrt 

  TARGET := LinnStrumentUpdater
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/list_ports_linux_a6619512.o \
  $(JUCE_OBJDIR)/list_ports_osx_77df00f2.o \
  $(JUCE_OBJDIR)/list_ports_win_84f44f2a.o \
  $(JUCE_OBJDIR)/unix_ff828b7e.o \
  $(JUCE_OBJDIR)/win_1be697ba.o \
  $(JUCE_OBJDIR)/serial_31f12e13.o \
  $(JUCE_OBJDIR)/disphelper_fddea979.o \
  $(JUCE_OBJDIR)/LinnStrumentSerial_161389bc.o \
  $(JUCE_OBJDIR)/LinnStrumentSerialMac_6f4d17fd.o \
  $(JUCE_OBJDIR)/LinnStrumentSerialWindows_957ef6b1.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/MainWindow_499ac812.o \
  $(JUCE_OBJDIR)/ProjectsComponent_9939ad84.o \
  $(JUCE_OBJDIR)/UpdaterApplication_52906ad0.o \
  $(JUCE_OBJDIR)/UpgradeComponent_f099604a.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking LinnStrument Updater
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	@$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning LinnStrument Updater
	@$(CLEANCMD)

strip:
	@echo Stripping LinnStrument Updater
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/list_ports_linux_a6619512.o: ../../Source/Serial/impl/list_ports/list_ports_linux.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_linux.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_osx_77df00f2.o: ../../Source/Serial/impl/list_ports/list_ports_osx.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_osx.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_win_84f44f2a.o: ../../Source/Serial/impl/list_ports/list_ports_win.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_win.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/unix_ff828b7e.o: ../../Source/Serial/impl/unix.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling unix.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/win_1be697ba.o: ../../Source/Serial/impl/win.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling win.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/serial_31f12e13.o: ../../Source/Serial/serial.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling serial.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/disphelper_fddea979.o: ../../Source/disphelper.c
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling disphelper.c"
	@$(CC) $(JUCE_CFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LinnStrumentSerial_161389bc.o: ../../Source/LinnStrumentSerial.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LinnStrumentSerial.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LinnStrumentSerialMac_6f4d17fd.o: ../../Source/LinnStrumentSerialMac.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LinnStrumentSerialMac.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LinnStrumentSerialWindows_957ef6b1.o: ../../Source/LinnStrumentSerialWindows.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LinnStrumentSerialWindows.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProjectsComponent_9939ad84.o: ../../Source/ProjectsComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProjectsComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UpdaterApplication_52906ad0.o: ../../Source/UpdaterApplication.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UpdaterApplication.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UpgradeComponent_f099604a.o: ../../Source/UpgradeComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UpgradeComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
