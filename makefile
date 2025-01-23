################################################################
#
# $Id:$
#
# $Log:$
#
#RISCV_PREFIX ?= riscv$(XLEN)-unknown-elf-
RISCV_PREFIX = riscv64-elf-
CC = $(RISCV_PREFIX)gcc
#CC= riscv64-elf-gcc  # gcc or g++

#CFLAGS=-g -Wall -DNORMALUNIX -DLINUX # -DUSEASM
#CFLAGS=-g -m80387 -DALLOCA
#CFLAGS= -I. -O -nostartfiles -g -march=rv32im  -mabi=ilp32
CFLAGS= -I. -I./riscv -O0 -static -nostdlib -nostartfiles -std=gnu11 -fno-common -fno-builtin-fprintf -fno-builtin-printf  -fno-tree-loop-distribute-patterns -march=rv32im_zicsr  -mabi=ilp32 -T ./riscv/test.ld  -mcmodel=medany
#CFLAGS= -I. -O -static -march=rv32im  -mabi=ilp32
#LDFLAGS=-L/usr/X11R6/lib
#LIBS=-lXext -lX11 -lnsl -lm
LDFLAGS= -mcmodel=medany
#LIBS= -lgcc -lgr -lpc
#LIBS= -lm -lgcc
LIBS= -lgcc
#LIBS= 
OBJDUMP = $(RISCV_PREFIX)objdump
RISCV_OBJDUMP ?= $(RISCV_PREFIX)objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.text.init --section=.data 

# subdirectory for objects
O=obj

# not too sophisticated dependency
OBJS=				\
		$(O)/doomdef.o		\
		$(O)/doomstat.o		\
		$(O)/dstrings.o		\
		$(O)/i_net.o			\
		$(O)/tables.o			\
		$(O)/f_finale.o		\
		$(O)/f_wipe.o 		\
		$(O)/d_main.o			\
		$(O)/d_net.o			\
		$(O)/d_items.o		\
		$(O)/g_game.o			\
		$(O)/m_menu.o			\
		$(O)/m_misc.o			\
		$(O)/m_argv.o  		\
		$(O)/m_bbox.o			\
		$(O)/m_fixed.o		\
		$(O)/m_swap.o			\
		$(O)/m_cheat.o		\
		$(O)/m_random.o		\
		$(O)/am_map.o			\
		$(O)/p_ceilng.o		\
		$(O)/p_doors.o		\
		$(O)/p_enemy.o		\
		$(O)/p_floor.o		\
		$(O)/p_inter.o		\
		$(O)/p_lights.o		\
		$(O)/p_map.o			\
		$(O)/p_maputl.o		\
		$(O)/p_plats.o		\
		$(O)/p_pspr.o			\
		$(O)/p_setup.o		\
		$(O)/p_sight.o		\
		$(O)/p_spec.o			\
		$(O)/p_switch.o		\
		$(O)/p_mobj.o			\
		$(O)/p_telept.o		\
		$(O)/p_tick.o			\
		$(O)/p_saveg.o		\
		$(O)/p_user.o			\
		$(O)/r_bsp.o			\
		$(O)/r_data.o			\
		$(O)/r_draw.o			\
		$(O)/r_main.o			\
		$(O)/r_plane.o		\
		$(O)/r_segs.o			\
		$(O)/r_sky.o			\
		$(O)/r_things.o		\
		$(O)/w_wad.o			\
		$(O)/wi_stuff.o		\
		$(O)/v_video.o		\
		$(O)/st_lib.o			\
		$(O)/st_stuff.o		\
		$(O)/hu_stuff.o		\
		$(O)/hu_lib.o			\
		$(O)/s_sound.o		\
		$(O)/z_zone.o			\
		$(O)/info.o				\
		$(O)/sounds.o	\
#		$(O)/strncasecmp.o	\
#		$(O)/strcasecmp.o	
#		$(O)/muldi3.o	\
#		$(O)/ashrdi3.o

SYSOBJ	=	\
		$(O)/i_system.o		\
		$(O)/i_video.o		\
		$(O)/i_sound.o		\
		$(O)/stdlib.o		\
		$(O)/stdio.o		\
		$(O)/string.o		\
		$(O)/ctype.o		\
		$(O)/malloc.o		\
		$(O)/fcntl.o		\
		$(O)/stat.o			\
		$(O)/unistd.o		\
		$(O)/crt.o		\
#		$(O)/xmalloc.o		\

#all:	 $(O)/doom

