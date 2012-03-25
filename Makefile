.DEFAULT_GOAL := r3

PREENFM_VERSION_NUMBER=1.62.B1
PREENFM_VERSION=\"$(PREENFM_VERSION_NUMBER)\"

LIB_MAPLE_HOME=/home/xhosxe/libmaple

OBJECTS= build/Env.o build/FM.o build/Osc.o build/Synth.o build/Voice.o build/Lfo.o build/Matrix.o build/RingBuffer.o build/MidiDecoder.o build/Encoders.o build/FMDisplay.o build/SynthState.o build/Menu.o build/PresetUtil.o build/Presets.o build/LfoOsc.o build/LfoEnv.o build/LfoStepSeq.o build/LiquidCrystal.o

LIBMAPLE_OBJECTS= $(LIB_MAPLE_HOME)/build/libmaple/i2c.o $(LIB_MAPLE_HOME)/build/libmaple/systick.o $(LIB_MAPLE_HOME)/build/libmaple/timer.o $(LIB_MAPLE_HOME)/build/libmaple/adc.o $(LIB_MAPLE_HOME)/build/libmaple/syscalls.o $(LIB_MAPLE_HOME)/build/libmaple/exti.o $(LIB_MAPLE_HOME)/build/libmaple/gpio.o $(LIB_MAPLE_HOME)/build/libmaple/nvic.o $(LIB_MAPLE_HOME)/build/libmaple/usart.o $(LIB_MAPLE_HOME)/build/libmaple/util.o $(LIB_MAPLE_HOME)/build/libmaple/rcc.o $(LIB_MAPLE_HOME)/build/libmaple/flash.o $(LIB_MAPLE_HOME)/build/libmaple/spi.o $(LIB_MAPLE_HOME)/build/libmaple/fsmc.o $(LIB_MAPLE_HOME)/build/libmaple/dac.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb_callbacks.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb_hardware.o $(LIB_MAPLE_HOME)/build/libmaple/usb/descriptors.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb_lib/usb_core.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb_lib/usb_init.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb_lib/usb_int.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb_lib/usb_mem.o $(LIB_MAPLE_HOME)/build/libmaple/usb/usb_lib/usb_regs.o $(LIB_MAPLE_HOME)/build/libmaple/exc.o $(LIB_MAPLE_HOME)/build/wirish/wirish_shift.o $(LIB_MAPLE_HOME)/build/wirish/wirish_analog.o $(LIB_MAPLE_HOME)/build/wirish/wirish_time.o $(LIB_MAPLE_HOME)/build/wirish/pwm.o $(LIB_MAPLE_HOME)/build/wirish/ext_interrupts.o $(LIB_MAPLE_HOME)/build/wirish/wirish_digital.o $(LIB_MAPLE_HOME)/build/wirish/wirish_math.o $(LIB_MAPLE_HOME)/build/wirish/Print.o $(LIB_MAPLE_HOME)/build/wirish/comm/HardwareSerial.o $(LIB_MAPLE_HOME)/build/wirish/comm/HardwareSPI.o $(LIB_MAPLE_HOME)/build/wirish/usb_serial.o $(LIB_MAPLE_HOME)/build/wirish/HardwareTimer.o $(LIB_MAPLE_HOME)/build/wirish/boards/maple_mini.o $(LIB_MAPLE_HOME)/build/wirish/boards.o $(LIB_MAPLE_HOME)/build/wirish/cxxabi-compat.o $(LIB_MAPLE_HOME)/build/libraries/Servo/Servo.o $(LIB_MAPLE_HOME)/build/libraries/Wire/Wire.o

#  $(LIB_MAPLE_HOME)/build/wirish/wirish.o
#  $(LIB_MAPLE_HOME)/build/libmaple/i2c.o

# Valid BOARDs: maple, maple_native, ...
BOARD = maple_mini
MEMORY_TARGET = flash

# USB ID for DFU upload
VENDOR_ID  := 1EAF
PRODUCT_ID := 0003

# Guess the MCU based on the BOARD (can be overridden )
ifeq ($(BOARD), maple)
   MCU := STM32F103RB
   PRODUCT_ID := 0003
endif
ifeq ($(BOARD), maple_native)
   MCU := STM32F103ZE
   PRODUCT_ID := 0003
endif
ifeq ($(BOARD), maple_mini)
   MCU := STM32F103CB
   PRODUCT_ID := 0003
endif

