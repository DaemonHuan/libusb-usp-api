/*
 * libusp_bpm22.cpp
 *
 *  Created on: 2020年5月22日
 *      Author: JK
 */
#include "libusp_bpm22.h"

#include "usbBasic/usbbasic.h"
#include "cDebug-log/cdebug.h"
#include "cjson/cJSON_Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define R_OK 4 /* Test for read permission. */
#define W_OK 2 /* Test for write permission. */
#define X_OK 1 /* Test for execute permission. */
#define F_OK 0 /* Test for existence. */

#include "freeimage3180/FreeImage.h"
#define MY_VID 0x14D6
#define MY_PID 0x2022

#define DATA_P_LENGTH   3
#define DATA_P_START    12
#define DATA_P_FLAG     10
#define DATA_MIN_LENGTH 18

#define LOCAL_LOG_PATH "./"
#define LOCAL_LOG_FILENAME "bpm22"

static int _st_flag = 0;
#define M_MAKE_SFLAG_W(_aa) ((0x0100 == _aa) ? ((_aa = 0x00) ++) : (_aa ++))
#define M_MAKE_SFLAG M_MAKE_SFLAG_W(_st_flag)

int execSaveImage(const char *tFilePath, const IMAGE_FORMAT &iFormate, int tTypeCode0,
    int tTypeCode1, int tTypeCode2);
int execCreateImage(const char *filePath, int type, int iWidth, int iHeight, const char *byteBinary,
    int byteLength, const IMAGE_FORMAT &iFormate);
int execFormatImage(const char* srcImage, const char* tarImage, int angle, float proportion, int thresh,
    int &gWidth, int &gHeight);
int execCmdTemplate(us_short i_fi, us_byte *iData, int iLength);
int execGetTemplate(us_short i_fi, us_byte *iData, int iLength, void *o_data, int &o_len);

int execSaveImage(const char *tFilePath, const IMAGE_FORMAT &iFormate,
                  int tTypeCode0, int tTypeCode1, int tTypeCode2)
{
    int i_Flag = M_MAKE_SFLAG;
    us_byte *tData = (us_byte*)malloc(6 * 1024 * 1024 * sizeof(us_byte));
    memset(tData, 0x00, 6 * 1024 * 1024 * sizeof(char));
    int tDataLength = 0;

    // ** -- ** -- ** -- ** -- ** -- ** -- ** -- **
    if (0 != tTypeCode0) {
        char *tImageData = (char*)malloc(2 * 1024 * 1024 * sizeof(char));
        memset(tImageData, 0x00, 2 * 1024 * 1024 * sizeof(us_byte));
        int tLength = 0;

//        execSaveData(tImageData, tLength, tTypeCode0);
        execSaveRGBData(tImageData, tLength, tTypeCode0, i_Flag);

        memcpy(tData + tDataLength, tImageData, tLength);
        tDataLength = tDataLength + tLength;
        free(tImageData);
    }

    // ** -- ** -- ** -- ** -- ** -- ** -- ** -- **
    if (0 != tTypeCode1) {
        char *tImageData = (char*)malloc(2 * 1024 * 1024 * sizeof(char));
        memset(tImageData, 0x00, 2 * 1024 * 1024 * sizeof(us_byte));
        int tLength = 0;

//        execSaveData(tImageData, tLength, tTypeCode1);
        execSaveRGBData(tImageData, tLength, tTypeCode1, i_Flag);

        memcpy(tData + tDataLength, tImageData, tLength);
        tDataLength = tDataLength + tLength;
        free(tImageData);
    }

    // ** -- ** -- ** -- ** -- ** -- ** -- ** -- **
    if (0 != tTypeCode2) {
        char *tImageData = (char*)malloc(2 * 1024 * 1024 * sizeof(char));
        memset(tImageData, 0x00, 2 * 1024 * 1024 * sizeof(us_byte));
        int tLength = 0;

//        execSaveData(tImageData, tLength, tTypeCode2);
        execSaveRGBData(tImageData, tLength, tTypeCode2, i_Flag);

        memcpy(tData + tDataLength, tImageData, tLength);
        tDataLength = tDataLength + tLength;
        free(tImageData);
    }

    // ** -- ** -- ** -- ** -- ** -- ** -- ** -- **
    int iResult = 0;
    if ((tTypeCode0 == 0x01) && (tTypeCode1 == 0x02) && (tTypeCode2 == 0x03))
        iResult = execCreateImage(tFilePath, 0x00, IMAGE_FIXED_WIDTH, IMAGE_CODE_FIXED_HEIGHT,
        (char*)tData, tDataLength, iFormate);
    else if ((tTypeCode0 == 0x04) && (tTypeCode1 == 0x00) && (tTypeCode2 == 0x00))
        iResult = execCreateImage(tFilePath, 0x01, IMAGE_FIXED_WIDTH, IMAGE_CODE_FIXED_HEIGHT,
        (char*)tData, tDataLength, iFormate);
    else if ((tTypeCode0 == 0x14) || (tTypeCode1 == 0x00) || (tTypeCode2 == 0x00))
        iResult = execCreateImage(tFilePath, 0x01, IMAGE_FIXED_WIDTH, IMAGE_FIXED_HEIGHT,
        (char*)tData, tDataLength, iFormate);
    else if ((tTypeCode0 == 0x15) || (tTypeCode1 == 0x00) || (tTypeCode2 == 0x00))
        iResult = execCreateImage(tFilePath, 0x01, IMAGE_FIXED_WIDTH, IMAGE_FIXED_HEIGHT,
        (char*)tData, tDataLength, iFormate);
    else if ((tTypeCode0 == 0x16) || (tTypeCode1 == 0x00) || (tTypeCode2 == 0x00))
        iResult = execCreateImage(tFilePath, 0x01, IMAGE_FIXED_WIDTH, IMAGE_FIXED_HEIGHT,
        (char*)tData, tDataLength, iFormate);
    else
        iResult = execCreateImage(tFilePath, 0x00, IMAGE_FIXED_WIDTH, IMAGE_FIXED_HEIGHT,
        (char*)tData, tDataLength, iFormate);

    free(tData);
    return iResult;
}

