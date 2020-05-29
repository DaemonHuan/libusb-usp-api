
#include "usbbasic.h"

#include <stdio.h>
#include <stdlib.h>

//LIBUSB_ENDPOINT_IN = 0x80     /** In: device-to-host */
#define USB_EP_IN		0x81

//LIBUSB_ENDPOINT_OUT = 0x00        /** Out: host-to-device */
#define USB_EP_OUT		0x01

libusb_device **g_devs;
libusb_device_handle *u_devHandle = NULL;

int device_init(void)
{
    if (u_devHandle) device_close();

    int relt = libusb_init(NULL);
    if (relt < 0) return relt;

    relt = libusb_get_device_list(NULL, &g_devs);
    if (relt < 0) return relt;

//    const struct libusb_version * t_version = libusb_get_version();
//    qDebug("-- Using libusb v%d.%d.%d.%d",
//           t_version->major, t_version->minor, t_version->micro, t_version->nano);

    return relt;
}

libusb_device_handle* device_openWithID(const us_short u_vid, const us_short u_pid)
{
    libusb_device *dev = NULL;
    struct libusb_device_descriptor t_desc;
    libusb_device_handle *t_handle = NULL;

    int ii = 0, relt = 0;
    while ((dev = g_devs[ii ++]) != NULL) {
        relt = libusb_get_device_descriptor(dev, &t_desc);
        if (relt < 0) continue;

        if ((t_desc.idVendor == u_vid) && (t_desc.idProduct == u_pid)) {
            relt = libusb_open(dev, &t_handle);
            break;
        }
    }

    return t_handle;
}

int device_config(libusb_device_handle *u_dev, const int cfg, const int inf)
{
    int relt = 0;

    relt = libusb_kernel_driver_active(u_dev, 0);
//    qDebug() <<"-- set driver active .." <<relt;

    if (1 == relt) {
        relt = libusb_set_auto_detach_kernel_driver(u_dev, 0);
//        qDebug() <<"-- set auto driver detach .." <<relt;
    }

    relt = libusb_set_configuration(u_dev, cfg);
//    qDebug() <<"-- set configure .." <<relt;
    if (relt < 0) {
    }

    relt = libusb_claim_interface(u_dev, inf);
//    qDebug() <<"-- set interface .." <<relt;
    if (relt < 0) {
    }

    return relt;
}

void device_close(void)
{
//    libusb_release_interface(u_dev, USB_G_INTF);
    if (u_devHandle == NULL) return;

    libusb_close(u_devHandle);
    u_devHandle = NULL;

    libusb_free_device_list(g_devs, 1);
    libusb_exit(NULL);

//    qDebug() <<"** close device and free ..";
}