# Useful paths
SRC_PATH := src
LIBMAPLE_PATH := $(LIB_MAPLE_HOME)/libmaple
SUPPORT_PATH := $(LIB_MAPLE_HOME)/support
BUILD_PATH :=  $(LIB_MAPLE_HOME)/build

PROJECT_BUILD_PATH=build/
# Useful variables
GLOBAL_CFLAGS   := -O2 -g -mcpu=cortex-m3 -mthumb -march=armv7-m -nostdlib \
                   -ffunction-sections -fdata-sections -Wl,--gc-sections   \
                   -DBOARD_$(BOARD) -DMCU_$(MCU) -fsigned-char -DSTM32_MEDIUM_DENSITY -I/home/xhosxe/libmaple/libraries/LiquidCrystal -I/home/xhosxe/libmaple/libraries/Wire

GLOBAL_CXXFLAGS := -fno-rtti -fno-exceptions -Wall -DBOARD_$(BOARD) -DMCU_$(MCU) -DPCB_$(PCB_VERSION) -DPREENFM_VERSION=$(PREENFM_VERSION)
GLOBAL_ASFLAGS  := -mcpu=cortex-m3 -march=armv7-m -mthumb -DBOARD_$(BOARD) \
                   -DMCU_$(MCU) -x assembler-with-cpp 

LDDIR    := $(SUPPORT_PATH)/ld
LDFLAGS  = -T$(LDDIR)/$(LDSCRIPT) -L$(LDDIR)    \
            -mcpu=cortex-m3 -mthumb -Xlinker     \
            --gc-sections --print-gc-sections --march=armv7-m -Wall 
#-L$(LIB_MAPLE_HOME)/build/ -lmaple

# Set up build rules and some useful templates
include $(SUPPORT_PATH)/make/build-rules.mk
include $(SUPPORT_PATH)/make/build-templates.mk

# Some target specific things
ifeq ($(MEMORY_TARGET), ram)
   LDSCRIPT := $(BOARD)/ram.ld
   VECT_BASE_ADDR := VECT_TAB_RAM
endif
ifeq ($(MEMORY_TARGET), flash)
   LDSCRIPT := $(BOARD)/flash.ld
   VECT_BASE_ADDR := VECT_TAB_FLASH
endif
ifeq ($(MEMORY_TARGET), jtag)
   LDSCRIPT := $(BOARD)/jtag.ld
   VECT_BASE_ADDR := VECT_TAB_BASE
endif

# Set all submodules here
LIBMAPLE_MODULES := $(LIB_MAPLE_HOME)/libmaple
LIBMAPLE_MODULES += $(LIB_MAPLE_HOME)/wirish
# Official libraries:
LIBMAPLE_MODULES += $(LIB_MAPLE_HOME)/libraries/Servo
LIBMAPLE_MODULES += $(LIB_MAPLE_HOME)/libraries/LiquidCrystal
LIBMAPLE_MODULES += $(LIB_MAPLE_HOME)/libraries/Wire

# call each module rules.mk
$(foreach m,$(LIBMAPLE_MODULES),$(eval $(call LIBMAPLE_MODULE_template,$(m))))


# Target upload commands
UPLOAD_ram   := $(SUPPORT_PATH)/scripts/reset.py && \
                sleep 1                  && \
                $(DFU) -a0 -d $(VENDOR_ID):$(PRODUCT_ID) -D $(PROJECT_BUILD_PATH)/$(BOARD).bin -R
UPLOAD_flash := $(SUPPORT_PATH)/scripts/reset.py && \
                sleep 1                  && \
                $(DFU) -a1 -d $(VENDOR_ID):$(PRODUCT_ID) -D $(PROJECT_BUILD_PATH)/$(BOARD).bin -R
UPLOAD_jtag  := $(OPENOCD) -f support/openocd/flash.cfg

# conditionally upload to whatever the last build was
install: INSTALL_TARGET = $(shell cat $(PROJECT_BUILD_PATH)/build-type 2>/dev/null)
install: $(PROJECT_BUILD_PATH)/$(BOARD).bin
	@echo Install target: $(INSTALL_TARGET)
	$(UPLOAD_$(INSTALL_TARGET))

# Force a rebuild if the maple target changed
PREV_BUILD_TYPE = $(shell cat $(PROJECT_BUILD_PATH)/build-type 2>/dev/null)
build-check:
ifneq ($(PREV_BUILD_TYPE), $(MEMORY_TARGET))
	$(shell rm -rf $(PROJECT_BUILD_PATH))
endif