int execCreateImage(const char *filePath, int type, int iWidth, int iHeight, const char *byteBinary,
                    int byteLength, const IMAGE_FORMAT &iFormate)
{
    long sizeImage = iWidth * iHeight * 3;

    LOG_INFO("execCreateImage .. filePath= %s byteLength= %d type= %d", filePath, byteLength, type);

    if (0x00 == type) {
        if (byteLength != sizeImage) return ERR_IMAGE_SAVE;
    }
    else if (0x01 == type) {
        if (byteLength != sizeImage / 3)  return ERR_IMAGE_SAVE;
    }
    else return ERR_IMAGE_SAVE;

    //
    FREE_IMAGE_FORMAT ifif = FIF_UNKNOWN;
    if (iFormate == IMG_F_BMP) ifif = FIF_BMP;
    if (iFormate == IMG_F_JPEG) ifif = FIF_JPEG;
    if (iFormate == IMG_F_TIFF) ifif = FIF_TIFF;

    // FreeImage
    FreeImage_Initialise(TRUE);

    FIBITMAP *newImage = FreeImage_Allocate(iWidth, iHeight, 24);

    FreeImage_SetDotsPerMeterX(newImage, 200 * 100 / 2.54);
    FreeImage_SetDotsPerMeterY(newImage, 200 * 100 / 2.54);

    long tCount = iWidth * iHeight, iCount = 0;
    for (int t_i = iHeight; t_i >= 0; t_i--) {
        for (int t_j = 0; t_j < iWidth; t_j++) {
            RGBQUAD tc;

            if (0x00 == type) {
                tc.rgbRed = byteBinary[iCount];  // R - red
                tc.rgbGreen = byteBinary[tCount + iCount];  // G - green
                tc.rgbBlue = byteBinary[tCount * 2 + iCount++];  // B - blue
            }
            else if (0x01 == type) {
                tc.rgbRed = byteBinary[iCount];  // B - blue
                tc.rgbGreen = byteBinary[iCount];  // G - green
                tc.rgbBlue = byteBinary[iCount++];  // R - red
            }

            FreeImage_SetPixelColor(newImage, t_j, t_i, &tc);
        }
    }

    FreeImage_Save(ifif, newImage, filePath);
    FreeImage_Unload(newImage);
    FreeImage_DeInitialise();

    //cv::Mat newImage = cv::Mat::zeros(cv::Size(iWidth, iHeight), CV_8UC3);  // BGR

    //long tCount = sizeImage / 3, iCount = 0;
    //for (int j = 0; j < iHeight; j++)
    //{
    //	for (int i = 0; i < iWidth; i++)
    //	{
    //		if (0x00 == type) {
    //			newImage.at<cv::Vec3b>(j, i)[2] = byteBinary[iCount];  // B - blue
    //			newImage.at<cv::Vec3b>(j, i)[1] = byteBinary[tCount + iCount];  // G - green
    //			newImage.at<cv::Vec3b>(j, i)[0] = byteBinary[tCount * 2 + iCount++];  // R - red
    //		}
    //		else if (0x01 == type) {
    //			newImage.at<cv::Vec3b>(j, i)[0] = byteBinary[iCount];  // B - blue
    //			newImage.at<cv::Vec3b>(j, i)[1] = byteBinary[iCount];  // G - green
    //			newImage.at<cv::Vec3b>(j, i)[2] = byteBinary[iCount++];  // R - red
    //		}
    //	}
    //}

    ////if (0x01 == type) cv::cvtColor(newImage, newImage, CV_BGR2GRAY);
    //
    //int iResult = (cv::imwrite(filePath, newImage)) ? 0 : ERR_IMAGE_SAVE;

    return 0;
}

int execFormatImage(const char* srcImage, const char* tarImage, int angle, float proportion, int thresh,
    int &gWidth, int &gHeight)
{
    FreeImage_Initialise(TRUE);

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    fif = FreeImage_GetFileType(srcImage);

    FIBITMAP *dib = FreeImage_Load(fif, srcImage);

    gWidth = FreeImage_GetWidth(dib);
    gHeight = FreeImage_GetHeight(dib);

//    unsigned pit = FreeImage_GetPitch(dib);
    // 旋转
    dib = FreeImage_Rotate(dib, angle);
    // 缩放 & 旋转画布
    if (angle % 180 == 0)
        dib = FreeImage_Rescale(dib, (int)(gWidth * proportion), (int)(gHeight * proportion));
    else
        dib = FreeImage_Rescale(dib, (int)(gHeight * proportion), (int)(gWidth * proportion));

    // 单通道位图转化
    dib = FreeImage_Threshold(dib, (BYTE)thresh);

    int sv = FreeImage_Save(FIF_BMP, dib, tarImage);
    FreeImage_Unload(dib);
    FreeImage_DeInitialise();

    LOG_INFO("image tarImage= %s save= %d", tarImage, sv);
    return 0x00;
}

int execCmdTemplate(us_short i_fi, us_byte *iData, int iLength)
{
    int i_Result = 0, i_ReadRst = 0, i_Length = 0, i_Flag = M_MAKE_SFLAG;

    // send
    i_Result = execCmdWrite(i_fi, iData, iLength, iLength, i_Flag, 0x00);
    if ((0xDFFF < i_Result) && (i_Result < 0xEFFF)) return i_Result;

    us_byte *pRevData = (us_byte*)malloc(BUFFER_MAX_LENGTH * sizeof(char));
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    i_ReadRst = execCmdRead(pRevData, i_Length, READ_BUS_CONFIRM_TIMEOUT);

    i_Result = (0 == i_ReadRst) ? ERR_DEVICE_BUSY : 0;
    i_Result = execCfmChk(pRevData, 0x00, i_Flag);

    LOG_DEBUG("execCfmChk flag= %d i_Result= %d", i_Flag, i_Result);
    if (0 != i_Result) return i_Result;

    // receive
    i_Result = execCmdCfm(0x01, i_Flag);
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    i_ReadRst = execCmdRead(pRevData, i_Length);

    // decode
    if (i_ReadRst >= DATA_MIN_LENGTH)
    {
        int iflag = (int)pRevData[DATA_P_FLAG] + ((int)pRevData[DATA_P_FLAG + 1] << 8);
        i_Result = (0 == iflag) ? iflag : ((int)pRevData[DATA_P_FLAG + 2]);
    }
    else i_Result = ERR_TRANS_DATA;

    LOG_DEBUG("execCmdRead i_ReadRst= %d i_Result= %d", i_ReadRst, i_Result);
    free(pRevData);
    return i_Result;
}

