#pragma once
#include <vector>
#include <string.h>


using std::vector;
using std::string;

// CShowImgIntoFrame �Ի���

class CShowImgIntoFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CShowImgIntoFrame)

public:
	CShowImgIntoFrame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowImgIntoFrame();

// �Ի�������
	enum { IDD = IDD_GWSYSTEM10_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	//��15��ͼƬ��ʾ����ӱ���
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

	//����15��ͼƬ��ʾ���Ӧ������
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

	vector <string>  imgNames;//װ��ÿ��ͼƬ·��������
	vector<UINT> FrameName;//װ��ÿ���߿�ID������
	vector<CRect> rect; //����15��ͼƬ��ʾ�������

public:
	virtual BOOL OnInitDialog();
	// �����ļ����������ͼƬ·��
	void FindAllImgFilePath();
};
