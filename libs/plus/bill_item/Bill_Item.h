#ifndef BILL_ITEM_API
#define BILL_ITEM_API extern "C" _declspec (dllexport)
#endif

//#include <opencv2/opencv.hpp>  
//using namespace cv ;

typedef struct BillResultstatistics
{
	char  Ocr_res[1000];     //��ocr�ַ������
	char  Qrcode_res[80];    //��ά����
	char  FactorComp_res[18];//��Ҫ�ص���
	char  Identify_res[16];     //����α����
	int   BillType;          //��оƬ���浥4  ��ͨ�浥2  ת��֧Ʊ0  �ֽ�֧Ʊ1 ����֧Ʊ3
	int   IsFactComp;        //Ҫ����ȫ���
	int   IsIdentification;     //��α���
	int   BankCode;          //���б��루��֧��֧Ʊ������֧Ʊ0  ������1��Ŀǰũ��֧Ʊ0 ����֧Ʊ1��
	int   IsDirection;       //Ʊ�ݷ��÷��� Ʊ�ݷ��÷���0���������ң�δ����1���������ң�����2����������δ����3���������󣬵���
    int   isIdentify;        //�Ƿ���Ҫ��α��ֻ����ַ�����  ��0 ����1
}BillRESULTSTATISTICS;


 //int Bill_item(Mat &srcIRT,Mat &srcIR,Mat &srcRGB,Mat &srcUV,Mat &srcUVTR,int idNum,BillRESULTSTATISTICS& BillResult);