clean:
	rm $(O)/*.o  || true
	rm $(O)/doom || true
	rm test/doom || true

remake: clean all

$(O)/doom:	$(OBJS) $(SYSOBJ) $(O)/i_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(SYSOBJ) $(O)/i_main.o -o $(O)/doom $(LIBS)
	cp $(O)/doom test
#	copy /Y $(O)\doom test
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(SYSOBJ) $(O)/i_main.o -o $(O)/doom $(LIBS)
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(O)/i_main.o -o $(O)/dosdoom.exe $(LIBS)

$(O)/doom.dump:	$(O)/doom
	$(RISCV_OBJDUMP) $(O)/doom  > $@

$(O)/doom.symbols:	$(O)/doom
	$(OBJDUMP) --show-all-symbols -x $(O)/doom  > $@

$(O)/%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(O)/doomdef.o:
	$(CC) $(CFLAGS) -c doomdef.c -o $@
$(O)/i_main.o:
	$(CC) $(CFLAGS) -c i_main.c -o $@
$(O)/doomstat.o:
	$(CC) $(CFLAGS) -c doomstat.c -o $@
$(O)/dstrings.o:
	$(CC) $(CFLAGS) -c dstrings.c -o $@
# $(O)/i_sound.o:
# 	$(CC) $(CFLAGS) -c i_sound.c -o $@
$(O)/i_net.o:
	$(CC) $(CFLAGS) -c i_net.c -o $@
$(O)/tables.o:
	$(CC) $(CFLAGS) -c tables.c -o $@
$(O)/f_finale.o:
	$(CC) $(CFLAGS) -c f_finale.c -o $@
$(O)/f_wipe.o:
	$(CC) $(CFLAGS) -c f_wipe.c -o $@
$(O)/d_main.o:
	$(CC) $(CFLAGS) -c d_main.c -o $@
$(O)/d_net.o:
	$(CC) $(CFLAGS) -c d_net.c -o $@
$(O)/d_items.o:
	$(CC) $(CFLAGS) -c d_items.c -o $@
$(O)/g_game.o:
	$(CC) $(CFLAGS) -c g_game.c -o $@
$(O)/m_menu.o:
	$(CC) $(CFLAGS) -c m_menu.c -o $@
$(O)/m_misc.o:
	$(CC) $(CFLAGS) -c m_misc.c -o $@
$(O)/m_argv.o:
	$(CC) $(CFLAGS) -c m_argv.c -o $@
$(O)/m_bbox.o:
	$(CC) $(CFLAGS) -c m_bbox.c -o $@
$(O)/m_fixed.o:
	$(CC) $(CFLAGS) -c m_fixed.c -o $@
$(O)/m_swap.o:
	$(CC) $(CFLAGS) -c m_swap.c -o $@
$(O)/m_cheat.o:
	$(CC) $(CFLAGS) -c m_cheat.c -o $@
$(O)/m_random.o:
	$(CC) $(CFLAGS) -c m_random.c -o $@
$(O)/am_map.o:
	$(CC) $(CFLAGS) -c am_map.c -o $@
$(O)/p_ceilng.o:
	$(CC) $(CFLAGS) -c p_ceilng.c -o $@
$(O)/p_doors.o:
	$(CC) $(CFLAGS) -c p_doors.c -o $@
$(O)/p_enemy.o:
	$(CC) $(CFLAGS) -c p_enemy.c -o $@
$(O)/p_floor.o:
	$(CC) $(CFLAGS) -c p_floor.c -o $@
$(O)/p_inter.o:
	$(CC) $(CFLAGS) -c p_inter.c -o $@
$(O)/p_lights.o:
	$(CC) $(CFLAGS) -c p_lights.c -o $@
$(O)/p_map.o:
	$(CC) $(CFLAGS) -c p_map.c -o $@
$(O)/p_maputl.o:
	$(CC) $(CFLAGS) -c p_maputl.c -o $@
$(O)/p_plats.o:
	$(CC) $(CFLAGS) -c p_plats.c -o $@
$(O)/p_pspr.o:
	$(CC) $(CFLAGS) -c p_pspr.c -o $@
$(O)/p_setup.o:
	$(CC) $(CFLAGS) -c p_setup.c -o $@
$(O)/p_sight.o:
	$(CC) $(CFLAGS) -c p_sight.c -o $@
$(O)/p_spec.o:
	$(CC) $(CFLAGS) -c p_spec.c -o $@
$(O)/p_switch.o:
	$(CC) $(CFLAGS) -c p_switch.c -o $@
$(O)/p_mobj.o:
	$(CC) $(CFLAGS) -c p_mobj.c -o $@
$(O)/p_telept.o:
	$(CC) $(CFLAGS) -c p_telept.c -o $@
$(O)/p_tick.o:
	$(CC) $(CFLAGS) -c p_tick.c -o $@
$(O)/p_saveg.o:
	$(CC) $(CFLAGS) -c p_saveg.c -o $@
#	 $(CC) -I. -c p_saveg.c -o $@
$(O)/p_user.o:
	$(CC) $(CFLAGS) -c p_user.c -o $@
$(O)/r_bsp.o:
	$(CC) $(CFLAGS) -c r_bsp.c -o $@
$(O)/r_data.o:
	$(CC) $(CFLAGS) -c r_data.c -o $@
$(O)/r_draw.o:
	$(CC) $(CFLAGS) -c r_draw.c -o $@
$(O)/r_main.o:
	$(CC) $(CFLAGS) -c r_main.c -o $@
$(O)/r_plane.o:
	$(CC) $(CFLAGS) -c r_plane.c -o $@
$(O)/r_segs.o:
	$(CC) $(CFLAGS) -c r_segs.c -o $@
$(O)/r_sky.o:
	$(CC) $(CFLAGS) -c r_sky.c -o $@
$(O)/r_things.o:
	$(CC) $(CFLAGS) -c r_things.c -o $@
$(O)/w_wad.o:
	$(CC) $(CFLAGS) -c w_wad.c -o $@
$(O)/wi_stuff.o:
	$(CC) $(CFLAGS) -c wi_stuff.c -o $@
$(O)/v_video.o:
	$(CC) $(CFLAGS) -c v_video.c -o $@
$(O)/st_lib.o:
	$(CC) $(CFLAGS) -c st_lib.c -o $@
$(O)/st_stuff.o:
	$(CC) $(CFLAGS) -c st_stuff.c -o $@
$(O)/hu_stuff.o:
	$(CC) $(CFLAGS) -c hu_stuff.c -o $@
$(O)/hu_lib.o:
	$(CC) $(CFLAGS) -c hu_lib.c -o $@
$(O)/s_sound.o:
	$(CC) $(CFLAGS) -c s_sound.c -o $@
$(O)/z_zone.o:
	$(CC) $(CFLAGS) -c z_zone.c -o $@
$(O)/info.o:
	$(CC) $(CFLAGS) -c info.c -o $@
$(O)/sounds.o:
	$(CC) $(CFLAGS) -c sounds.c -o $@
$(O)/strncasecmp.o:
	$(CC) $(CFLAGS) -c strncasecmp.c -o $@
$(O)/strcasecmp.o:
	$(CC) $(CFLAGS) -c strcasecmp.c -o $@
$(O)/muldi3.o:
	$(CC) $(CFLAGS) -c muldi3.c -o $@
$(O)/ashrdi3.o:
	$(CC) $(CFLAGS) -c ashrdi3.c -o $@

#Go32
#$(O)/i_video.o:
#	$(CC) $(CFLAGS) -c go32/i_video.c -o $@
#$(O)/i_system.o:
#	$(CC) $(CFLAGS) -c go32/i_system.c -o $@
#go32 DPMI
# $(O)/i_video.o:
# 	$(CC) $(CFLAGS) -c go32_dpmi/i_video.c -o $@
# $(O)/i_system.o:
# 	$(CC) $(CFLAGS) -c go32_dpmi/i_system.c -o $@
# $(O)/dpmi.o:
# 	$(CC) $(CFLAGS) -c go32_dpmi/dpmi.c -o $@
# NULL
#$(O)/i_video.o:
#	$(CC) $(CFLAGS) -c null/i_video.c -o $@
#$(O)/i_system.o:
#	$(CC) $(CFLAGS) -c null/i_system.c -o $@

#GCC 1.39
#$(O)/xmalloc.o:
#	$(CC) $(CFLAGS) -c go32/xmalloc.c -o $@

#RISCV-VM
$(O)/malloc.o:
	$(CC) $(CFLAGS) -c riscv/malloc.c -o $@
$(O)/fcntl.o:
	$(CC) $(CFLAGS) -c riscv/fcntl.c -o $@
$(O)/stat.o:
	$(CC) $(CFLAGS) -c riscv/sys/stat.c -o $@
$(O)/unistd.o:
	$(CC) $(CFLAGS) -c riscv/unistd.c -o $@
$(O)/crt.o:
	$(CC) $(CFLAGS) -c riscv/crt.S -o $@

$(O)/i_video.o:
	$(CC) $(CFLAGS) -c riscv/i_video.c -o $@
$(O)/i_system.o:
	$(CC) $(CFLAGS) -c riscv/i_system.c -o $@
$(O)/i_sound.o:
	$(CC) $(CFLAGS) -c riscv/i_sound.c -o $@
$(O)/stdlib.o:
	$(CC) $(CFLAGS) -c riscv/stdlib.c -o $@
$(O)/stdio.o:
	$(CC) $(CFLAGS) -c riscv/stdio.c -o $@
$(O)/string.o:
	$(CC) $(CFLAGS) -c riscv/string.c -o $@
$(O)/ctype.o:
	$(CC) $(CFLAGS) -c riscv/ctype.c -o $@

#------------------------------------------------------------
# Default
.DEFAULT_GOAL := all

all: $(O)/doom.dump $(O)/doom.symbols

#############################################################
#
#############################################################