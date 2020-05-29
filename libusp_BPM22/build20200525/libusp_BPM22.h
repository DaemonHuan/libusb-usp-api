/*
 * libusp_bpm22.h
 *
 *  Created on: 2020��5��22��
 *      Author: JK
 */

#ifndef __LIBUSP_BPM22_H__
#define __LIBUSP_BPM22_H__

#if !defined(__cplusplus)
#define inline __inline
#endif

///* on windows with mingw32 */
#if defined(__GNUC__)
	#define PRE_CDECL
	#define POST_CDECL __attribute__((cdecl))
	#define PRE_STDCALL
	#define POST_STDCALL __attribute__((stdcall))
	#define PRE_FASTCALL
	#define POST_FASTCALL __attribute__((fastcall))

	#if defined(LIBUSP_BPM22_EXPORTS)
		#define LIBUSP_BPM22_API __attribute__ ((visibility("default")))
	#else
		#define LIBUSP_BPM22_API
	#endif
#else
	#define PRE_CDECL __cdecl
	#define POST_CDECL
	#define PRE_STDCALL __stdcall
	#define POST_STDCALL
	#define PRE_FASTCALL __fastcall
	#define POST_FASTCALL

	#if defined(LIBUSP_BPM22_EXPORTS)
		#define LIBUSP_BPM22_API __declspec(dllexport)
	#else
		#define LIBUSP_BPM22_API __declspec(dllimport)
	#endif
#endif /* __GNUC__ */

//#if defined(LIBUSP_BPM22_EXPORTS)
//	#define LIBUSP_BPM22_API __declspec(dllexport)
//#else
//	#define LIBUSP_BPM22_API __declspec()
//#endif

typedef unsigned char BYTE;
// 6.2. ״̬�ṹ��
typedef struct DeviceStatus {
    BYTE iSize; //�ṹ���С��������չ
    BYTE iType; //Ʊ������
    BYTE iState; //�豸����״̬
    BYTE bError; //��������
    BYTE bLastFault; //���һ��������
    BYTE bRetrackError; //��Ʊ��ȡ��
    BYTE bRetrackHavePaper; //��Ʊ����Ʊ
    BYTE bBox1HavePaper; //Ʊ�� 1 ��Ʊ
    BYTE bBox2HavePaper; //Ʊ�� 2 ��Ʊ
    BYTE bBox3HavePaper; //Ʊ�� 3 ��Ʊ
    BYTE bBox4HavePaper; //Ʊ�� 4 ��Ʊ
    BYTE bBox1Exist; //Ʊ�� 1 ����
    BYTE bBox2Exist; //Ʊ�� 2 ����
    BYTE bBox3Exist; //Ʊ�� 3 ����
    BYTE bBox4Exist; //Ʊ�� 4 ����
    BYTE bFeedCheckOpen; //��ֽģ���
    BYTE bOCRCoverOpen; //OCR ģ���ϸǴ�
    BYTE bPrintHeadOpen; //��ӡģ��̧��
    BYTE bScanUnitOpen; //ɨ��ģ���ϸǴ�
    BYTE bOutputBoxOpen; //��Ʊ���ϸǴ�
    BYTE bGateOpen; //բ�Ŵ�
    BYTE bChannelHasPaper; //ͨ����Ʊ
    BYTE bWaitOCRHasPaper; //�ȴ� OCR λ����Ʊ
    BYTE bOCRHasPaper; //OCR λ����Ʊ
    BYTE bWaitPrintHasPaper; //�ȴ���ӡλ����Ʊ
    BYTE bScanHasPaper; //ɨ��λ����Ʊ
    BYTE bWaitOutHasPaper; //�ȴ���Ʊλ����Ʊ
    BYTE bStackerHavePaper; //��Ʊ����Ʊ
    BYTE bGateError; //բ�Ŵ���
    BYTE bBoardError; //ѹ�����
    BYTE bStampError; //��Ʊ�´���
    BYTE bHeadError; //��ӡͷ̧��ѹ�´���
    BYTE bCorrectResetError; //��ƫģ�����
    BYTE bChannelError; //ͨ��ѹƱ����
    BYTE bPrinterError; //��ӡ��ͨ�Ŵ���
    BYTE bBeltLackError; //̼���쳣
    BYTE bBeltOverError; //̼������
    BYTE bTempVolError; //̼����������
    BYTE bMotorError; //��������쳣
    BYTE bMotorComFaiL; //�����ͨ��ʧ��
    BYTE bPaperJamError; //��ֽ����
    BYTE bFeedingError; //Ʊ���ֽʧ��
    BYTE bPaperLengthError; //Ʊ�ݳ��ȴ���
    BYTE bDoubleDocError; //���Ŵ���
    BYTE bCorrectionError; //��ƫ����
    BYTE bHeatOpen; //����ģ���
    BYTE bMagnetOn; //����ֿ��
    BYTE bHaveData; //��ɨ������
    BYTE bRibbonEmpty; //̼��ȱʧ
    BYTE bBackDoorOpen; //���Ŵ�
    BYTE bFPGAError; //FPGA ͨ�Ŵ���
    BYTE bRetrackHasPaper; //��Ʊͨ����Ʊ����Ʊ����ڴ�
    BYTE bCorrectionHasPaper; //��ƫλ����Ʊ
    BYTE bRecycleHasPaper; //��ȡ������Ʊ
    BYTE bVoltError; //��ѹ����
    BYTE bReserved1; //����λ
}tDeviceStatus;

