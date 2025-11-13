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
FINAL_IMAGE=${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c adc.c iic.c usb_descriptors.c Microchip/USB/usb_device.c Microchip/USB/usb_function_hid.c SSD1306.c gfx.c font.c ee_als.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/adc.o ${OBJECTDIR}/iic.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/Microchip/USB/usb_device.o ${OBJECTDIR}/Microchip/USB/usb_function_hid.o ${OBJECTDIR}/SSD1306.o ${OBJECTDIR}/gfx.o ${OBJECTDIR}/font.o ${OBJECTDIR}/ee_als.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/adc.o.d ${OBJECTDIR}/iic.o.d ${OBJECTDIR}/usb_descriptors.o.d ${OBJECTDIR}/Microchip/USB/usb_device.o.d ${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d ${OBJECTDIR}/SSD1306.o.d ${OBJECTDIR}/gfx.o.d ${OBJECTDIR}/font.o.d ${OBJECTDIR}/ee_als.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/adc.o ${OBJECTDIR}/iic.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/Microchip/USB/usb_device.o ${OBJECTDIR}/Microchip/USB/usb_function_hid.o ${OBJECTDIR}/SSD1306.o ${OBJECTDIR}/gfx.o ${OBJECTDIR}/font.o ${OBJECTDIR}/ee_als.o

# Source Files
SOURCEFILES=main.c adc.c iic.c usb_descriptors.c Microchip/USB/usb_device.c Microchip/USB/usb_function_hid.c SSD1306.c gfx.c font.c ee_als.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128B
MP_LINKER_FILE_OPTION=,--script="app_32MX250F128D.ld"
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
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/6c24c28b0e1a39d66a487d290390a73e15bcdabd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/27a426327450c5e62193d0542281b2982acf5e04 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/adc.o.d" -o ${OBJECTDIR}/adc.o adc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/iic.o: iic.c  .generated_files/flags/default/dcf258bca5f02c35179c49468f3c767a8b04b6a3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/iic.o.d 
	@${RM} ${OBJECTDIR}/iic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/iic.o.d" -o ${OBJECTDIR}/iic.o iic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  .generated_files/flags/default/72bfa8ed5d73eb35eb7a882406234e04f3fe706 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_device.o: Microchip/USB/usb_device.c  .generated_files/flags/default/8e08b2d4751d751b92e504487cce0b8d9e5b9673 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_device.o Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_function_hid.o: Microchip/USB/usb_function_hid.c  .generated_files/flags/default/47a7471c735423f99b6e80984d9e1d629e065a5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_function_hid.o Microchip/USB/usb_function_hid.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/SSD1306.o: SSD1306.c  .generated_files/flags/default/132da91a7942e37e771f13b2321a01002e7ec16e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SSD1306.o.d 
	@${RM} ${OBJECTDIR}/SSD1306.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/SSD1306.o.d" -o ${OBJECTDIR}/SSD1306.o SSD1306.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/gfx.o: gfx.c  .generated_files/flags/default/23f37144cbb87b577cb46a60d03e3794426d55a2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gfx.o.d 
	@${RM} ${OBJECTDIR}/gfx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/gfx.o.d" -o ${OBJECTDIR}/gfx.o gfx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/font.o: font.c  .generated_files/flags/default/1f45dfc78a523a3eb275307dbd56b88464ce0f6f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/ee_als.o: ee_als.c  .generated_files/flags/default/69cc7a945bee86f36fe3b437e6ec505fabf97694 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ee_als.o.d 
	@${RM} ${OBJECTDIR}/ee_als.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/ee_als.o.d" -o ${OBJECTDIR}/ee_als.o ee_als.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/4c19aa27c41e87f6a142034c5bead4da84e688dc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/28be8114e7f73a565a0f4e11d6e8066a54dafeec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/adc.o.d" -o ${OBJECTDIR}/adc.o adc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/iic.o: iic.c  .generated_files/flags/default/5492da91b1402cb3f6374cd8a9a2c6f488f8b8a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/iic.o.d 
	@${RM} ${OBJECTDIR}/iic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/iic.o.d" -o ${OBJECTDIR}/iic.o iic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  .generated_files/flags/default/31e8bb6961fd39d31d74bcd8fbe8c5759821a5e0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_device.o: Microchip/USB/usb_device.c  .generated_files/flags/default/e07d03971cdce037b3f85362f827cc18401d46c1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_device.o Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_function_hid.o: Microchip/USB/usb_function_hid.c  .generated_files/flags/default/a87a01184537385e5235f9b598cafe1bdd0cc559 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_function_hid.o Microchip/USB/usb_function_hid.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/SSD1306.o: SSD1306.c  .generated_files/flags/default/b10cfe4bbeca5a996732be3115a05df3e0c99481 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SSD1306.o.d 
	@${RM} ${OBJECTDIR}/SSD1306.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/SSD1306.o.d" -o ${OBJECTDIR}/SSD1306.o SSD1306.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/gfx.o: gfx.c  .generated_files/flags/default/2678a4b0518b59ba4cee8ad5ae982e66b65f39e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gfx.o.d 
	@${RM} ${OBJECTDIR}/gfx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/gfx.o.d" -o ${OBJECTDIR}/gfx.o gfx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/font.o: font.c  .generated_files/flags/default/dd3a90a550992fdc56eb54624ab79e166826b577 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/ee_als.o: ee_als.c  .generated_files/flags/default/d27683b15f7df06e251e5275848ceef6de7179d2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ee_als.o.d 
	@${RM} ${OBJECTDIR}/ee_als.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -MP -MMD -MF "${OBJECTDIR}/ee_als.o.d" -o ${OBJECTDIR}/ee_als.o ee_als.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    app_32MX250F128D.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o ${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    -D_SUPPRESS_PLIB_WARNING $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s 
	
else
${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   app_32MX250F128D.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o ${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    -D_SUPPRESS_PLIB_WARNING $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s 
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
