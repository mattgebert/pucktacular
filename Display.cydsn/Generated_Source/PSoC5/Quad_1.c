/*******************************************************************************
* File Name: Quad_1.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
*
* Note:
*  None.
*   
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Quad_1.h"

#if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)
    #include "Quad_1_PVT.h"
#endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT */

uint8 Quad_1_initVar = 0u;


/*******************************************************************************
* Function Name: Quad_1_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Quad_1_Init(void) 
{
    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(Quad_1_ISR_NUMBER);
        /* Set the ISR to point to the Quad_1_isr Interrupt. */
        (void) CyIntSetVector(Quad_1_ISR_NUMBER, & Quad_1_ISR);
        /* Set the priority. */
        CyIntSetPriority(Quad_1_ISR_NUMBER, Quad_1_ISR_PRIORITY);
    #endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Quad_1_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Quad_1_Enable(void) 
{
    uint8 enableInterrupts;

    Quad_1_SetInterruptMask(Quad_1_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) Quad_1_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    Quad_1_SR_AUX_CONTROL |= Quad_1_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(Quad_1_ISR_NUMBER);
    #endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Quad_1_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  Quad_1_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Quad_1_Start(void) 
{
    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT)
        Quad_1_Cnt8_Start();
        Quad_1_Cnt8_WriteCounter(Quad_1_COUNTER_INIT_VALUE);
    #else
        /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_16_BIT) || 
        *  (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT) 
        */
        Quad_1_Cnt16_Start();
        Quad_1_Cnt16_WriteCounter(Quad_1_COUNTER_INIT_VALUE);
    #endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT */
    
    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)        
       Quad_1_count32SoftPart = 0;
    #endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT */

    if (Quad_1_initVar == 0u)
    {
        Quad_1_Init();
        Quad_1_initVar = 1u;
    }

    Quad_1_Enable();
}


/*******************************************************************************
* Function Name: Quad_1_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Quad_1_Stop(void) 
{
    uint8 enableInterrupts;

    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT)
        Quad_1_Cnt8_Stop();
    #else 
        /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_16_BIT) ||
        *  (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)
        */
        Quad_1_Cnt16_Stop();    /* counter disable */
    #endif /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    Quad_1_SR_AUX_CONTROL &= (uint8) (~Quad_1_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)
        CyIntDisable(Quad_1_ISR_NUMBER);    /* interrupt disable */
    #endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Quad_1_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  Quad_1_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int16 Quad_1_GetCounter(void) 
{
    int16 count;
    uint16 tmpCnt;

    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(Quad_1_ISR_NUMBER);

        tmpCnt = Quad_1_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) Quad_1_COUNTER_INIT_VALUE);
        count = Quad_1_count32SoftPart + hwCount;

        CyIntEnable(Quad_1_ISR_NUMBER);
    #else 
        /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT) || 
        *  (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_16_BIT)
        */
        #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT)
            tmpCnt = Quad_1_Cnt8_ReadCounter();
        #else /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_16_BIT) */
            tmpCnt = Quad_1_Cnt16_ReadCounter();
        #endif  /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT */

        count = (int16) ((int32) tmpCnt -
                (int32) Quad_1_COUNTER_INIT_VALUE);

    #endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: Quad_1_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  Quad_1_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Quad_1_SetCounter(int16 value) 
{
    #if ((Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT) || \
         (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + Quad_1_COUNTER_INIT_VALUE;
        }
        else
        {
            count = Quad_1_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT)
            Quad_1_Cnt8_WriteCounter(count);
        #else /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_16_BIT) */
            Quad_1_Cnt16_WriteCounter(count);
        #endif  /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT */
    #else /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT) */
        CyIntDisable(Quad_1_ISR_NUMBER);

        Quad_1_Cnt16_WriteCounter(Quad_1_COUNTER_INIT_VALUE);
        Quad_1_count32SoftPart = value;

        CyIntEnable(Quad_1_ISR_NUMBER);
    #endif  /* (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_8_BIT) ||
             * (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: Quad_1_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 Quad_1_GetEvents(void) 
{
    return (Quad_1_STATUS_REG & Quad_1_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: Quad_1_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void Quad_1_SetInterruptMask(uint8 mask) 
{
    #if (Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (Quad_1_COUNTER_OVERFLOW | Quad_1_COUNTER_UNDERFLOW |
                 Quad_1_COUNTER_RESET);
    #endif /* Quad_1_COUNTER_SIZE == Quad_1_COUNTER_SIZE_32_BIT */

    Quad_1_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: Quad_1_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 Quad_1_GetInterruptMask(void) 
{
    return (Quad_1_STATUS_MASK & Quad_1_INIT_INT_MASK);
}


/* [] END OF FILE */