//Ʊ����Ϣ�ṹ��
typedef struct CheckInfo
{
    int iCheckSN;									//Ʊ�ݱ��
    int iCheckCoding;								//Ʊ�ݱ���
    int iCheckLen;									//Ʊ�ݳ���
    int iCheckWid;									//Ʊ�ݿ��
}tCheckInfo;

//������ṹ��
typedef struct DeviceConfig
{
    int iUpdateFlag;								//������־
    int iBoxNum;									//Ʊ����
    int iCorrectDif;								//������ƫƫ��
    int	iReserve1;									//����
    int	iReserve2;
    int	iReserve3;
    int	iReserve4;
    int	iReserve5;
    tCheckInfo iCheckInfo[10];
}tDeviceConfig;

typedef struct sBill_Identify_Info
{
    int   BillType;          //��оƬ���浥4  ��ͨ�浥2  ת��֧Ʊ0  �ֽ�֧Ʊ1 ����֧Ʊ3
    int   BankCode;          //���б��루��֧��֧Ʊ������֧Ʊ0  ������-1��Ŀǰũ��֧Ʊ0 ����֧Ʊ1��
    int   IsFactComp;        //Ҫ����ȫ���
    int   IsIdentification;     //��α���
    int   IsDirection;       //Ʊ�ݷ��÷��� Ʊ�ݷ��÷���0���������ң�δ����1���������ң�����2����������δ����3���������󣬵���
    int   isIdentify;        //�Ƿ���Ҫ��α��ֻ����ַ�����  ��0 ����1
    char qr_code[128];    //��ά����

    char ocr_bdrticket[32];		//Ʊ��
    char ocr_bdraccount[32];		//�˺�
    char ccr_code[32];				//ӡ�´���

    char ocr_numcode_01[32];  		//��Ʊ��
    char ocr_numcode_02[32]; 		//��Ʊ��
    char ocr_drawerAct[32];     //��Ʊ���˺�
    char ocr_amtFigures[128];    //Сд���
    char ocr_amtWords[128];      //��д���
    char ocr_password[32];      //����
    char ocr_bankcode[32];    //�к�
    char ocr_issueDate[32];     //��Ʊ����
    char ocr_payee[128];         // �տ���
    char ocr_payBankName[128];   //����������
    char ocr_endorser[32];     //������ǩ��
    char ocr_payeeAndIDCarD[128];   //�տ������ֺ����֤����

}SBILLIDENTIFYINFO;

#include "g_header.h"