int device_config_info_printf()
{
//    struct libusb_device_descriptor desc;
//    struct libusb_config_descriptor *config;
//    int relt = 0;
//    struct libusb_device *dev = libusb_get_device(u_devHandle);
//
//    relt = libusb_get_device_descriptor(dev, &desc);
//    if (relt < 0) return -1;
//    qDebug() <<"-- get device descriptor .." <<relt;
//    qDebug() <<"-- Bus:" <<libusb_get_bus_number(dev) <<"device:" <<libusb_get_device_address(dev);
//    qDebug("-- device vID pID : %04X - %04X", desc.idVendor, desc.idProduct);
//
//    for (int a = 0; a < desc.bNumConfigurations; a++) {
//        relt = libusb_get_config_descriptor(dev, a, &config);
//        if (relt < 0) break;
//        qDebug() <<"-- get device config descriptor .." <<relt;
//        qDebug() <<"\t-- Configure .." <<a;
//        qDebug() <<"\t< wTotalLength: \t" <<config->wTotalLength;
//        qDebug() <<"\t< bNumInterfaces: \t" <<config->bNumInterfaces;
//        qDebug() <<"\t< bConfigurationValue: \t" <<config->bConfigurationValue;
//        qDebug() <<"\t< iConfiguration: \t" <<config->iConfiguration;
//        qDebug() <<"\t< bmAttributes: \t" <<config->bmAttributes;
//        qDebug() <<"\t< MaxPower: \t" <<config->MaxPower;
//
//        for (int b = 0; b < config->bNumInterfaces; b++) {
//            const struct libusb_interface *infa = &config->interface[b];
//            for (int c = 0; c < infa->num_altsetting; c ++) {
//                const struct libusb_interface_descriptor *altsetting = &infa->altsetting[c];
//                qDebug() <<"\t\t-- Interface .." <<c;
//                qDebug() <<"\t\t<< bInterfaceNumber: \t" <<altsetting->bInterfaceNumber;
//                qDebug() <<"\t\t<< bAlternateSetting: \t" <<altsetting->bAlternateSetting;
//                qDebug() <<"\t\t<< bNumEndpoints: \t" <<altsetting->bNumEndpoints;
//                qDebug() <<"\t\t<< bInterfaceClass: \t" <<altsetting->bInterfaceClass;
//                qDebug() <<"\t\t<< bInterfaceSubClass: \t" <<altsetting->bInterfaceSubClass;
//                qDebug() <<"\t\t<< bInterfaceProtocol: \t" <<altsetting->bInterfaceProtocol;
//                qDebug() <<"\t\t<< iInterface: \t" <<altsetting->iInterface;
//            }
//        }
//    }
    return 0;
}

int usbBusWrite(us_byte *p_data, int *p_length, int s_timeout)
{
    int iResult = ERR_USB_STATIC_WRITE;
    if (!u_devHandle) return iResult;

//    iResult = libusb_bulk_transfer(u_dev, USB_EP_OUT, p_data, p_length, NULL, s_timeout);
    iResult = libusb_interrupt_transfer(u_devHandle, USB_EP_OUT, p_data, *p_length, NULL, s_timeout);
//    qDebug("+ [%03d] %s", (*p_length), QByteArray::fromRawData((char*)p_data, *p_length).toHex(' ').data());
    return (iResult == 0) ? (*p_length) : iResult;
}

int usbBusRead(us_byte *p_data, int *p_length, int s_timeout)
{
    int iResult = ERR_USB_STATIC_READ;
    if (!u_devHandle) return iResult;

//    iResult = libusb_bulk_transfer(u_dev, USB_EP_IN, p_data, BUFFER_MAX_LENGTH, &p_length, s_timeout);
    iResult = libusb_interrupt_transfer(u_devHandle, USB_EP_IN, p_data, BUFFER_MAX_LENGTH, p_length, s_timeout);

//    qDebug("- [%03d] %s", (*p_length), QByteArray::fromRawData((char*)p_data, *p_length).toHex(' ').data());
    return (iResult == 0) ? (*p_length) : iResult;
}

us_short g_getCRC16(const us_byte *p_data, int p_length)
{
    us_short cst_H[16] = { 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
        0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1 };
    us_short cst_L[16] = { 0x00, 0x21, 0x42, 0x63, 0x84, 0xa5, 0xc6, 0xe7,
        0x08, 0x29, 0x4a, 0x6b, 0x8c, 0xad, 0xce, 0xef };

    if (0 >= p_length) return 0x00;

    us_byte kh = 0x00, kl = 0x00;
    kh = *p_data;
    if (p_length > 1) kl = *(p_data + 1);
    else kl = 0;

    us_byte j = 0;
    for (int i = 0; i < p_length; i++) {
        j = (us_byte)(kh / 16);
        kh = (us_byte)(kh * 16);
        kh = (us_byte)(kh + (kl / 16));

        kl = (us_byte)(kl * 16);
        if ((i + 2) < p_length) kl = (us_byte)(kl + (*(p_data + i + 2) / 16));

        kh = (us_byte)(kh ^ cst_H[j]);
        kl = (us_byte)(kl ^ cst_L[j]);

        j = (us_byte)(kh / 16);

        kh = (us_byte)(kh * 16);
        kh = (us_byte)(kh + (kl / 16));

        kl = (us_byte)(kl * 16);
        if ((i + 2) < p_length) kl = (us_byte)(kl + (*(p_data + i + 2) & 0x0F));

        kh = (us_byte)(kh ^ cst_H[j]);
        kl = (us_byte)(kl ^ cst_L[j]);
    }

    return (us_short)(kh * 256 + kl);
}

