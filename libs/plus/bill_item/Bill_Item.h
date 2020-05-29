#ifndef BILL_ITEM_API
#define BILL_ITEM_API extern "C" _declspec (dllexport)
#endif

//#include <opencv2/opencv.hpp>  
//using namespace cv ;

typedef struct BillResultstatistics
{
	char  Ocr_res[1000];     //各ocr字符串结果
	char  Qrcode_res[80];    //二维码结果
	char  FactorComp_res[18];//各要素点结果
	char  Identify_res[16];     //各鉴伪点结果
	int   BillType;          //大额（芯片）存单4  普通存单2  转账支票0  现金支票1 其他支票3
	int   IsFactComp;        //要素齐全结果
	int   IsIdentification;     //鉴伪结果
	int   BankCode;          //银行编码（仅支持支票）本行支票0  其他行1（目前农行支票0 他行支票1）
	int   IsDirection;       //票据放置方向 票据放置方向0：正面向右，未倒置1：正面向右，倒置2：正面向左，未倒置3：正面向左，倒置
    int   isIdentify;        //是否需要鉴伪（只输出字符串）  是0 ，否1
}BillRESULTSTATISTICS;


 //int Bill_item(Mat &srcIRT,Mat &srcIR,Mat &srcRGB,Mat &srcUV,Mat &srcUVTR,int idNum,BillRESULTSTATISTICS& BillResult);

