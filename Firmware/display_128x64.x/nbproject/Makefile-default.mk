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
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/29625a9a8d84a0a2e23588624679b6620949c3ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/15e85d626c64881851ef319fddfeb16ada630a5e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/adc.o.d" -o ${OBJECTDIR}/adc.o adc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/iic.o: iic.c  .generated_files/flags/default/89936703821222be1e0dea1921216891b2da8029 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/iic.o.d 
	@${RM} ${OBJECTDIR}/iic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/iic.o.d" -o ${OBJECTDIR}/iic.o iic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  .generated_files/flags/default/2958307a99364f2459a79f7df53e2265873dcaec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_device.o: Microchip/USB/usb_device.c  .generated_files/flags/default/3dd99394c6351620127c8906dfe065e12b99958f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_device.o Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_function_hid.o: Microchip/USB/usb_function_hid.c  .generated_files/flags/default/d9655c799cab8824b06080ac6cc0e0590eece077 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_function_hid.o Microchip/USB/usb_function_hid.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/SSD1306.o: SSD1306.c  .generated_files/flags/default/208808bb5c1ef423ae15863885fcd38100955a27 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SSD1306.o.d 
	@${RM} ${OBJECTDIR}/SSD1306.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/SSD1306.o.d" -o ${OBJECTDIR}/SSD1306.o SSD1306.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/gfx.o: gfx.c  .generated_files/flags/default/44efcfcf01d3081be12215ed1fb99eb33ed010ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gfx.o.d 
	@${RM} ${OBJECTDIR}/gfx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/gfx.o.d" -o ${OBJECTDIR}/gfx.o gfx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/font.o: font.c  .generated_files/flags/default/57cc9738cae6d8268310ab6cd2288c3a967743ff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/ee_als.o: ee_als.c  .generated_files/flags/default/342ef62016e427ba6bd650d1743cccbe20a26a4b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ee_als.o.d 
	@${RM} ${OBJECTDIR}/ee_als.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/ee_als.o.d" -o ${OBJECTDIR}/ee_als.o ee_als.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/be43f204db8a9ec4e38c597a70dc815131e18f78 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/396487383fd0c516281b55811648db10fc9dfa11 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/adc.o.d" -o ${OBJECTDIR}/adc.o adc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/iic.o: iic.c  .generated_files/flags/default/caf8cc7936ffc6924a8d50f9cf6f7be943138aa8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/iic.o.d 
	@${RM} ${OBJECTDIR}/iic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/iic.o.d" -o ${OBJECTDIR}/iic.o iic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  .generated_files/flags/default/afee786415d486bc8aadfc1bc008426bf232095e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_device.o: Microchip/USB/usb_device.c  .generated_files/flags/default/4dd7c4728adb29c702971bfef520367afe6b2bd1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_device.o Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/Microchip/USB/usb_function_hid.o: Microchip/USB/usb_function_hid.c  .generated_files/flags/default/877a75069112022a54f73dae9c912a5cef0c01b7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Microchip/USB" 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/Microchip/USB/usb_function_hid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/Microchip/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/Microchip/USB/usb_function_hid.o Microchip/USB/usb_function_hid.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/SSD1306.o: SSD1306.c  .generated_files/flags/default/9cb4e9d1e91af69c048b501e15aeb23c8375c2e2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SSD1306.o.d 
	@${RM} ${OBJECTDIR}/SSD1306.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/SSD1306.o.d" -o ${OBJECTDIR}/SSD1306.o SSD1306.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/gfx.o: gfx.c  .generated_files/flags/default/228283c7d7335f438ed415498e9b9f0bf420fc1f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gfx.o.d 
	@${RM} ${OBJECTDIR}/gfx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/gfx.o.d" -o ${OBJECTDIR}/gfx.o gfx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/font.o: font.c  .generated_files/flags/default/a96a85d81e7cf540ee59c905f543e736e15eb55a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
${OBJECTDIR}/ee_als.o: ee_als.c  .generated_files/flags/default/eb61f7aff3d1cadc425ed9694a3905710bebb342 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ee_als.o.d 
	@${RM} ${OBJECTDIR}/ee_als.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -I"." -I".." -I"Microchip/Include" -I"Microchip/Include/USB" -I"Microchip/USB" -I"../pic32mx/include" -MP -MMD -MF "${OBJECTDIR}/ee_als.o.d" -o ${OBJECTDIR}/ee_als.o ee_als.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -D_SUPPRESS_PLIB_WARNING   
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -o ${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    -D_SUPPRESS_PLIB_WARNING $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml 
	
else
${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   app_32MX250F128D.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -o ${DISTDIR}/display_128x64.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    -D_SUPPRESS_PLIB_WARNING $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml 
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