int execCmdCoder(const us_byte *iData, int iLength, int iClean)
{
    int iResult = 0, tCount = iLength;
    if (iLength < 0 || iLength > BUFFER_MAX_LENGTH) return ERR_TRANS_PARA;

    // free Bus Data
    if (1 == iClean) libusb_clear_halt(u_devHandle, USB_EP_IN);

    //
    iResult = usbBusWrite(const_cast<us_byte*>(iData), &tCount);
    if (iResult == ERR_USB_STATIC_WRITE) iResult = ERR_USB_DONT_OPEN;
    else if (iResult <= 0) iResult = ERR_TRANS_NOTSEND;

    return (iResult = 0);
}

int execCmdWrite(us_short t_fi, const us_byte *iData, int is_length, int it_length,
                 int iFlag, int iNum, int iClean)
{
    int iResult = 0;
    if (is_length < 0 || is_length > BUFFER_MAX_DATA_LENGTH) return ERR_TRANS_PARA;
    if (iNum < 0) return ERR_TRANS_PARA;

    // free Bus Data
    if (1 == iClean) libusb_clear_halt(u_devHandle, USB_EP_IN);

    //
    us_byte *iCommands = (us_byte *)malloc(BUFFER_MAX_LENGTH * sizeof(us_byte));
    if (iCommands == NULL) return ERR_TRANS_MALLOC;

    us_byte *pData = iCommands;
    int tCount = 0;
    memset(iCommands, 0x00, BUFFER_MAX_LENGTH * sizeof(char));

    // 10 07
    memset(pData + tCount++, 0x10, sizeof(us_byte));
    memset(pData + tCount++, 0x07, sizeof(us_byte));
    // flag
    memset(pData + tCount++, iFlag, sizeof(us_byte));
    // data length
    memset(pData + tCount++, (us_short)(is_length), sizeof(us_byte));
    memset(pData + tCount++, (us_short)(is_length >> 8), sizeof(us_byte));
    // totle length
    memset(pData + tCount++, (us_short)(it_length), sizeof(us_byte));
    memset(pData + tCount++, (us_short)(it_length >> 8), sizeof(us_byte));
    memset(pData + tCount++, (us_short)(it_length >> 16), sizeof(us_byte));
    memset(pData + tCount++, (us_short)(it_length >> 24), sizeof(us_byte));
    // fi
    memset(pData + tCount++, t_fi, sizeof(us_byte));
    // data
    if (is_length > 0) {
        memcpy(pData + tCount, iData, is_length * sizeof(us_byte));
        tCount += is_length;
    }
    //
    memset(pData + tCount++, (us_short)(iNum), sizeof(us_byte));
    memset(pData + tCount++, (us_short)(iNum >> 8), sizeof(us_byte));
    //
    memset(pData + tCount++, 0x10, sizeof(us_byte));
    memset(pData + tCount++, 0x08, sizeof(us_byte));

    // SRC
    //    int t_crc = g_getCRC16(pData, tCount);
    //    if (0 == t_crc) iResult = ERR_TRANS_DATA;
    //    memset(pData + tCount++, (us_short)(t_crc), sizeof(us_byte));
    //    memset(pData + tCount++, (us_short)(t_crc >> 8), sizeof(us_byte));
    memset(pData + tCount++, 0x00, sizeof(us_byte));
    memset(pData + tCount++, 0x00, sizeof(us_byte));

    // send data
    iResult = usbBusWrite(iCommands, &tCount);
    if (iResult == ERR_USB_STATIC_WRITE) iResult = ERR_USB_DONT_OPEN;
    if (iResult <= 0) iResult = ERR_TRANS_NOTSEND;

    // clean
    free(iCommands);
    pData = iCommands = NULL;

    return iResult;
}

