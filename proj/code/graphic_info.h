#ifndef __GRAPHIC_INFO_H
#define __GRAPHIC_INFO_H

#include <stdint.h>
#include <minix/syslib.h>
#include <sys/types.h>
#include "lmlib.h"

// VbeInfoBlock struct

typedef struct {

char VbeSignature[4];			//VBE Signature
uint16_t VbeVersion;			//VBE version
phys_bytes OemStringPtr;		//Pointer to OEM String
uint32_t Capabilities;			//Capabilities of graphics controller
phys_bytes VideoModePtr;		//Pointer to VideoModeList
uint16_t TotalMemory;			//Number of 64kb memory blocks (Added for VBE 2.0)
uint16_t OemSoftwareRev;	 	//VBE implementation Software revision
phys_bytes OemVendorNamePtr;	//Pointer to Vendor Name String
phys_bytes OemProductNamePtr;	//Pointer to Product Name String
phys_bytes OemProductRevPtr;	//Pointer to Product Revision String
uint8_t Reserved[222];			//Reserved for VBE implementation scratch area
uint8_t OemData[256];			//Data Area for OEM Strings

} __attribute__((packed)) vbe_info_block;

#define BIOS_INTERFACE 0x10
#define VBE_FUNCTION_CODE 0x4F /*Para o registro AH*/
#define SET_BIT_14 (1 << 14)
#define TEXT_MODE_CODE 0x03

//Function status checks

#define VBE_FUNCTION_SUPPORTED 0x4F
#define VBE_FUNCTION_SUCCESSFULL 0x00
#define VBE_FUNCTION_CALL_FAILED 0x01
#define VBE_FUNCTION_NOT_SUPPORTED 0x02
#define VBE_FUNCTION_VM_NOT_VALID 0x03

//Screen Resolutions

#define RES_640x480 0x101
#define RES_800x600 0x103
#define RES_1024x768 0x105
#define RES_1280x1024 0x107

//VBE Functions

#define VBE_RETURN_CONTROLLER_INFO 0x00
#define VBE_RETURN_MODE_INFO 0x01
#define VBE_SET_MODE 0x02
#define VBE_SIGNATURE_2 "VBE2"
#define VBE_SIGNATURE_VESA "VESA"
#define END_OF_VIDEO_MODES_PTR 0x0FFFF

#endif
