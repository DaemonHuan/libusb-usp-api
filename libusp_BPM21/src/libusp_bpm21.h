/*
 * libusp_bpm21.h
 *
 *  Created on: 2020��5��22��
 *      Author: JK
 */

#ifndef __LIBUSP_BPM21_H__
#define __LIBUSP_BPM21_H__

/* on MS environments, the inline keyword is available in C++ only */
#if !defined(__cplusplus)
#define inline __inline
#endif

/* on windows with mingw32 */
#if defined(__GNUC__)
	#define PRE_CDECL
	#define POST_CDECL __attribute__((cdecl))
	#define PRE_STDCALL
	#define POST_STDCALL __attribute__((stdcall))
	#define PRE_FASTCALL
	#define POST_FASTCALL __attribute__((fastcall))

	#if defined(LIBUSP_BPM21_EXPORTS)
		#define LIBUSP_BPM21_API __attribute__ ((visibility("default")))
	#else
		#define LIBUSP_BPM21_API
	#endif
#else
	#define PRE_CDECL __cdecl
	#define POST_CDECL
	#define PRE_STDCALL __stdcall
	#define POST_STDCALL
	#define PRE_FASTCALL __fastcall
	#define POST_FASTCALL

	#if defined(LIBUSP_BPM21_EXPORTS)
		#define LIBUSP_BPM21_API __declspec(dllexport)
	#else
		#define LIBUSP_BPM21_API __declspec(dllimport)
	#endif
#endif /* __GNUC__ */

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
    int iCheckWid;									//Ʊ�ݿ���
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
    char ocr_payeeAndIDCarD[128];   //�տ������ֺ�����֤����

}SBILLIDENTIFYINFO;

//#include "g_header.h"

#ifdef __cplusplus
extern "C"
{
#endif

	LIBUSP_BPM21_API int PRE_CDECL dll_test(int aa);

//    LIBUSP_BPM22_API int PRE_CDECL openDevice() POST_CDECL;
//
//    LIBUSP_BPM22_API void PRE_CDECL closeDevice() POST_CDECL;

#ifdef __cplusplus
}
#endif
#endif /* SRC_LIBUSP_BPM21_H_ */