int execBusClear()
{
    int iResult = 0, i_len = 0;

    us_byte *iData = (us_byte*)malloc(BUFFER_MAX_LENGTH * sizeof(char));
    if (!iData) return ERR_TRANS_MALLOC;

    do {
        memset(iData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
        iResult = usbBusRead(iData, &i_len);

        Sleep(10);
    }while((0 != iResult) && (ERR_USB_STATIC_READ != iResult));

    free(iData);
    return iResult;
}

int execCmdRead(us_byte *iData, int &it_length, int s_timeout)
{
    int iResult = 0;
    long iEndTime = 0, iStartTime = GetTickCount();
    do {
        iResult = usbBusRead(iData, &it_length);

        if (0 == iResult) Sleep(TRANSFER_SPACE_MS);
        iEndTime = GetTickCount();

    } while (iResult <= 0 && (iEndTime - iStartTime) < s_timeout);

    return iResult;
}

int execCmdCfm(int tNum, us_short tFlag)
{
    int iResult = 0, iCount = 0;
    if (tNum < 0) return ERR_TRANS_PARA;

    us_byte *iCommands = (us_byte *)malloc(BUFFER_MAX_LENGTH * sizeof(char));
    if (iCommands == NULL) return ERR_TRANS_MALLOC;

    memset(iCommands, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    us_byte *t_pdata = iCommands;
    // 10 05 00 00 00
    memset(t_pdata + iCount ++, 0x10, sizeof(us_byte));
    memset(t_pdata + iCount ++, 0x05, sizeof(us_byte));
    memset(t_pdata + iCount ++, tFlag, sizeof(us_byte));
    memset(t_pdata + iCount ++, (us_short)(tNum >> 0), sizeof(us_byte));
    memset(t_pdata + iCount ++, (us_short)(tNum >> 8), sizeof(us_byte));

    iResult = usbBusWrite(iCommands, &iCount);
    if (iResult == ERR_USB_STATIC_WRITE) iResult = ERR_USB_DONT_OPEN;
    if (iResult <= 0) iResult = ERR_TRANS_NOTSEND;

    // clean
    free(iCommands);
    t_pdata = iCommands = NULL;

    return iResult;
}

int execCfmChk(us_byte *iData, int iNum, us_short iFlag)
{
    if ((int)iData[0x00] != 0x10) return -1;
    if ((int)iData[0x01] != 0x06) return -1;
    if ((int)iData[0x02] != iFlag) return -1;

    int i_num = ((int)iData[0x04] << 8) + ((int)iData[0x03]);
    if (i_num != iNum) return -1;

    return 0;
}

//int execCmdCheck(int tNum, us_short tFlag)
//{
//    int iResult = 0, iLen = BUFFER_MAX_LENGTH;

//    us_byte *tData = (us_byte*)malloc(BUFFER_MAX_LENGTH * sizeof(char));
//    if (!tData) return ERR_TRANS_MALLOC;

//    memset(tData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
//    long iEndTime = 0, iStartTime = GetTickCount();

//    do {
//        iResult = usbBusRead(tData, &iLen);

//        if (((int)tData[0x00] == 0x10) && ((int)tData[0x01] == 0x06) && ((int)tData[0x02] == tFlag)
//            && (((int)tData[0x04] << 8) + ((int)tData[0x03])) == tNum) break;
//        else {
//            iResult = 0;
//            memset(tData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
//        }

//        if (0 >= iResult) Sleep(TRANSFER_SPACE_MS);
//        iEndTime = GetTickCount();

//    } while (iResult <= 0 && (iEndTime - iStartTime) < READ_BUS_CONFIRM_TIMEOUT);

//    free(tData);
//    return (0 == iResult) ? ERR_DEVICE_BUSY : iResult;
//}
int execSendData(us_short t_fi, const char *iData, unsigned int iLength, unsigned int iMaxLength,
                 us_short iFlag)
{
    if (iMaxLength > BUFFER_MAX_DATA_LENGTH) return ERR_TRANS_PARA;
    if (NULL == iData) return ERR_TRANS_PARA;

    int iResult = 0, iReadRst = 0, i_Num = 0, tmp_len = 0;
    us_byte *tp_data = (us_byte*)iData;
    unsigned int t_data_length = iLength, ts_l = 0;

    us_byte *pRevData = (us_byte*)malloc(BUFFER_MAX_LENGTH * sizeof(us_byte));
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(us_byte));

    do {
        ts_l = (iLength < iMaxLength) ? t_data_length : iMaxLength;
        iResult = execCmdWrite(t_fi, (us_byte*)tp_data, ts_l, iLength, iFlag, i_Num);
        if ((0xDFFF < iResult) && (iResult < 0xEFFF)) break;

        // 10 05
        iReadRst = execCmdRead(pRevData, tmp_len, READ_BUS_CONFIRM_TIMEOUT);
        iResult = (0 == iReadRst) ? ERR_DEVICE_BUSY : 0;
        iResult = execCfmChk(pRevData, (i_Num ++), iFlag);

        if (0 != iResult) break;

        // == ==
        t_data_length   -= ts_l;
        tp_data         += ts_l;

        iResult = execCmdCfm((i_Num ++), iFlag);
        memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
        iReadRst = execCmdRead(pRevData, tmp_len);

        if (iReadRst >= 18)
        {
            int ii_flag = (int)pRevData[10] + ((int)pRevData[11] << 8);
            iResult = (0 == ii_flag) ? ii_flag : ((int)pRevData[12]);
        }
        else iResult = ERR_TRANS_DATA;

        // == == == ==
        if (0 != iResult) break;
    }while(t_data_length > 0);

    free(pRevData);
    return iResult;
}

int execSaveRGBData(char *iData, int &iLength, int iType, us_short iFlag)
{
    us_byte * t_fi_data = (us_byte*)malloc(sizeof(us_byte));
    memset(t_fi_data, 0x00, sizeof(us_byte));
    memset(t_fi_data, (us_short)iType, sizeof(us_byte));

    int iResult = 0, iReadRst = 0, i_len = 0;
    iResult = execCmdWrite(0xF0, t_fi_data, 0x01, 0x01, iFlag, 0x00);
    free(t_fi_data);
    if ((0xDFFF < iResult) && (iResult < 0xEFFF)) return iResult;

    us_byte *pRevData = (us_byte*)malloc(BUFFER_MAX_LENGTH * sizeof(char));
    memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));
    iReadRst = execCmdRead(pRevData, i_len, READ_BUS_CONFIRM_TIMEOUT);

    iResult = (0 == iReadRst) ? ERR_DEVICE_BUSY : 0;
    iResult = execCfmChk(pRevData, 0x00, iFlag);
    if (0 != iResult) return iResult;

    iResult = execCmdCfm(0x01, iFlag);

    // -- -- -- --
    int t_length = 0, t_total = 0;
    do {
        memset(pRevData, 0x00, BUFFER_MAX_LENGTH * sizeof(char));

        iReadRst = usbBusRead(pRevData, &i_len);

        if (0 == iReadRst) continue;

        // count data length
        int t_single = ((int)pRevData[4] << 8) + (int)pRevData[3];
        if (0 == t_total) {
            t_total +=  (int)pRevData[5];
            t_total += ((int)pRevData[6] << 8);
            t_total += ((int)pRevData[7] << 16);
            t_total += ((int)pRevData[8] << 24);
        }

        memcpy(iData + t_length, pRevData + 12, t_single * sizeof(char));
        t_length += t_single;
    } while (t_length < t_total);
    iLength = t_total;

    free(pRevData);
    return 0x00;
}
