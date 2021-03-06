/*******************************************************************************
* File Name: LCD_Ready.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "LCD_Ready.h"

#if !defined(LCD_Ready_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: LCD_Ready_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 LCD_Ready_Read(void) 
{ 
    return LCD_Ready_Status;
}


/*******************************************************************************
* Function Name: LCD_Ready_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void LCD_Ready_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    LCD_Ready_Status_Aux_Ctrl |= LCD_Ready_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: LCD_Ready_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void LCD_Ready_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    LCD_Ready_Status_Aux_Ctrl &= (uint8)(~LCD_Ready_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: LCD_Ready_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void LCD_Ready_WriteMask(uint8 mask) 
{
    #if(LCD_Ready_INPUTS < 8u)
    	mask &= ((uint8)(1u << LCD_Ready_INPUTS) - 1u);
	#endif /* End LCD_Ready_INPUTS < 8u */
    LCD_Ready_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: LCD_Ready_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 LCD_Ready_ReadMask(void) 
{
    return LCD_Ready_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
