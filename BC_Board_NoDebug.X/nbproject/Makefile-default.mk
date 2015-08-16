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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/AFE_Driver/AFE.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/BC_Application.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/PL_PairedList.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Button/BN_ButtonDetection.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Display/DBUF_DisplayBuffer.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Handler.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Rx.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Tx.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/I2CM.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/LCDM_St7032.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/MDLL_DataLinkLayer.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/RX_Manchester.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/TX_Manchester.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Touch/TD_CvdHandler.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/459576982/AFE.p1 ${OBJECTDIR}/_ext/1732390931/BC_Application.p1 ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1 ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1 ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1 ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1 ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1 ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1 ${OBJECTDIR}/_ext/1095495242/I2CM.p1 ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1 ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1 ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1 ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1 ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1 ${OBJECTDIR}/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/459576982/AFE.p1.d ${OBJECTDIR}/_ext/1732390931/BC_Application.p1.d ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1.d ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1.d ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1.d ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1.d ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1.d ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1.d ${OBJECTDIR}/_ext/1095495242/I2CM.p1.d ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1.d ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1.d ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1.d ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1.d ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1.d ${OBJECTDIR}/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/459576982/AFE.p1 ${OBJECTDIR}/_ext/1732390931/BC_Application.p1 ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1 ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1 ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1 ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1 ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1 ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1 ${OBJECTDIR}/_ext/1095495242/I2CM.p1 ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1 ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1 ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1 ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1 ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1 ${OBJECTDIR}/main.p1

