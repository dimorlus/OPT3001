#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Source/FrameWork/Framework.c ../Source/USB/Usb_HID_tasks.c ../Source/USB/usb_descriptors.c ../Source/USB/usb_device.c ../Source/USB/usb_function_hid.c ../Source/BootLoader.c ../Source/NVMem.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/930953831/Framework.o ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o ${OBJECTDIR}/_ext/940238529/usb_descriptors.o ${OBJECTDIR}/_ext/940238529/usb_device.o ${OBJECTDIR}/_ext/940238529/usb_function_hid.o ${OBJECTDIR}/_ext/1728301206/BootLoader.o ${OBJECTDIR}/_ext/1728301206/NVMem.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/930953831/Framework.o.d ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o.d ${OBJECTDIR}/_ext/940238529/usb_descriptors.o.d ${OBJECTDIR}/_ext/940238529/usb_device.o.d ${OBJECTDIR}/_ext/940238529/usb_function_hid.o.d ${OBJECTDIR}/_ext/1728301206/BootLoader.o.d ${OBJECTDIR}/_ext/1728301206/NVMem.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/930953831/Framework.o ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o ${OBJECTDIR}/_ext/940238529/usb_descriptors.o ${OBJECTDIR}/_ext/940238529/usb_device.o ${OBJECTDIR}/_ext/940238529/usb_function_hid.o ${OBJECTDIR}/_ext/1728301206/BootLoader.o ${OBJECTDIR}/_ext/1728301206/NVMem.o

# Source Files
SOURCEFILES=../Source/FrameWork/Framework.c ../Source/USB/Usb_HID_tasks.c ../Source/USB/usb_descriptors.c ../Source/USB/usb_device.c ../Source/USB/usb_function_hid.c ../Source/BootLoader.c ../Source/NVMem.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128B
MP_LINKER_FILE_OPTION=,--script="..\linker_scripts\others\btl_32MX250F128B_generic.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/930953831/Framework.o: ../Source/FrameWork/Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/930953831 
	@${RM} ${OBJECTDIR}/_ext/930953831/Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/930953831/Framework.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/930953831/Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/930953831/Framework.o.d" -o ${OBJECTDIR}/_ext/930953831/Framework.o ../Source/FrameWork/Framework.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o: ../Source/USB/Usb_HID_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o.d" -o ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o ../Source/USB/Usb_HID_tasks.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/usb_descriptors.o: ../Source/USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/940238529/usb_descriptors.o ../Source/USB/usb_descriptors.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/usb_device.o: ../Source/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/usb_device.o.d" -o ${OBJECTDIR}/_ext/940238529/usb_device.o ../Source/USB/usb_device.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/usb_function_hid.o: ../Source/USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/940238529/usb_function_hid.o ../Source/USB/usb_function_hid.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/1728301206/BootLoader.o: ../Source/BootLoader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728301206 
	@${RM} ${OBJECTDIR}/_ext/1728301206/BootLoader.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/BootLoader.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728301206/BootLoader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/1728301206/BootLoader.o.d" -o ${OBJECTDIR}/_ext/1728301206/BootLoader.o ../Source/BootLoader.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/1728301206/NVMem.o: ../Source/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728301206 
	@${RM} ${OBJECTDIR}/_ext/1728301206/NVMem.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/NVMem.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728301206/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/1728301206/NVMem.o.d" -o ${OBJECTDIR}/_ext/1728301206/NVMem.o ../Source/NVMem.c    -D_SUPPRESS_PLIB_WARNING
	
else
${OBJECTDIR}/_ext/930953831/Framework.o: ../Source/FrameWork/Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/930953831 
	@${RM} ${OBJECTDIR}/_ext/930953831/Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/930953831/Framework.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/930953831/Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/930953831/Framework.o.d" -o ${OBJECTDIR}/_ext/930953831/Framework.o ../Source/FrameWork/Framework.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o: ../Source/USB/Usb_HID_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o.d" -o ${OBJECTDIR}/_ext/940238529/Usb_HID_tasks.o ../Source/USB/Usb_HID_tasks.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/usb_descriptors.o: ../Source/USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/940238529/usb_descriptors.o ../Source/USB/usb_descriptors.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/usb_device.o: ../Source/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/usb_device.o.d" -o ${OBJECTDIR}/_ext/940238529/usb_device.o ../Source/USB/usb_device.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/940238529/usb_function_hid.o: ../Source/USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/940238529 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/940238529/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/940238529/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/940238529/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/940238529/usb_function_hid.o ../Source/USB/usb_function_hid.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/1728301206/BootLoader.o: ../Source/BootLoader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728301206 
	@${RM} ${OBJECTDIR}/_ext/1728301206/BootLoader.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/BootLoader.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728301206/BootLoader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/1728301206/BootLoader.o.d" -o ${OBJECTDIR}/_ext/1728301206/BootLoader.o ../Source/BootLoader.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/_ext/1728301206/NVMem.o: ../Source/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728301206 
	@${RM} ${OBJECTDIR}/_ext/1728301206/NVMem.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/NVMem.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728301206/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mips16 -Os -DTRANSPORT_LAYER_USB -DDEMO_BOARD_USB_STARTER_KIT -I"../" -I"../Include" -I"../Include/Usb/Stack" -I"../Include/FrameWork" -I"../Include/Usb" -I"../Include/HardwareProfile" -I"../Include/USB" -I"../Include/Framework" -MMD -MF "${OBJECTDIR}/_ext/1728301206/NVMem.o.d" -o ${OBJECTDIR}/_ext/1728301206/NVMem.o ../Source/NVMem.c    -D_SUPPRESS_PLIB_WARNING
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../linker_scripts/others/btl_32MX250F128B_generic.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PIC32MXSK=1 -mprocessor=$(MP_PROCESSOR_OPTION) -mips16 -o dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -D_SUPPRESS_PLIB_WARNING     -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1,--gc-sections,-L".",-Map="$(BINDIR_)$(TARGETBASE).map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../linker_scripts/others/btl_32MX250F128B_generic.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -mips16 -o dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -D_SUPPRESS_PLIB_WARNING -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--gc-sections,-L".",-Map="$(BINDIR_)$(TARGETBASE).map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/USB_HID_Btl_OPT.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
