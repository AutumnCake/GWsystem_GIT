#pragma once
#include <vector>
#include <string.h>


using std::vector;
using std::string;

// CShowImgIntoFrame 对话框

class CShowImgIntoFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CShowImgIntoFrame)

public:
	CShowImgIntoFrame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowImgIntoFrame();

// 对话框数据
	enum { IDD = IDD_GWSYSTEM10_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	//给15个图片显示框添加变量
	CStatic m_frame1;
	CStatic m_frame2;
	CStatic m_frame3;
	CStatic m_frame4;
	CStatic m_frame5;
	CStatic m_frame6;
	CStatic m_frame7;
	CStatic m_frame8;
	CStatic m_frame9;
	CStatic m_frame10;
	CStatic m_frame11;
	CStatic m_frame12;
	CStatic m_frame13;
	CStatic m_frame14;
	CStatic m_frame15;

	//定义15个图片显示框对应的区域
	CRect rect1;
	CRect rect2;
	CRect rect3;
	CRect rect4;
	CRect rect5;
	CRect rect6;
	CRect rect7;
	CRect rect8;
	CRect rect9;
	CRect rect10;
	CRect rect11;
	CRect rect12;
	CRect rect13;
	CRect rect14;
	CRect rect15;

	vector <string>  imgNames;//装载每张图片路径的数组
	vector<UINT> FrameName;//装载每个边框ID的数组
	vector<CRect> rect; //定义15个图片显示框的区域

public:
	virtual BOOL OnInitDialog();
	// 查找文件夹里的所有图片路径
	void FindAllImgFilePath();
};
