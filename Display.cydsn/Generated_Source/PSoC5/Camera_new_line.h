/*******************************************************************************
* File Name: Camera_new_line.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Camera_new_line_H)
#define CY_ISR_Camera_new_line_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Camera_new_line_Start(void);
void Camera_new_line_StartEx(cyisraddress address);
void Camera_new_line_Stop(void);

CY_ISR_PROTO(Camera_new_line_Interrupt);

void Camera_new_line_SetVector(cyisraddress address);
cyisraddress Camera_new_line_GetVector(void);

void Camera_new_line_SetPriority(uint8 priority);
uint8 Camera_new_line_GetPriority(void);

void Camera_new_line_Enable(void);
uint8 Camera_new_line_GetState(void);
void Camera_new_line_Disable(void);

void Camera_new_line_SetPending(void);
void Camera_new_line_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Camera_new_line ISR. */
#define Camera_new_line_INTC_VECTOR            ((reg32 *) Camera_new_line__INTC_VECT)

/* Address of the Camera_new_line ISR priority. */
#define Camera_new_line_INTC_PRIOR             ((reg8 *) Camera_new_line__INTC_PRIOR_REG)

/* Priority of the Camera_new_line interrupt. */
#define Camera_new_line_INTC_PRIOR_NUMBER      Camera_new_line__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Camera_new_line interrupt. */
#define Camera_new_line_INTC_SET_EN            ((reg32 *) Camera_new_line__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Camera_new_line interrupt. */
#define Camera_new_line_INTC_CLR_EN            ((reg32 *) Camera_new_line__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Camera_new_line interrupt state to pending. */
#define Camera_new_line_INTC_SET_PD            ((reg32 *) Camera_new_line__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Camera_new_line interrupt. */
#define Camera_new_line_INTC_CLR_PD            ((reg32 *) Camera_new_line__INTC_CLR_PD_REG)


#endif /* CY_ISR_Camera_new_line_H */


/* [] END OF FILE */
