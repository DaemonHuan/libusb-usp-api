#ifndef __USB_GLOBAL_H__
#define __USB_GLOBAL_H__

typedef unsigned char us_byte;
typedef unsigned short us_short;

#define USB_G_CONF      0x01
#define USB_G_INTF      0x00

#define READ_BUS_TIMEOUT    (5 * 1000)//(3 * 60 * 1000)
#define READ_BUS_CONFIRM_TIMEOUT    (500)

#define TRANSFER_DELAY_MS 	500
#define TRANSFER_SPACE_MS 	100

#define BUFFER_MAX_LENGTH		512
#define BUFFER_MAX_DATA_LENGTH		(BUFFER_MAX_LENGTH - 16)
#define USP_FIX_PRINTDPI_PARA		(100 / 2.54)

///////////////////////////////////
#define IMAGE_FIXED_WIDTH 1008
#define IMAGE_FIXED_HEIGHT 2000
#define IMAGE_CODE_FIXED_HEIGHT 1000

#define ERR_USB_FIND_BUS		0xE000
#define ERR_USB_FIND_DEV		0xE001
#define ERR_USB_STATIC_READ			0xE002
#define ERR_USB_STATIC_WRITE		0xE003
#define ERR_USB_DONT_OPEN		0xE004
#define ERR_USB_SET_CONFIG		0xE005
#define ERR_USB_SET_CLAIM		0xE006
#define ERR_TRANS_ACK			0xE101 //
#define ERR_TRANS_DATA			0xE102 // crc check code
#define ERR_TRANS_NULLSTR		0xE103 // null data
#define ERR_TRANS_MISSONE		0xE104 // miss package
#define ERR_TRANS_TOOLONG		0xE105 // package is too long
#define ERR_TRANS_PARA			0xE106 // para is wrong
#define ERR_TRANS_NOTSEND		0xE107 // send unsuccessfully
#define ERR_TRANS_MALLOC		0xE108 // malloc unsuccessfully
#define ERR_TRANS_UNKNOW		0xE1FF // unknown error
#define ERR_IMAGE_SAVE		0xE200 // image do not save
#define ERR_BILLS_UNKNOW		0xE201 // bills processing error
#define ERR_JSON_FORMAT		0xE202 // json str format is wrong
#define ERR_JSON_PARA		0xE203 // json str para is wrong
#define ERR_JSON_FILE		0xE204 // json file not exist
#define ERR_DEVICE_BUSY         0xE205
#define ERR_UPDATE_FAILED		0xE206 // update failed

typedef enum {
    IMGNumRGB = 0x01,		// 1 票号彩色 01 02 03
    IMGNum_IGRAY,		// 2 票号红外灰度 04
    IMGFrontRGB,		// 3 整票正面彩色 05 06 07
    IMGFloorRGB,		// 4 整票背面彩色 08 09 10
    IMGFront_URGB,		// 5 整票正面紫外反射彩色 11 12 13
    IMGFloor_URGB,		// 6 整票反面紫外反射彩色 14 15 16
    IMG_UTRGB,		// 7 整票紫外透射彩色 17 18 19
    IMGFront_IGRAY,		// 8 整票红外反射正面灰度 20
    IMGFloor_IGRAY,		// 9 整票红外反射反面灰度 21
    IMG_ITGRAY		// 10 整票红外透射灰度 22
}IMAGETYPE;

typedef enum {
    IMG_F_BMP = 0x00,
    IMG_F_JPEG,
    IMG_F_TIFF
}IMAGE_FORMAT;

#endif // USBGLOBAL_H