int execGetTemplate(us_short i_fi, us_byte *iData, int iLength, void *o_data, int &o_len)
{
    int i_Result = 0, i_ReadRst = 0, i_Length = 0, i_Flag = M_MAKE_SFLAG;

    // send
    i_Result = execCmdWrite(i_fi, iData, iLength, iLength, i_Flag, 0x00);
    if ((0xDFFF < i_Result) && (i_Result < 0xEFFF)) return i_Result;

    us_byte *pRevData = (us_byte*)malloc(BUFFER_MAX_LENGTH * sizeof(char));
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    i_ReadRst = execCmdRead(pRevData, i_Length, READ_BUS_CONFIRM_TIMEOUT);

    i_Result = (0 == i_ReadRst) ? ERR_DEVICE_BUSY : 0;
    i_Result = execCfmChk(pRevData, 0x00, i_Flag);

    LOG_DEBUG("execCfmChk flag= %d i_Result= %d", i_Flag, i_Result);
    if (0 != i_Result) return i_Result;

    // receive
    i_Result = execCmdCfm(0x01, i_Flag);
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    i_ReadRst = execCmdRead(pRevData, i_Length);

    // decode
    if (i_ReadRst >= DATA_MIN_LENGTH)
    {
        int iflag = (int)pRevData[DATA_P_FLAG] + ((int)pRevData[DATA_P_FLAG + 1] << 8);
        i_Result = (0 == iflag) ? iflag : ((int)pRevData[DATA_P_FLAG + 2]);
    }
    else i_Result = ERR_TRANS_DATA;

    if (0 == i_Result) {
        o_len = ((int)pRevData[DATA_P_LENGTH]) + ((int)pRevData[DATA_P_LENGTH + 1] << 8);
        memcpy(o_data, pRevData + DATA_P_START, o_len);
    }

    LOG_DEBUG("execCmdRead i_ReadRst= %d i_Result= %d", i_ReadRst, i_Result);
    free(pRevData);
    return i_Result;
}

// -- ** -- ** -- ** -- ** -- ** -- ** -- ** -- ** --
// -- ** -- ** -- ** -- ** -- ** -- ** -- ** -- ** --

int openDevice()
{
    int i_Result = device_init();

    u_devHandle = device_openWithID(MY_VID, MY_PID);
    if (NULL == u_devHandle) return ERR_USB_DONT_OPEN;

    i_Result = device_config(u_devHandle, USB_G_CONF, USB_G_INTF);

    char *filePath = (char*)malloc(32 * sizeof(char));
    memset(filePath, '\0', (32 * sizeof(char)));
    strcat(filePath, LOCAL_LOG_PATH);
    strcat(filePath, LOCAL_LOG_FILENAME);
    strcat(filePath, ".clog");

    if (!access(filePath, 0)) cDebug_init(LL_DEBUG, LOCAL_LOG_FILENAME, LOCAL_LOG_PATH, 1, 1);
    LOG_INFO("== == == == == == == == == == == ==");
    LOG_INFO("== == $$ RUN BPM22 Device. $$ == ==");
    LOG_INFO("== == == == == == == == == == == ==");
    LOG_DEBUG("device_openWithID= %p", u_devHandle);
    LOG_DEBUG("device_config= %d", i_Result);

    i_Result = device_config_info_printf();
    return i_Result;
}

void closeDevice()
{
    device_close();

    LOG_INFO("== == $$ Device STOPED ... $$ == ==");
    cDebug_close();
}
LIBUSP_BPM22_API int feedCheck(int iCheckBox, int iPaperType)
{
    int i_Result = 0, i_Count = 0;
    LOG_INFO("feedCheck !! iCheckBox= %d iPaperType=%d", iCheckBox, iPaperType);

    us_byte * t_fi_data = (us_byte*)malloc(2 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 2 * sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iCheckBox), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iPaperType), sizeof(us_byte));

    i_Result = execCmdTemplate(0xE2, t_fi_data, i_Count);
    free(t_fi_data);

    return i_Result;

    // 0xEF
    //iResult = execCmdWrite(0xEF, 0, NULL, 0, 0);
    //i_flag = M_MAKE_SFLAG;
    //iResult = execCmdWrite(0xEF, NULL, 0, 0, 0x00, i_flag);
    //if ((0xDFFF < iResult) && (iResult < 0xEFFF)) return iResult;
}

int ocrScan(const char *fpRGB, int pType, char *ocrNum)
{
    int i_Result = 0, i_Count = 0;
    LOG_INFO("ocrScan !! filepath=%s", fpRGB);

    i_Result = execCmdTemplate(0xE3, NULL, i_Count);

    return i_Result;

    // save image

    // exec ocr
//    HINSTANCE m_bill_DLL = LoadLibrary(L"BillProcess.dll");
//	typedef int(*lpBP)(const char *, int , char *);

//	if (m_bill_DLL != NULL)
//	{
//		lpBP lpBP_fun = (lpBP)GetProcAddress(m_bill_DLL, "BillSaleProcess");
//		iResult = lpBP_fun(fpRGB, pType, ocrNum);
//	}
//	else iResult = ERR_BILLS_UNKNOW;

//	LOG(INFO) << "-- Bill OCR lpBP_fun .. " << pType << " " << ocrNum;

//	FreeLibrary(m_bill_DLL);
    return i_Result;
}

int execPrintScan_withJson(const char *json_str, int t_length,
                               int iPrintEnabled, int iScanEnabled, int iPrintNext)
{
    int i_Result = 0, i_ReadRst = 0, i_Count = BUFFER_MAX_LENGTH, i_Flag = M_MAKE_SFLAG;
    LOG_INFO("getDevStatus !! flag= %d iPrintEnabled= %d iScanEnabled= %d iPrintNext= %d",
             i_Flag, iPrintEnabled, iScanEnabled, iPrintNext);

    int iCheckLen = 0, iCheckWid = 0;
    char s_size[512] = "\0", s_bmp[32 * 1024] = "\0", s_txt[32 * 1024] = "\0";

    return i_Result;
}