# Source Files
SOURCEFILES=C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/AFE_Driver/AFE.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/BC_Application.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/PL_PairedList.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Button/BN_ButtonDetection.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Display/DBUF_DisplayBuffer.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Handler.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Rx.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Tx.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/I2CM.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/LCDM_St7032.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/MDLL_DataLinkLayer.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/RX_Manchester.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/TX_Manchester.c C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Touch/TD_CvdHandler.c main.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16LF1829
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/459576982/AFE.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/AFE_Driver/AFE.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/459576982" 
	@${RM} ${OBJECTDIR}/_ext/459576982/AFE.p1.d 
	@${RM} ${OBJECTDIR}/_ext/459576982/AFE.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/459576982/AFE.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/AFE_Driver/AFE.c 
	@-${MV} ${OBJECTDIR}/_ext/459576982/AFE.d ${OBJECTDIR}/_ext/459576982/AFE.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/459576982/AFE.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1732390931/BC_Application.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/BC_Application.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1732390931" 
	@${RM} ${OBJECTDIR}/_ext/1732390931/BC_Application.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1732390931/BC_Application.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1732390931/BC_Application.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/BC_Application.c 
	@-${MV} ${OBJECTDIR}/_ext/1732390931/BC_Application.d ${OBJECTDIR}/_ext/1732390931/BC_Application.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1732390931/BC_Application.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/PL_PairedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1732390931" 
	@${RM} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/PL_PairedList.c 
	@-${MV} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.d ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Button/BN_ButtonDetection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1796298347" 
	@${RM} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Button/BN_ButtonDetection.c 
	@-${MV} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.d ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Display/DBUF_DisplayBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1579737791" 
	@${RM} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Display/DBUF_DisplayBuffer.c 
	@-${MV} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.d ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1740996605/PC_Handler.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1740996605" 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1740996605/PC_Handler.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Handler.c 
	@-${MV} ${OBJECTDIR}/_ext/1740996605/PC_Handler.d ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1740996605/PC_Rx.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Rx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1740996605" 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1740996605/PC_Rx.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Rx.c 
	@-${MV} ${OBJECTDIR}/_ext/1740996605/PC_Rx.d ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1740996605/PC_Tx.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Tx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1740996605" 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1740996605/PC_Tx.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Tx.c 
	@-${MV} ${OBJECTDIR}/_ext/1740996605/PC_Tx.d ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1095495242/I2CM.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/I2CM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1095495242" 
	@${RM} ${OBJECTDIR}/_ext/1095495242/I2CM.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1095495242/I2CM.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1095495242/I2CM.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/I2CM.c 
	@-${MV} ${OBJECTDIR}/_ext/1095495242/I2CM.d ${OBJECTDIR}/_ext/1095495242/I2CM.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1095495242/I2CM.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/LCDM_St7032.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1095495242" 
	@${RM} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/LCDM_St7032.c 
	@-${MV} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.d ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/MDLL_DataLinkLayer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1364635209" 
	@${RM} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/MDLL_DataLinkLayer.c 
	@-${MV} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.d ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/RX_Manchester.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1364635209" 
	@${RM} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/RX_Manchester.c 
	@-${MV} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.d ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/TX_Manchester.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1364635209" 
	@${RM} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/TX_Manchester.c 
	@-${MV} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.d ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Touch/TD_CvdHandler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/512689276" 
	@${RM} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1.d 
	@${RM} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Touch/TD_CvdHandler.c 
	@-${MV} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.d ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/main.p1  main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/459576982/AFE.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/AFE_Driver/AFE.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/459576982" 
	@${RM} ${OBJECTDIR}/_ext/459576982/AFE.p1.d 
	@${RM} ${OBJECTDIR}/_ext/459576982/AFE.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/459576982/AFE.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/AFE_Driver/AFE.c 
	@-${MV} ${OBJECTDIR}/_ext/459576982/AFE.d ${OBJECTDIR}/_ext/459576982/AFE.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/459576982/AFE.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1732390931/BC_Application.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/BC_Application.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1732390931" 
	@${RM} ${OBJECTDIR}/_ext/1732390931/BC_Application.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1732390931/BC_Application.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1732390931/BC_Application.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/BC_Application.c 
	@-${MV} ${OBJECTDIR}/_ext/1732390931/BC_Application.d ${OBJECTDIR}/_ext/1732390931/BC_Application.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1732390931/BC_Application.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/PL_PairedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1732390931" 
	@${RM} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/BodyCom_Application/PL_PairedList.c 
	@-${MV} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.d ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1732390931/PL_PairedList.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Button/BN_ButtonDetection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1796298347" 
	@${RM} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Button/BN_ButtonDetection.c 
	@-${MV} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.d ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1796298347/BN_ButtonDetection.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Display/DBUF_DisplayBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1579737791" 
	@${RM} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Display/DBUF_DisplayBuffer.c 
	@-${MV} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.d ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1579737791/DBUF_DisplayBuffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1740996605/PC_Handler.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1740996605" 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1740996605/PC_Handler.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Handler.c 
	@-${MV} ${OBJECTDIR}/_ext/1740996605/PC_Handler.d ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1740996605/PC_Handler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1740996605/PC_Rx.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Rx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1740996605" 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1740996605/PC_Rx.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Rx.c 
	@-${MV} ${OBJECTDIR}/_ext/1740996605/PC_Rx.d ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1740996605/PC_Rx.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1740996605/PC_Tx.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Tx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1740996605" 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1740996605/PC_Tx.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/EUSART/PC_Tx.c 
	@-${MV} ${OBJECTDIR}/_ext/1740996605/PC_Tx.d ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1740996605/PC_Tx.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1095495242/I2CM.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/I2CM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1095495242" 
	@${RM} ${OBJECTDIR}/_ext/1095495242/I2CM.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1095495242/I2CM.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1095495242/I2CM.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/I2CM.c 
	@-${MV} ${OBJECTDIR}/_ext/1095495242/I2CM.d ${OBJECTDIR}/_ext/1095495242/I2CM.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1095495242/I2CM.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/LCDM_St7032.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1095495242" 
	@${RM} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/LCD/LCDM_St7032.c 
	@-${MV} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.d ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1095495242/LCDM_St7032.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/MDLL_DataLinkLayer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1364635209" 
	@${RM} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/MDLL_DataLinkLayer.c 
	@-${MV} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.d ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1364635209/MDLL_DataLinkLayer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/RX_Manchester.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1364635209" 
	@${RM} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/RX_Manchester.c 
	@-${MV} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.d ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1364635209/RX_Manchester.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/TX_Manchester.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1364635209" 
	@${RM} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Manchester/TX_Manchester.c 
	@-${MV} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.d ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1364635209/TX_Manchester.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1: C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Touch/TD_CvdHandler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/512689276" 
	@${RM} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1.d 
	@${RM} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1  C:/Users/tazevedo/Documents/BodyCom/BC_Board_NoDebug/Touch/TD_CvdHandler.c 
	@-${MV} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.d ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/512689276/TD_CvdHandler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/main.p1  main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.map  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"        -odist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.map  --double=24 --float=24 --rom=default,-1E20-1FFF --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DBASE -P -N63 -I"../AFE_Driver" -I"../BodyCom_Application" -I"../Button" -I"../Display" -I"../Encryption" -I"../EUSART" -I"../LCD" -I"../Manchester" -I"../Touch" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -odist/${CND_CONF}/${IMAGE_TYPE}/BC_Board_NoDebug.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
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
