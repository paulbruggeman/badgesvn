TOPDIR = $(HOME)/Devel/badge2016/software/branches/lloyd
SRCDIR = $(TOPDIR)/src
PROJDIR = $(TOPDIR)/Badge16.X
BADGEID_BIN = $(SRCDIR)/intelHex/badgeId.awk

OUR_INCLUDE=$(SRCDIR)/include

XC32 = /opt/microchip/xc32/v1.32
PIC32_MX_INCLUDE = $(XC32)/pic32mx/include
GCC_PIC32_INCLUDE = $(XC32)/lib/gcc/pic32mx/4.5.2/include
MCC_SPECIFIC = $(PIC32_MX_INCLUDE)/proc/p32mx460f512l.h

TAGS_FLAGS = --declarations -o $(SRCDIR)/TAGS
ALL_SOURCE = $(SRCDIR)/*.[ch] $(SRCDIR)/include/*.h $(SRCDIR)/apps/*.[ch]
BINARY_IMAGE = $(PROJDIR)/dist/default/production/Badge16.X.production.hex
DOC_FILE = $(TOPDIR)/docs/html/index.html

# From Makefile-default.mk
MP_PROCESSOR_OPTION=32MX270F256D
MP_LINKER_FILE_OPTION=,--script="../src/app_32MX270F256D.ld"

all: $(SRCDIR)/TAGS $(DOC_FILE) $(BINARY_IMAGE)

$(SRCDIR)/TAGS: $(ALL_SOURCE)
	rm -rf $@; \
	find $(SRCDIR) -name '*.[ch]' -print0 | xargs -0 etags $(TAGS_FLAGS); \
	find $(PIC32_MX_INCLUDE) -name lega-c -prune -o -name Cpp -prune -o -name proc -prune -o -name '*.[ch]' -print0 | xargs -0 etags $(TAGS_FLAGS) --append; \
	find $(GCC_PIC32_INCLUDE) -name '*.[ch]' -print0 | xargs -0 etags $(TAGS_FLAGS) --append; \
	etags $(TAGS_FLAGS) --append $(MCC_SPECIFIC)

$(DOC_FILE): $(ALL_SOURCE)
	cd ${TOPDIR}; \
	rm -rf docs; \
	doxygen

# need two files with different badge IDs to test comms, and for some
# reason badgeId.awk is not working for me. Can't replace ID with just
# anything because of checksums; however, to make checksum stay same,
# can use any two bytes whose sum = be + ef = 1ad. Because my sense of
# humor is stuck in 3rd grade, replace "beef" with "feeb"
# see burnit.sh for easy way to burn each image to a card

$(BINARY_IMAGE): $(ALL_SOURCE)
	cd $(PROJDIR); \
	make -f nbproject/Makefile-default.mk SUBPROJECTS= .build-conf
	cd $(PROJDIR); \
	make -f nbproject/Makefile-default.mk $(BINARY_IMAGE); \
	sed -e 's/beef/efbe/' $(BINARY_IMAGE) > $(BINARY_IMAGE).2
	cd $(TOPDIR); $(MAKE) tags

.PHONY: exec tags check

exec: $(BINARY_IMAGE)

tags: $(SRCDIR)/TAGS

DEFINES=-D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -DPIC32MX460F512L_PIM -D__32MX270F256D__ -D__LANGUAGE_C__ -D_SUPPRESS_PLIB_WARNING
INCLUDES=-I$(SRCDIR) -I$(OUR_INCLUDE) -I $(GCC_PIC32_INCLUDE) -I $(PIC32_MX_INCLUDE)
WARNINGS=-Wall -Wno-unknown-pragmas -Wno-attributes

check:
	gcc $(DEFINES) $(WARNINGS) $(INCLUDES) -c $(SRCDIR)/apps/firewall_ng.c; \
	gcc $(DEFINES) $(WARNINGS) $(INCLUDES) -c $(SRCDIR)/apps/firewall_comm.c; \
	rm ${SRCDIR}/apps/*.o
