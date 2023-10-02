# Makefile.am

# Set the automake options
AUTOMAKE_OPTIONS = foreign

# Variables
CFLAGS = @CFLAGS@
TARGET_INSTALL_DIR = $(HOME)/roko-local
SOURCE_INSTALL_DIR = "."
STATIC_DIR_NAME = "static"
INSTALLER_LOG_FILE = "./installer.log"
# Check local version
RUN_VERSION := $$(./$(TARGET) -v)
# Specify the shell to use in the makefile
SHELL := /bin/bash
# Set the .ONESHELL special target to use a specific shell for all recipes
.ONESHELL:
# Check system info
MACHINE := $$(uname -m)
PACK_NAME = $(TARGET)-$(VERSION)-$(OS)-$(MACHINE)

# Source files
roko_SOURCES = src/rk_core.c src/main.c src/anvil__roko.c

# Linking rule
LDADD = $(ROKO_LDFLAGS)
AM_LDFLAGS = -lm

if DEBUG_BUILD
AM_LDFLAGS += -ggdb -O0
#KLS_LDFLAGS = -ggdb -O0
#ROKO_CFLAGS += -DKLS_DEBUG_CORE
else
# Linker flag to strip symbols
AM_LDFLAGS += -s -O2
#KLS_LDFLAGS = -O2
endif

# Compiler flags
AM_CFLAGS = $(ROKO_CFLAGS) -Werror -Wpedantic -Wall -Wno-trigraphs

./src/anvil__roko.c:
	@echo -e "\033[1;34m[Makefile]\e[0m    Using \033[1;33mamboso\e[0m to generate [\033[1;34m./src/anvil__roko.c\e[0m and \033[1;34m./src/anvil__roko.h\e[0m] for [ \033[1;36m$(VERSION)\e[0m ]:"
	-./anvil -qXG ./src $(VERSION)
	@echo -e "\033[1;33mDone.\e[0m"

# Build rule for object files
%.o: %.c
	@echo -e "    \033[1;35mAM_CFLAGS\e[0m: [ \"\033[1;34m$(AM_CFLAGS)\e[0m\" ]"
	@echo -en "    [ $< ] -> [ \033[1;34m$@\e[0m ]:  "
	$(CCOMP) -c $(CFLAGS) $(AM_CFLAGS) $< -o $@
	@echo -e "\033[1;33mDone.\e[0m"

# Build rule for the final binary
$(TARGET): $(roko_SOURCES:.c=.o)
	@echo -e "    \033[1;35mAM_CFLAGS\e[0m: [ \"\033[1;34m$(AM_CFLAGS)\e[0m\" ]"
	@echo -e "    \033[1;35mAM_LDFLAGS\e[0m: [ \"\033[1;34m$(AM_LDFLAGS)\e[0m\" ]"
	@echo -e "    \033[1;35mLDADD\e[0m: [ \"\033[1;34m$(LDADD)\e[0m\" ]"
	@echo -en "\033[1;34m[Makefile]    Building [\033[1;35m $@ \033[1;34m]:\e[0m  "
	$(CCOMP) $(CFLAGS) $(AM_CFLAGS) $(roko_SOURCES:.c=.o) -o $@ $(LDADD) $(AM_LDFLAGS)
	@echo -e "\033[1;33mDone.\e[0m"

doc:
	@echo -e "Using doxygen to create tex + html from $(VERSION) src/:    "
	doxygen ./docs/roko.doxyfile
	@echo -e "\n\033[1;33mDone.\e[0m\n"
	sleep 1
	clear
	@echo -e "Listing ./doxygen directory contents:\n"
	ls -R ./doxygen
	@echo -e "\n\033[1;32mDone.\e[0m"
	@echo -e "\ncd to ./doxygen/latex and make tex into pdf:\n"
	cd ./doxygen/latex ; make | grep "Output written"
	@echo -e "\n\033[1;32mDone.\e[0m"
	@echo -e "\ncd back to ../..:\n"
	cd ../..
	@echo -e "\nmv ./doxygen/latex/refman.pdf to (../../docs/docs.pdf) :\n"
	mv ./doxygen/latex/refman.pdf ./docs/docs.pdf
	@echo -e "\n\033[1;32mDone.\e[0m"
	@echo -en "\nRemove ./doxygen/:    "
	yes | rm -rd ./doxygen/
	@echo -e "\n\033[1;32mDone.\e[0m"

clean:
	@echo -en "Cleaning build artifacts:  "
	-rm $(TARGET)
	-rm src/*.o
	-rm ./*.o
	-rm src/anvil__roko.*
	@echo -e "\033[1;33mDone.\e[0m"

cleanob:
	@echo -en "Cleaning object build artifacts:  "
	-rm src/*.o
	-rm ./*.o
	@echo -e "\033[1;33mDone.\e[0m"

# Default target (builds everything)
all: ./src/anvil__roko.c $(TARGET)

# Target to clean and rebuild
rebuild: clean all

# By default, build the binary
.DEFAULT_GOAL := all