int printAndScan(int iPrintEnabled, int iScanEnabled, int iPrintNext,
                                            int iCheckLen, int iCheckWid)
{
    int i_Result = 0, i_Count = 0;
    LOG_INFO("printAndScan !! iPrintEnabled= %d iScanEnabled= %d iPrintNext= %d iCheckLen= %d iCheckWid= %d",
             iPrintEnabled, iScanEnabled, iPrintNext, iCheckLen, iCheckWid);

    // make data
    us_byte * t_fi_data = (us_byte*)malloc(32 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 32 * sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iPrintEnabled), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iScanEnabled), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iPrintNext), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iCheckLen), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iCheckLen >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iCheckWid), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iCheckWid >> 8), sizeof(us_byte));

    // exec send commands
    i_Result = execCmdTemplate(0xE5, t_fi_data, i_Count);

    free(t_fi_data);
    return i_Result;
}

int outPaperDoorControl(int iCloseDoor)
{
    int i_Result = 0, i_Count = 0;
    LOG_INFO("outPaperDoorControl !! iCloseDoor= %d", iCloseDoor);

    // make data
    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iCloseDoor), sizeof(us_byte));

    // exec send commands
    i_Result = execCmdTemplate(0xE6, t_fi_data, i_Count);
    free(t_fi_data);

    return i_Result;
}

int getIdentifyInfo()
{
    LOG_INFO("getIdentifyInfo !! 0xE7");

    // exec send commands
    return execCmdTemplate(0xE7, NULL, 0);
}

int getOneImage(const char* cImageFileName, const IMAGETYPE &type, const IMAGE_FORMAT &iFormat)
{
    int iResult = 0;
    LOG_INFO("getOneImage !! filepath= %s type= %d iFormat= %d", cImageFileName, type, iFormat);

    switch (type)
    {
    case IMGNumRGB: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x01, 0x02, 0x03);
        break;
    }
    case IMGNum_IGRAY: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x04, 0x00, 0x00);
        break;
    }
    case IMGFrontRGB: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x05, 0x06, 0x07);
        break;
    }
    case IMGFloorRGB: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x08, 0x09, 0x0A);
        break;
    }
    case IMGFront_URGB: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x0B, 0x0C, 0x0D);
        break;
    }
    case IMGFloor_URGB: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x0E, 0x0F, 0x10);
        break;
    }
    case IMG_UTRGB: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x11, 0x12, 0x013);
        break;
    }
    case IMGFront_IGRAY: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x14, 0x00, 0x00);
        break;
    }
    case IMGFloor_IGRAY: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x15, 0x00, 0x00);
        break;
    }
    case IMG_ITGRAY: {
        iResult = execSaveImage(cImageFileName, iFormat, 0x16, 0x00, 0x00);
        break;
    }
    default: {
        iResult = -1;
        break;
    }
    }

    LOG_INFO("getOneImage !! result= %d", iResult);
    return iResult;
}

int getDevStatus(DeviceStatus &m_DeviceStatus)
{
    int i_Result = 0, i_ReadRst = 0, i_Len = BUFFER_MAX_LENGTH, i_Flag = M_MAKE_SFLAG;
    LOG_INFO("getDevStatus.. flag= %d", i_Flag);

    i_Result = execCmdWrite(0xE8, NULL, 0, 0, i_Flag, 0x00);
    if ((0xDFFF < i_Result) && (i_Result < 0xEFFF)) return i_Result;

    us_byte *pRevData = (us_byte*)malloc(BUFFER_MAX_LENGTH * sizeof(char));
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    i_ReadRst = execCmdRead(pRevData, i_Len, READ_BUS_CONFIRM_TIMEOUT);

    i_Result = (0 == i_ReadRst) ? ERR_DEVICE_BUSY : 0;
    i_Result = execCfmChk(pRevData, 0x00, i_Flag);
    LOG_DEBUG("execCfmChk= %d i_Result= %d", i_Flag, i_Result);
    if (0 != i_Result) return i_Result;

    i_Result = execCmdCfm(0x01, i_Flag);
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    i_ReadRst = execCmdRead(pRevData, i_Len);

    // receive and decode
    if (i_ReadRst >= DATA_MIN_LENGTH)
    {
        int iflag = (int)pRevData[DATA_P_FLAG] + ((int)pRevData[DATA_P_FLAG + 1] << 8);
        i_Result = (0 == iflag) ? iflag : ((int)pRevData[DATA_P_FLAG + 2]);
    }
    else i_Result = ERR_TRANS_DATA;

    if (0 == i_Result) {
        int iSize = ((int)pRevData[DATA_P_LENGTH]) + ((int)pRevData[DATA_P_LENGTH + 1] << 8);
        memcpy(&m_DeviceStatus, pRevData + DATA_P_START, iSize);
    }

    LOG_DEBUG("i_ReadRst= %d i_Result= %d", i_ReadRst, i_Result);
    free(pRevData);
    return i_Result;
}

int takeCheck(int iTimeout)
{
    int i_Result = 0, i_Count = 0;
    LOG_INFO("takeCheck !! iTimeout= %d", iTimeout);

    // make data
    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(iTimeout), sizeof(us_byte));

    // exec send commands
    i_Result = execCmdTemplate(0xE9, t_fi_data, i_Count);
    free(t_fi_data);

    return i_Result;
}

int setCheckOut(int mode, int stamp)
{
    int i_Result = 0, i_Count = 0;
    LOG_INFO("setCheckOut !! mode= %d stamp= %d", mode, stamp);

    // make data
    us_byte * t_fi_data = (us_byte*)malloc(2 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 2 * sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(mode), sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(stamp), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0xED, t_fi_data, i_Count);
    free(t_fi_data);

    return i_Result;
}

int resetDevEx(int bSoftReset)
{
    int i_Result = 0, i_Count = 0;
    LOG_INFO("resetDevEx !! bSoftReset= %d", bSoftReset);

    // make data
    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Count ++, (us_short)(bSoftReset), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0xF3, t_fi_data, i_Count);
    free(t_fi_data);

    return i_Result;
}

int getFirmwareVersion(char *version, int &bufSize)
{
    LOG_INFO("getFirmwareVersion !! %p", version);

    return execGetTemplate(0xEB, NULL, 0, version, bufSize);
}