#ifdef __cplusplus
extern "C"
{
#endif

//    LIBUSP_BPM22_API int dll_test(int aa);

    LIBUSP_BPM22_API int openDevice();

    LIBUSP_BPM22_API void closeDevice();

    LIBUSP_BPM22_API int feedCheck(int iCheckBox, int iPaperType);

    LIBUSP_BPM22_API int ocrScan(const char *fpRGB, int pType, char *ocrNum);

    LIBUSP_BPM22_API int execPrintScan_withJson(const char *json_str, int t_length,
                                   int iPrintEnabled, int iScanEnabled, int iPrintNext);

    LIBUSP_BPM22_API int printAndScan(int iPrintEnabled, int iScanEnabled, int iPrintNext,
                                   int iCheckLen, int iCheckWid);

    LIBUSP_BPM22_API int outPaperDoorControl(int iCloseDoor);

    LIBUSP_BPM22_API int getIdentifyInfo();

    LIBUSP_BPM22_API int getOneImage(const char* cImageFileName,
                                   const IMAGETYPE &type, const IMAGE_FORMAT &iFormat);

    LIBUSP_BPM22_API int getDevStatus(DeviceStatus &m_DeviceStatus);

    LIBUSP_BPM22_API int takeCheck(int iTimeout);

    LIBUSP_BPM22_API int setCheckOut(int mode, int stamp);

    LIBUSP_BPM22_API int resetDevEx(int bSoftReset);

    LIBUSP_BPM22_API int getFirmwareVersion(char *version, int &bufSize);

    LIBUSP_BPM22_API int updateFirmware(const char * cFileName);

    LIBUSP_BPM22_API int recycleCheck();

    LIBUSP_BPM22_API int getLastErrorCode();

    LIBUSP_BPM22_API int readConfigDev(DeviceConfig &m_DeviceConfig);

    LIBUSP_BPM22_API int writeConfigDev(DeviceConfig &m_DeviceConfig);

    LIBUSP_BPM22_API int cisConfigDev(const char *cFileName);

    LIBUSP_BPM22_API int cisLightConfigDev(const char *cLightConfig, int cfgLength);

    LIBUSP_BPM22_API int picTypeConfigDev(int iPicType);

    LIBUSP_BPM22_API int calibrationCIS(int iBoxNum);

    LIBUSP_BPM22_API int b_OcrIdentify(SBILLIDENTIFYINFO *sInfo, char *iFrontImage, char *iRearImage);

    // Debug model
    LIBUSP_BPM22_API int dm_startCIS(int i_value);

    LIBUSP_BPM22_API int dm_motoPower(int i_value);

    LIBUSP_BPM22_API int dm_printerPower(int i_value);

    LIBUSP_BPM22_API int dm_heatingPower(int i_value);

    LIBUSP_BPM22_API int dm_setDebugModel(int i_value);

    LIBUSP_BPM22_API int dm_readHeatTemperature(int &i_value);

    LIBUSP_BPM22_API int dm_readVoltage(int &i_value);

    LIBUSP_BPM22_API int dm_readButtonValue(int &i_value);

    LIBUSP_BPM22_API int dm_setLedValue(int i_num, int i_value);

    LIBUSP_BPM22_API int dm_setWarnValue(int i_value);

    LIBUSP_BPM22_API int dm_r_FRAM(int address, char *i_value, int &i_size);
    LIBUSP_BPM22_API int dm_w_FRAM(int address, char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_r_SPIFLASH(int address, char *i_value, int &i_size);
    LIBUSP_BPM22_API int dm_w_SPIFLASH(int address, char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_r_DDR(int address, char *i_value, int &i_size);
    LIBUSP_BPM22_API int dm_w_DDR(int address, char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_getSensorStatus(char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_getPowerStatus(char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_getPrinterStatus(char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_getVerion_ARM(char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_getVerion_DSP(char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_getVerion_FPGA(char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_getVerion_STM32(char *i_value, int &i_size);

    LIBUSP_BPM22_API int dm_dotest(us_short t_fi);

#ifdef __cplusplus
}
#endif
#endif /* SRC_LIBUSP_BPM22_H_ */
