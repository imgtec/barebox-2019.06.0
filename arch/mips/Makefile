#
# Select the object file format to substitute into the linker script.
#
ifdef CONFIG_CPU_LITTLE_ENDIAN
32bit-emul		= elf32ltsmip
64bit-emul		= elf64ltsmip
else
32bit-emul		= elf32btsmip
64bit-emul		= elf64btsmip
endif

CPPFLAGS += -D__MIPS__ -fno-strict-aliasing -fno-merge-constants

cflags-y += -G 0 -mno-abicalls -fno-pic -pipe
cflags-y += -Wall -Wstrict-prototypes \
	-Wno-uninitialized -Wno-format -Wno-main

ifdef CONFIG_32BIT
ld-emul		= $(32bit-emul)
cflags-y	+= -mabi=32
endif

ifdef CONFIG_64BIT
ld-emul		= $(64bit-emul)
cflags-y	+= -mabi=64
endif

undef-all += -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__
undef-all += -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__
predef-be += -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__
predef-le += -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__

ifdef CONFIG_CPU_BIG_ENDIAN
cflags-y += $(shell $(CC) -dumpmachine |grep -q 'mips.*el-.*' && echo -EB $(undef-all) $(predef-be))
ldflags-y += $(shell $(CC) -dumpmachine |grep -q 'mips.*el-.*' && echo -EB )
endif

ifdef CONFIG_CPU_LITTLE_ENDIAN
cflags-y += $(shell $(CC) -dumpmachine |grep -q 'mips.*el-.*' || echo -EL $(undef-all) $(predef-le))
ldflags-y += $(shell $(CC) -dumpmachine |grep -q 'mips.*el-.*' || echo -EL )
endif

LDFLAGS += $(ldflags-y) -m $(ld-emul)
LDFLAGS_barebox += $(ldflags-y)

#
# CPU-dependent compiler/assembler options for optimization.
#
cflags-$(CONFIG_CPU_MIPS32_R1) += $(call cc-option,-march=mips32,-mips32 -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS32) -Wa,-mips32 -Wa,--trap
cflags-$(CONFIG_CPU_MIPS32_R2) += $(call cc-option,-march=mips32r2,-mips32r2 -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS32) -Wa,-mips32r2 -Wa,--trap
cflags-$(CONFIG_CPU_MIPS64_R1) += $(call cc-option,-march=mips64,-mips64 -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS64) -Wa,-mips64 -Wa,--trap
cflags-$(CONFIG_CPU_MIPS64_R2) += $(call cc-option,-march=mips64r2,-mips64r2 -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS64) -Wa,-mips64r2 -Wa,--trap
cflags-$(CONFIG_CPU_LOONGSON1)	+= \
	$(call cc-option,-march=mips32r2,-mips32r2 -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS32) \
	-Wa,-mips32r2 -Wa,--trap

CPPFLAGS += -DTEXT_BASE=$(CONFIG_TEXT_BASE)

ifndef CONFIG_MODULES
# Add cleanup flags
CPPFLAGS += -fdata-sections -ffunction-sections
LDFLAGS_barebox += -static --gc-sections --emit-relocs
endif

ifdef CONFIG_IMAGE_COMPRESSION
KBUILD_BINARY := arch/mips/pbl/zbarebox.bin
KBUILD_TARGET := zbarebox.bin
$(KBUILD_BINARY): $(KBUILD_TARGET)
else
KBUILD_BINARY := barebox.bin
KBUILD_TARGET := barebox.bin
endif

LDFLAGS_barebox += -nostdlib

machine-$(CONFIG_MACH_MIPS_MALTA)	:= malta
machine-$(CONFIG_MACH_MIPS_AR231X)	:= ar231x
machine-$(CONFIG_MACH_MIPS_ATH79)	:= ath79
machine-$(CONFIG_MACH_MIPS_BCM47XX)	:= bcm47xx
machine-$(CONFIG_MACH_MIPS_LOONGSON)	:= loongson
machine-$(CONFIG_MACH_MIPS_XBURST)	:= xburst
machdirs := $(patsubst %,arch/mips/mach-%/,$(machine-y))

ifeq ($(KBUILD_SRC),)
CPPFLAGS += $(patsubst %,-I%include,$(machdirs))
else
CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(machdirs))
endif

ifeq ($(incdir-y),)
incdir-y := $(machine-y)
endif
INCDIR   := arch-$(incdir-y)

ifeq ($(KBUILD_SRC),)
CPPFLAGS += -I$(BOARD)/include
else
CPPFLAGS += -I$(srctree)/$(BOARD)/include
endif

ifneq ($(machine-y),)
MACH  := arch/mips/mach-$(machine-y)/
else
MACH  :=
endif

common-y += $(BOARD)

common-y += $(MACH)
common-y += arch/mips/lib/
common-y += arch/mips/boot/
common-y += arch/mips/boards/

CPPFLAGS += $(cflags-y)
CFLAGS += $(cflags-y)

lds-$(CONFIG_GENERIC_LINKER_SCRIPT)   := arch/mips/lib/barebox.lds

cmd_barebox__ ?= $(LD) $(LDFLAGS) $(LDFLAGS_barebox) -o $@ \
      -T $(barebox-lds)                         \
      --start-group $(barebox-common) --end-group                  \
      $(filter-out $(barebox-lds) $(barebox-common) FORCE ,$^);	\
	$(objtree)/scripts/mips-relocs $@


CLEAN_FILES    += arch/mips/lib/barebox.lds barebox.map barebox.S

pbl := arch/mips/pbl
zbarebox.S zbarebox.bin zbarebox: barebox.bin
	$(Q)$(MAKE) $(build)=$(pbl) $(pbl)/$@

archclean:
	$(MAKE) $(clean)=$(pbl)

dts := arch/mips/dts

%.dtb: scripts
	$(Q)$(MAKE) $(build)=$(dts) $(dts)/$@

common-$(CONFIG_OFTREE) += arch/mips/dts/

KBUILD_IMAGE ?= $(KBUILD_BINARY)