int updateFirmware(const char * cFileName)
{
    int i_Result = 0;

    // 0xEC
    i_Result = execCmdTemplate(0xEC, NULL, 0x00);
    if (0 != i_Result) return i_Result;
    LOG_DEBUG("updateFirmware !! 0xEC result= %d", i_Result);

    // wait 2s to open usb interface
    Sleep(2000);
    openDevice();

    // 0xA0
    i_Result = execCmdTemplate(0xA0, NULL, 0x00);
    if (0 != i_Result) return ERR_UPDATE_FAILED;
    LOG_DEBUG("updateFirmware !! 0xA0 result= %d", i_Result);

    // open the file for update
    FILE *fp = NULL;
    fopen_s(&fp, cFileName, "rb");
    if (NULL == fp) return ERR_UPDATE_FAILED;

    fseek(fp, 0L, SEEK_END);
    int t_filelength = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char *fdata = (char*)malloc(t_filelength * sizeof(char));
    memset(fdata, 0x00, t_filelength * sizeof(char));
    fread(fdata, sizeof(char), t_filelength, fp);

    int i_flag = M_MAKE_SFLAG;
    i_Result = execSendData(0xA1, fdata, t_filelength, BUFFER_MAX_DATA_LENGTH, i_flag);
    free(fdata);
    fclose(fp);

    if (0 != i_Result) return ERR_UPDATE_FAILED;
    LOG_DEBUG("updateFirmware !! update file result= %d", i_Result);

    // 0xA2
    i_Result = execCmdTemplate(0xA2, NULL, 0x00);
    if (0 != i_Result) return ERR_UPDATE_FAILED;
    LOG_DEBUG("updateFirmware !! 0xA2 result= %d", i_Result);

    return i_Result;
}

int recycleCheck()
{
    LOG_INFO("recycleCheck !! %02X", 0xEE);

    // exec send commands
    return execCmdTemplate(0xEE, NULL, 0);
}

int getLastErrorCode()
{
    LOG_INFO("getLastErrorCode !! %02X", 0xF2);
    // exec send commands
    return execCmdTemplate(0xF2, NULL, 0);
}

int readConfigDev(DeviceConfig &m_DeviceConfig)
{
    LOG_INFO("readConfigDev !! %p", &m_DeviceConfig);

    int ilen = sizeof(DeviceConfig);
    int i_Result = execGetTemplate(0xF4, NULL, 0, &m_DeviceConfig, ilen);

    return i_Result;
}

int writeConfigDev(DeviceConfig &m_DeviceConfig)
{
    LOG_INFO("writeConfigDev !! %p", &m_DeviceConfig);
    int i_Result = 0, i_Len = sizeof(DeviceConfig);

    us_byte * t_fi_data = (us_byte*)malloc(i_Len);
    memset(t_fi_data, 0x00, i_Len);
    memcpy(t_fi_data, &m_DeviceConfig, i_Len);

    // exec
    i_Result = execCmdTemplate(0xF5, t_fi_data, i_Len);
    free(t_fi_data);

    return i_Result;
}

int cisConfigDev(const char *cFileName)
{
    LOG_INFO("writeConfigDev !! filepath= %s", &cFileName);
    int i_Result = 0, i_Flag = M_MAKE_SFLAG;
    i_Result = execCmdTemplate(0xF6, NULL, 0);

    if (0 == i_Result) {
        FILE *fp = NULL;
        fopen_s(&fp, cFileName, "rb");
        if (NULL == fp) return 1;

        fseek(fp, 0L, SEEK_END);
        int t_filelength = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        char *fdata = (char*)malloc(t_filelength * sizeof(char));
        memset(fdata, 0x00, t_filelength * sizeof(char));
        fread(fdata, sizeof(char), t_filelength, fp);

        // splite the update file to transfer
        i_Result = execSendData(0xF6, fdata, t_filelength, BUFFER_MAX_DATA_LENGTH, i_Flag);

        free(fdata);
        fclose(fp);
    }

    return i_Result;
}

int cisLightConfigDev(const char *cLightConfig, int cfgLength)
{
    LOG_INFO("cisLightConfigDev !! cfgLength= %d", cfgLength);

    return execCmdTemplate(0xF7, (us_byte*)cLightConfig, cfgLength);
}