all: MSG_INFO $(PROJECT_BUILD_PATH)/$(BOARD).bin
	
	cp $(PROJECT_BUILD_PATH)/$(BOARD).bin firmware/PreenFM_$(PCB_VERSION)_v$(PREENFM_VERSION_NUMBER).bin
	echo "dfu-util.exe -a1 -d 0x1EAF:0x0003 -D PreenFM_$(PCB_VERSION)_v$(PREENFM_VERSION_NUMBER).bin -R" > firmware/PreenFM_$(PCB_VERSION)_v$(PREENFM_VERSION_NUMBER).cmd
r3:
	@$(MAKE) PCB_VERSION=R3 all

r2:
	@$(MAKE) PCB_VERSION=R2 all

r1:
	@$(MAKE) PCB_VERSION=R2 all

clean:
	rm -rf build/*

help:
	@echo ""
	@echo "  PreenFM Makefile help"
	@echo "  ----------------------"
	@echo "  Compile targets :"
	@echo "      r3 (default) : for PCB R3b and R3c"
	@echo "      r2 (default) : for PCB R2"
	@echo "  "
	@echo "  Programming targets:"
	@echo "      install:  Upload code to PreenFM"
	@echo "  "
	@echo "  Other targets:"
	@echo "      clean: Remove all build and object files"
	@echo "      help: Show this message"
	@echo "  "

debug:
	$(OPENOCD) -f support/openocd/run.cfg

cscope:
	rm -rf *.cscope
	find . -name '*.[hcs]' -o -name '*.cpp' | xargs cscope -b

tags:
	etags `find . -name "*.c" -o -name "*.cpp" -o -name "*.h"`
	@echo "Made TAGS file for EMACS code browsing"

ctags:
	ctags-exuberant -R .
	@echo "Made tags file for VIM code browsing"

ram:
	@$(MAKE) MEMORY_TARGET=ram --no-print-directory sketch
	$(SILENT_CXX) $(CXX) $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

flash:
	@$(MAKE) MEMORY_TARGET=flash --no-print-directory sketch

jtag:
	@$(MAKE) MEMORY_TARGET=jtag --no-print-directory sketch


# main project target


build/Env.o: src/Env.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Osc.o: src/Osc.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/FM.o: src/FM.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Synth.o: src/Synth.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Voice.o: src/Voice.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Lfo.o: src/Lfo.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/LfoOsc.o: src/LfoOsc.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/LfoEnv.o: src/LfoEnv.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/LfoStepSeq.o: src/LfoStepSeq.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Matrix.o: src/Matrix.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/MidiDecoder.o: src/MidiDecoder.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/RingBuffer.o: src/RingBuffer.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Encoders.o: src/Encoders.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/FMDisplay.o: src/FMDisplay.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/SynthState.o: src/SynthState.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Menu.o: src/Menu.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/PresetUtil.o: src/PresetUtil.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/Presets.o: src/Presets.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

build/LiquidCrystal.o: src/LiquidCrystal.cpp
	$(CXX) -I/home/xhosxe/src $(CFLAGS) $(CXXFLAGS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 


$(PROJECT_BUILD_PATH)/$(BOARD).elf: $(OBJECTS) $(BUILDDIRS) 
	$(CXX) $(LDFLAGS) -o $@ $(LIBMAPLE_OBJECTS) $(OBJECTS)

$(PROJECT_BUILD_PATH)/$(BOARD).bin: $(PROJECT_BUILD_PATH)/$(BOARD).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) -v -Obinary $(PROJECT_BUILD_PATH)/$(BOARD).elf $@ 1>/dev/null
	$(SILENT_DISAS) $(DISAS) -d $(PROJECT_BUILD_PATH)/$(BOARD).elf > $(PROJECT_BUILD_PATH)/$(BOARD).disas
	@echo " "
	@echo "Object file sizes:"
	@find $(PROJECT_BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(PROJECT_BUILD_PATH)/$(BOARD).sizes
	@cat $(PROJECT_BUILD_PATH)/$(BOARD).sizes
	@echo " "
	@echo "Final Size:"
	@$(SIZE) $<
	@echo $(MEMORY_TARGET) > $(PROJECT_BUILD_PATH)/build-type

$(BUILDDIRS):
	@mkdir -p $@

MSG_INFO:
	@echo "================================================================================"
	@echo ""
	@echo "  Build info:"
	@echo "     BOARD:          " $(BOARD)
	@echo "     MCU:            " $(MCU)
	@echo "     MEMORY_TARGET:  " $(MEMORY_TARGET)
	@echo ""
	@echo "  See 'make help' for all possible targets"
	@echo ""
	@echo "================================================================================"
	@echo


