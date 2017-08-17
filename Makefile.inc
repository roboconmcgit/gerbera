mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += balancer.o balancer_param.o

APPL_CXXOBJS += \
	lcdDraw.o \
	logging.o \
	ClockTimer.o \
	MotorParts.o \
	GyroParts.o \
	ColorParts.o \
	SonarParts.o \
	TouchParts.o \
	calibration.o \
	Cruise.o \
	Controller.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += \
	$(mkfile_path)utility \
	$(mkfile_path)calibration \
	$(mkfile_path)controller \
	$(mkfile_path)controller/cruise \
	$(mkfile_path)controller/difficult \
	$(mkfile_path)parts

INCLUDES += \
	-I$(mkfile_path)utility \
	-I$(mkfile_path)calibration \
	-I$(mkfile_path)controller \
	-I$(mkfile_path)controller/cruise \
	-I$(mkfile_path)controller/difficult \
	-I$(mkfile_path)parts

#COPTS += -fno-use-cxa-atexit
COPTS +=