int picTypeConfigDev(int iPicType)
{
    LOG_INFO("picTypeConfigDev !! iPicType= %d", iPicType);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Len++, (us_short)(iPicType), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0xF8, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int calibrationCIS(int iBoxNum)
{
    LOG_INFO("calibrationCIS !! iBoxNum= %d", iBoxNum);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Len++, (us_short)(iBoxNum), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0xF9, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int b_OcrIdentify(SBILLIDENTIFYINFO *sInfo, char *iFrontImage, char *iRearImage)
{
	int i_Result = 0;
	//  pIRT 为红外透射图像bmp文件地址。
	//	pIR 为红外图像bmp文件目录地址
	//	pRGB为rgb图像bmp文件目录地址
	//	pUV为紫外bmp图像文件目录地址
	//	pUVTR为紫外透射图像文件地址
	char pCWD[1024] = "\0";
	_getcwd(pCWD, 1024);
//
	char pRGB[1024] = "\0";
	//const char * pRGB = (NULL == iFrontImage) ? ".\\rgbFront.bmp" : iFrontImage;
	if (NULL != iFrontImage) strcpy(pRGB, iFrontImage);
	else {
		strcpy(pRGB, pCWD);
		strcat(pRGB, "\\RGB_F.bmp");
	}
	i_Result = getOneImage(pRGB, IMGFrontRGB, IMG_F_BMP);
//	LOG(INFO) << "-- IMGFrontRGB " << pRGB << " " << T_Result;
	if (0 != i_Result) return ERR_IMAGE_SAVE;

	char pRearRGB[1024] = "\0";
	//const char * pRearRGB = (NULL == iRearImage) ? ".\\rgbRear.bmp" : iRearImage;
	if (NULL != iRearImage) strcpy(pRearRGB, iRearImage);
	else {
		strcpy(pRearRGB, pCWD);
		strcat(pRearRGB, "\\RGB_R.bmp");
	}
	i_Result = getOneImage(pRearRGB, IMGFloorRGB, IMG_F_BMP);
//	LOG(INFO) << "-- IMGFloorRGB " << pRearRGB << " " << T_Result;
	if (0 != i_Result) return ERR_IMAGE_SAVE;

	return i_Result;

	////////

	char pIRT[1024] = "\0";
	strcpy(pIRT, pCWD);
	strcat(pIRT, "\\IRT.bmp");
	i_Result = getOneImage(pIRT, IMG_ITGRAY, IMG_F_BMP);
//	LOG(INFO) << "-- IMG_ITGRAY " << pIRT << " " << T_Result;
	if (0 != i_Result) return ERR_IMAGE_SAVE;

	char pIR[1024] = "\0";
	strcpy(pIR, pCWD);
	strcat(pIR, "\\IR_F.bmp");
	i_Result = getOneImage(pIR, IMGFront_IGRAY, IMG_F_BMP);
//	LOG(INFO) << "-- IMGFront_IGRAY " << pIR << " " << T_Result;
	if (0 != i_Result) return ERR_IMAGE_SAVE;

	char pUV[1024] = "\0";
	strcpy(pUV, pCWD);
	strcat(pUV, "\\UV_F.bmp");
	i_Result = getOneImage(pUV, IMGFront_URGB, IMG_F_BMP);
//	LOG(INFO) << "-- IMGFront_URGB " << pUV << " " << T_Result;
	if (0 != i_Result) return ERR_IMAGE_SAVE;

	char pUVTR[1024] = "\0";
	strcpy(pUVTR, pCWD);
	strcat(pUVTR, "\\UVT.bmp");
	i_Result = getOneImage(pUVTR, IMG_UTRGB, IMG_F_BMP);
//	LOG(INFO) << "-- IMG_UTRGB " << pUVTR <<" " << T_Result;
	if (0 != i_Result) return ERR_IMAGE_SAVE;

	return i_Result;
}

//LIBUSP_BPM22_API int b_OcrIdentify(SBILLIDENTIFYINFO *sInfo, char *iFrontImage, char *iRearImage)
//{
//	int i_Result = 0;
//	//  pIRT 为红外透射图像bmp文件地址。
//	//	pIR 为红外图像bmp文件目录地址
//	//	pRGB为rgb图像bmp文件目录地址
//	//	pUV为紫外bmp图像文件目录地址
//	//	pUVTR为紫外透射图像文件地址
//	char pCWD[1024] = "\0";
//	_getcwd(pCWD, 1024);
////
//	char pRGB[1024] = "\0";
//	//const char * pRGB = (NULL == iFrontImage) ? ".\\rgbFront.bmp" : iFrontImage;
//	if (NULL != iFrontImage) strcpy(pRGB, iFrontImage);
//	else {
//		strcpy(pRGB, pCWD);
//		strcat(pRGB, "\\RGB_F.bmp");
//	}
//	i_Result = getOneImage(pRGB, IMGFrontRGB, IMG_F_BMP);
////	LOG(INFO) << "-- IMGFrontRGB " << pRGB << " " << T_Result;
//	if (0 != i_Result) return ERR_IMAGE_SAVE;
//
//	char pRearRGB[1024] = "\0";
//	//const char * pRearRGB = (NULL == iRearImage) ? ".\\rgbRear.bmp" : iRearImage;
//	if (NULL != iRearImage) strcpy(pRearRGB, iRearImage);
//	else {
//		strcpy(pRearRGB, pCWD);
//		strcat(pRearRGB, "\\RGB_R.bmp");
//	}
//	i_Result = getOneImage(pRearRGB, IMGFloorRGB, IMG_F_BMP);
////	LOG(INFO) << "-- IMGFloorRGB " << pRearRGB << " " << T_Result;
//	if (0 != i_Result) return ERR_IMAGE_SAVE;
//
//	return i_Result;
//
//	////////
//
////	char pIRT[1024] = "\0";
////	strcpy(pIRT, pCWD);
////	strcat(pIRT, "\\IRT.bmp");
////	T_Result = getOneImage(pIRT, IMG_ITGRAY, IMG_F_BMP);
//////	LOG(INFO) << "-- IMG_ITGRAY " << pIRT << " " << T_Result;
////	if (0 != T_Result) return ERR_IMAGE_SAVE;
////
////	char pIR[1024] = "\0";
////	strcpy(pIR, pCWD);
////	strcat(pIR, "\\IR_F.bmp");
////	T_Result = getOneImage(pIR, IMGFront_IGRAY, IMG_F_BMP);
//////	LOG(INFO) << "-- IMGFront_IGRAY " << pIR << " " << T_Result;
////	if (0 != T_Result) return ERR_IMAGE_SAVE;
////
////	char pUV[1024] = "\0";
////	strcpy(pUV, pCWD);
////	strcat(pUV, "\\UV_F.bmp");
////	T_Result = getOneImage(pUV, IMGFront_URGB, IMG_F_BMP);
//////	LOG(INFO) << "-- IMGFront_URGB " << pUV << " " << T_Result;
////	if (0 != T_Result) return ERR_IMAGE_SAVE;
////
////	char pUVTR[1024] = "\0";
////	strcpy(pUVTR, pCWD);
////	strcat(pUVTR, "\\UVT.bmp");
////	T_Result = getOneImage(pUVTR, IMG_UTRGB, IMG_F_BMP);
//////	LOG(INFO) << "-- IMG_UTRGB " << pUVTR <<" " << T_Result;
////	if (0 != T_Result) return ERR_IMAGE_SAVE;
////
//////	const char * pBDRTitleTEM = ".\\template\\BDRTitleTEM.bmp";
//////	const char * pEmblemTEM = ".\\template\\EmblemTEM.bmp";
//////	const char * pChTitleTEM1 = ".\\template\\ChTitleTEM1.bmp";
//////	const char * pChTitleTEM2 = ".\\template\\ChTitleTEM2.bmp";
//////
//////	HINSTANCE m_bill_DLL = LoadLibrary(L"BillProcess.dll");
//////	typedef int(*lpBP)(const char *, const char *, const char *, const char *, const char *, const char *,
//////		const char *, const char *, const char *, const char *, BillRESULTSTATISTICS &);
//////
//////	if (m_bill_DLL != NULL)
//////	{
//////		BillRESULTSTATISTICS tb_result = { '\0', '\0', '\0', '\0', 0, 0, 0, 0, 0, 0 };
//////		lpBP lpBP_fun = (lpBP)GetProcAddress(m_bill_DLL, "BillProcess");
//////
//////		T_Result = lpBP_fun(pIRT, pIR, pRGB, pUV, pUVTR, pRearRGB,
//////			pBDRTitleTEM, pEmblemTEM, pChTitleTEM1, pChTitleTEM2, tb_result);
//////
//////		LOG(INFO) << "-- lpBP_fun result " << T_Result;
//////		if (0 != T_Result) return T_Result;
//////
//////		// // // //
//////		sInfo->BillType = tb_result.BillType;
//////		sInfo->BankCode = tb_result.BankCode;
//////		sInfo->IsFactComp = tb_result.IsFactComp;
//////		sInfo->IsIdentification = tb_result.IsIdentification;
//////		sInfo->IsDirection = tb_result.IsDirection;
//////		sInfo->isIdentify = tb_result.isIdentify;
//////		strncpy(sInfo->qr_code, tb_result.Qrcode_res, 80);
//////
//////		char *tp_OCR = tb_result.Ocr_res;
//////		if ((4 == tb_result.BillType) || (2 == tb_result.BillType)) {
//////			strncpy(sInfo->ocr_numcode_01, tp_OCR, 12);		// char bdrticketocr_res[12];//票号
//////			tp_OCR += 12;
//////			strncpy(sInfo->ocr_numcode_02, tp_OCR, 18);		// char bdraccountocr_res[18];//账号
//////			tp_OCR += 18;
//////			strncpy(sInfo->ocr_amtFigures, tp_OCR, 12);		// char bdrAmountFig[12];//小写金额
//////			tp_OCR += 12;
//////			strncpy(sInfo->ccr_code, tp_OCR, 30);		// char ccCode[30];//印章串码
//////			tp_OCR = NULL;
//////		}
//////		else if ((0 == tb_result.BillType) || (1 == tb_result.BillType) || (3 == tb_result.BillType)) {
//////			strncpy(sInfo->ocr_numcode_01, tp_OCR, 8);	// char chocr_res1[8]; //上票号
//////			tp_OCR += 8;
//////			strncpy(sInfo->ocr_numcode_02, tp_OCR, 8);	// char chocr_res2[8]; //下票号
//////			tp_OCR += 8;
//////			strncpy(sInfo->ocr_drawerAct, tp_OCR, 20);	// char chOcrDrawerAct_res[20]; //出票人账号
//////			tp_OCR += 20;
//////			strncpy(sInfo->ocr_amtFigures, tp_OCR, 11);	// char chOcrAmtFigures_res[11]; //小写金额
//////			tp_OCR += 11;
//////			strncpy(sInfo->ocr_amtWords, tp_OCR, 50);	// char chOcrAmtWords_res[50]; //大写金额
//////			tp_OCR += 50;
//////			strncpy(sInfo->ocr_password, tp_OCR, 22);	// char chOcrPassword_res[22]; //密码
//////			tp_OCR += 22;
//////			strncpy(sInfo->ocr_bankcode, tp_OCR, 12);	// char chOcrBankNumber_res[12]; //行号
//////			tp_OCR += 12;
//////			strncpy(sInfo->ocr_issueDate, tp_OCR, 30);		// char chOcrIssueDate_res[30]; //出票日期
//////			tp_OCR += 30;
//////			strncpy(sInfo->ocr_payee, tp_OCR, 50);		// char chOcrPayee_res[50]; // 收款人
//////			tp_OCR += 50;
//////			strncpy(sInfo->ocr_payBankName, tp_OCR, 50);		// char chOcrPayBankName_res[50]; //付款行名称
//////			tp_OCR += 50;
//////			strncpy(sInfo->ocr_endorser, tp_OCR, 15);		// 背书人签章
//////			tp_OCR += 15;
//////			strncpy(sInfo->ocr_payeeAndIDCarD, tp_OCR, 40);		// 收款人名字和身份证号码
//////			tp_OCR = NULL;
//////		}
//////	}
//////	else T_Result = ERR_BILLS_UNKNOW;
//////
//////	LOG(INFO) << "-- lpBP_fun free ";
//////
//////	FreeLibrary(m_bill_DLL);
////	return T_Result;
//}

// -- ** -- ** -- ** -- ** -- ** -- ** -- ** -- ** --
// -- ** -- ** -- ** -- ** -- ** -- ** -- ** -- ** --
int dm_cmdcoder(const char *iData, int iLength)
{
	return execCmdCoder((us_byte*)iData, iLength);
}

int dm_cmddecoder(char *iData, int &iLength)
{
	int i_Result = execCmdRead((us_byte*)iData, iLength);
	if (i_Result >= 0) i_Result = 0;

	return i_Result;
}

int dm_startCIS(int i_value)
{
    LOG_INFO("dm_startCIS !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Len++, (us_short)(i_value), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0x11, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_motoPower(int i_value)
{
    LOG_INFO("dm_motoPower !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Len++, (us_short)(i_value), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0x12, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_printerPower(int i_value)
{
    LOG_INFO("dm_printerPower !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Len++, (us_short)(i_value), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0x13, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_heatingPower(int i_value)
{
    LOG_INFO("dm_heatingPower !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(4 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 4 * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_value), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_value >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_value >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_value >> 24), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0x14, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_setDebugModel(int i_value)
{
    LOG_INFO("dm_setDebugModel !! i_value= %d", i_value);

    if (0 == i_value) return execCmdTemplate(0x2A, NULL, 0);
    if (1 == i_value) return execCmdTemplate(0x29, NULL, 0);

    return (-1);
}

int dm_readHeatTemperature(int &i_value)
{
    LOG_INFO("dm_readHeatTemperature !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(32 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 32 * sizeof(us_byte));

    // exec
    i_Result = execGetTemplate(0x15, NULL, 0, t_fi_data, i_Len);

    i_value = 0;
    i_value += (int)(t_fi_data[0]);
    i_value += (int)(t_fi_data[1]) << 8;
    i_value += (int)(t_fi_data[2]) << 16;
    i_value += (int)(t_fi_data[3]) << 24;

    free(t_fi_data);
    return i_Result;
}

int dm_readVoltage(int &i_value)
{
    LOG_INFO("dm_readVoltage !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(32 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 32 * sizeof(us_byte));

    // exec
    i_Result = execGetTemplate(0x16, NULL, 0, t_fi_data, i_Len);

    i_value = 0;
    i_value += (int)(t_fi_data[0]);
    i_value += (int)(t_fi_data[1]) << 8;
    i_value += (int)(t_fi_data[2]) << 16;
    i_value += (int)(t_fi_data[3]) << 24;

    free(t_fi_data);
    return i_Result;
}

int dm_readButtonValue(int &i_value)
{
    LOG_INFO("dm_readButtonValue !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(32 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 32 * sizeof(us_byte));

    // exec
    i_Result = execGetTemplate(0x17, NULL, 0, t_fi_data, i_Len);

    i_value = 0;
    i_value += (int)t_fi_data[0];

    free(t_fi_data);
    return i_Result;
}

int dm_setLedValue(int i_num, int i_value)
{
    LOG_INFO("dm_setLedValue !! i_num= %d i_value= %d", i_num, i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(2 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 2 * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_num), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_value), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0x18, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_setWarnValue(int i_value)
{
    LOG_INFO("dm_setWarnValue !! i_value= %d", i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_value), sizeof(us_byte));

    // exec
    i_Result = execCmdTemplate(0x19, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_r_FRAM(int address, char *i_value, int &i_size)
{
	LOG_INFO("dm_r_FRAM !! address= %d i_value= %p", address, i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(4 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 4 * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 24), sizeof(us_byte));

    i_Result = execGetTemplate(0x1A, t_fi_data, i_Len, i_value, i_size);

    free(t_fi_data);
    return i_Result;
}

int dm_w_FRAM(int address, char *i_value, int &i_size)
{
    LOG_INFO("dm_w_FRAM !! address= %d i_value= %p", address, i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data, 0x00, BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 24), sizeof(us_byte));
    memcpy(t_fi_data + i_Len, i_value, i_size);

    i_Len += i_size;
    i_Result = execCmdTemplate(0x1B, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_r_SPIFLASH(int address, char *i_value, int &i_size)
{
	LOG_INFO("dm_r_SPIFLASH !! address= %d i_value= %p", address, i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(4 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 4 * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 24), sizeof(us_byte));

    i_Result = execGetTemplate(0x1C, t_fi_data, i_Len, i_value, i_size);

    free(t_fi_data);
    return i_Result;
}

int dm_w_SPIFLASH(int address, char *i_value, int &i_size)
{
    LOG_INFO("dm_w_SPIFLASH !! address= %d i_value= %p", address, i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data, 0x00, BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 24), sizeof(us_byte));
    memcpy(t_fi_data + i_Len, i_value, i_size);

    i_Len += i_size;
    i_Result = execCmdTemplate(0x1D, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_r_DDR(int address, char *i_value, int &i_size)
{
	LOG_INFO("dm_r_DDR !! address= %d i_value= %p", address, i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(4 * sizeof(us_byte));
    memset(t_fi_data, 0x00, 4 * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 24), sizeof(us_byte));

    i_Result = execGetTemplate(0x1E, t_fi_data, i_Len, i_value, i_size);

    free(t_fi_data);
    return i_Result;
}

int dm_w_DDR(int address, char *i_value, int &i_size)
{
    LOG_INFO("dm_w_DDR !! address= %d i_value= %p", address, i_value);
    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data, 0x00, BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(address >> 24), sizeof(us_byte));
    memcpy(t_fi_data + i_Len, i_value, i_size);

    i_Len += i_size;
    i_Result = execCmdTemplate(0x1F, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_getSensorStatus(char *i_value, int &i_size)
{
    LOG_INFO("dm_getSensorStatus !! %p", i_value);

    return execGetTemplate(0x20, NULL, 0, i_value, i_size);
}

int dm_getPowerStatus(char *i_value, int &i_size)
{
    LOG_INFO("dm_getPowerStatus !! %p", i_value);

    return execGetTemplate(0x21, NULL, 0, i_value, i_size);
}

int dm_getPrinterStatus(char *i_value, int &i_size)
{
    LOG_INFO("dm_getPrinterStatus !! %p", i_value);

    return execGetTemplate(0x22, NULL, 0, i_value, i_size);
}

int dm_getVerion_ARM(char *i_value, int &i_size)
{
    LOG_INFO("dm_getVerion_ARM !! %p", i_value);

    return execGetTemplate(0x23, NULL, 0, i_value, i_size);
}

int dm_getVerion_DSP(char *i_value, int &i_size)
{
    LOG_INFO("dm_getVerion_DSP !! %p", i_value);

    return execGetTemplate(0x24, NULL, 0, i_value, i_size);
}


int dm_getVerion_FPGA(char *i_value, int &i_size)
{
    LOG_INFO("dm_getVerion_FPGA !! %p", i_value);

    return execGetTemplate(0x25, NULL, 0, i_value, i_size);
}

int dm_getVerion_STM32(char *i_value, int &i_size)
{
    LOG_INFO("dm_getVerion_STM32 !! %p", i_value);

    return execGetTemplate(0x26, NULL, 0, i_value, i_size);
}

int dm_ctl_stepMotor(short i_id, short i_enabled, short i_dirt,
		int i_speed, int i_steps)
{
    LOG_INFO("dm_ctl_stepMotor !! i_id= %d", i_id);
    LOG_INFO("i_enabled= %d i_dirt= %d i_speed= %d i_steps= %d",
    		i_enabled, i_dirt, i_speed, i_steps);

    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data, 0x00, BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_id), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_enabled), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_dirt), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_speed), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_speed >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_steps), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_steps >> 8), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_steps >> 16), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_steps >> 24), sizeof(us_byte));

    i_Result = execCmdTemplate(0x40, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_ctl_dcMotor(short i_id, short i_enabled, short i_dirt,
		int i_speed)
{
    LOG_INFO("dm_ctl_dcMotor !! i_id= %d", i_id);
    LOG_INFO("i_enabled= %d i_dirt= %d i_speed= %d",
    		i_enabled, i_dirt, i_speed);

    int i_Result = 0, i_Len = 0;

    us_byte * t_fi_data = (us_byte*)malloc(BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data, 0x00, BUFFER_MAX_DATA_LENGTH * sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_id), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_enabled), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_dirt), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_speed), sizeof(us_byte));
    memset(t_fi_data + i_Len ++, (us_short)(i_speed >> 8), sizeof(us_byte));

    i_Result = execCmdTemplate(0x41, t_fi_data, i_Len);

    free(t_fi_data);
    return i_Result;
}

int dm_dotest(us_short t_fi)
{
	LOG_INFO("dm_dotest !! %d", t_fi);

	return execCmdTemplate(t_fi, NULL, 0);
}
