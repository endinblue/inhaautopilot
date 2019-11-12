#pragma once

// define the following 4 definition to remove error #include dtrans.h
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__


#include <sstream>
//#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include <highgui.h>
#include <cv.h>
#include <windows.h>
#include <dshow.h>	//include for getting device ID of USB...
#include "stdafx.h"
#include "bloblabeling.h"
#include "thinning.h"
//#include <initguid.h>
#include <nb30.h>
#pragma comment(lib, "strmiids")  //include for getting device ID of USB...
using namespace std;
//using namespace cv;


CvScalar myGreenColor = cvScalar( 0, 255, 0 );
CvScalar myRedColor	= cvScalar( 0, 0, 255 );		

int			m_candidateLabel[100];	
int gCamFlag=0;
int gRunCamState=0;
int UNLOCK=0;
int gShow_Img_flag=0;


int resolution=0;
#define HIGH	100
#define LOW		20

int resultNum[8][2];//인식된 결과가 들어감 
int digit;//인식된 결과의 자릿수를 나타냄

// screen size
const int m_pImage_WIDTH = 640;
const int m_pImage_HEIGHT = 480;

int	IMG_WIDTH_CAM	=	640;	// 카메라에 따라 변할 수 있다. 640, 720, 1024, 1280, etc.
int	IMG_HEIGHT_CAM	=	480;	// 카메라에 따라 변할 수 있다. 480, 576, 768, 1024, etc.

int REAL_WIN_WIDTH = 640;	// 프로그램창의 가로는 변하지 않는다.고정시킴.
int REAL_WIN_HEIGHT = 480;	// 세로는 이미지의 가로세로비에 따라 변한다.


	IplImage*	gray;
	IplImage*	bnw;
	
	IplImage*	binary;
	IplImage*	end1=NULL;


	CvvImage	m_cImage;
	CvvImage	m_gray;
	CvvImage	m_binary;
	CvvImage	m_lable;

	IplImage*   img[10];

	int gDetBlobCnt=0;


CvCapture *capture=NULL;
IplImage *m_pImage=NULL;
//CvCapture* m_pCapture;//화면출력용
IplImage* labelImage;//레이블링이 표시되는 이미지 변수
IplImage* gTemplateImg; 

namespace write_nfctag {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::IO;
	using namespace System::Data::OleDb;
	using namespace System::Runtime::InteropServices;
	using namespace System::Text::RegularExpressions;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace blobing;
	using namespace thin;

	/// <summary>
	/// Form1에 대한 요약입니다.
	///
	/// 경고: 이 클래스의 이름을 변경하면 이 클래스가 의존하는
	///          모든 .resx 파일과 관련된 관리되는 리소스 컴파일러 도구의
	///          '리소스 파일 이름' 속성도 변경해야 합니다. 그렇지 않은 경우
	///          디자이너는 이 폼과 관련된 지역화된 리소스와
	///          올바르게 상호 작용할 수 없습니다.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

			

			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			
			//cal_file_open();
			//
			gCamFlag = find_valid_cam();

			if(gCamFlag==0)
			{
				MessageBox::Show(gCamFlag.ToString() +" NO CAM");
			}
			

			if(gCamFlag!=0)
			{
				capture = cvCaptureFromCAM(0);
			}

			//capture
			if(capture !=NULL) 
			{
				timer_cam->Start();

				cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH,m_pImage_WIDTH);
				cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT,m_pImage_HEIGHT);
				gRunCamState=1;
			}
			else
			{
				gRunCamState=0;
			}

			blob = gcnew blobing::CBlobLabeling;
			gTemplateImg = cvLoadImage("./digit/ten.bmp", -1);


			// init 
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  PB_CAM;
	protected: 

	protected: 
	private: System::Windows::Forms::Timer^  timer_cam;
	private: System::Windows::Forms::RichTextBox^  RB_RESULT;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::PictureBox^  PB_NUM;
	private: System::Windows::Forms::RichTextBox^  RB_RESULT2;
	private: System::Windows::Forms::NumericUpDown^  NUM_OUTER;

	private: System::Windows::Forms::CheckBox^  CB_INVERSE;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  NUM_INNER;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  NUM_THICK;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  NUM_BIN_THRESHOLD;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::NumericUpDown^  NUM_SMALL_X;
	private: System::Windows::Forms::NumericUpDown^  NUM_SMALL_Y;
	private: System::Windows::Forms::NumericUpDown^  NUM_BIG_X;
	private: System::Windows::Forms::NumericUpDown^  NUM_BIG_Y;
	private: System::Windows::Forms::CheckBox^  CB_USE_PIC;
	private: System::Windows::Forms::NumericUpDown^  NUM_DILATE;
	private: System::Windows::Forms::Label^  label9;


	private:
	private: blobing::CBlobLabeling^ blob;
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->PB_CAM = (gcnew System::Windows::Forms::PictureBox());
			this->timer_cam = (gcnew System::Windows::Forms::Timer(this->components));
			this->RB_RESULT = (gcnew System::Windows::Forms::RichTextBox());
			this->PB_NUM = (gcnew System::Windows::Forms::PictureBox());
			this->RB_RESULT2 = (gcnew System::Windows::Forms::RichTextBox());
			this->NUM_OUTER = (gcnew System::Windows::Forms::NumericUpDown());
			this->CB_INVERSE = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->NUM_INNER = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->NUM_THICK = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->NUM_BIN_THRESHOLD = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->NUM_SMALL_X = (gcnew System::Windows::Forms::NumericUpDown());
			this->NUM_SMALL_Y = (gcnew System::Windows::Forms::NumericUpDown());
			this->NUM_BIG_X = (gcnew System::Windows::Forms::NumericUpDown());
			this->NUM_BIG_Y = (gcnew System::Windows::Forms::NumericUpDown());
			this->CB_USE_PIC = (gcnew System::Windows::Forms::CheckBox());
			this->NUM_DILATE = (gcnew System::Windows::Forms::NumericUpDown());
			this->label9 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PB_CAM))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PB_NUM))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_OUTER))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_INNER))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_THICK))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_BIN_THRESHOLD))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_SMALL_X))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_SMALL_Y))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_BIG_X))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_BIG_Y))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_DILATE))->BeginInit();
			this->SuspendLayout();
			// 
			// PB_CAM
			// 
			this->PB_CAM->Location = System::Drawing::Point(26, 35);
			this->PB_CAM->Name = L"PB_CAM";
			this->PB_CAM->Size = System::Drawing::Size(640, 480);
			this->PB_CAM->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->PB_CAM->TabIndex = 0;
			this->PB_CAM->TabStop = false;
			// 
			// timer_cam
			// 
			this->timer_cam->Interval = 500;
			this->timer_cam->Tick += gcnew System::EventHandler(this, &Form1::timer_cam_Tick);
			// 
			// RB_RESULT
			// 
			this->RB_RESULT->Location = System::Drawing::Point(691, 35);
			this->RB_RESULT->Name = L"RB_RESULT";
			this->RB_RESULT->Size = System::Drawing::Size(241, 77);
			this->RB_RESULT->TabIndex = 1;
			this->RB_RESULT->Text = L"";
			// 
			// PB_NUM
			// 
			this->PB_NUM->Location = System::Drawing::Point(691, 196);
			this->PB_NUM->Name = L"PB_NUM";
			this->PB_NUM->Size = System::Drawing::Size(240, 160);
			this->PB_NUM->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->PB_NUM->TabIndex = 2;
			this->PB_NUM->TabStop = false;
			// 
			// RB_RESULT2
			// 
			this->RB_RESULT2->Location = System::Drawing::Point(691, 118);
			this->RB_RESULT2->Name = L"RB_RESULT2";
			this->RB_RESULT2->Size = System::Drawing::Size(241, 72);
			this->RB_RESULT2->TabIndex = 3;
			this->RB_RESULT2->Text = L"";
			// 
			// NUM_OUTER
			// 
			this->NUM_OUTER->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->NUM_OUTER->Location = System::Drawing::Point(747, 386);
			this->NUM_OUTER->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {250, 0, 0, 0});
			this->NUM_OUTER->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->NUM_OUTER->Name = L"NUM_OUTER";
			this->NUM_OUTER->Size = System::Drawing::Size(77, 21);
			this->NUM_OUTER->TabIndex = 4;
			this->NUM_OUTER->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// CB_INVERSE
			// 
			this->CB_INVERSE->AutoSize = true;
			this->CB_INVERSE->Checked = true;
			this->CB_INVERSE->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CB_INVERSE->Location = System::Drawing::Point(830, 387);
			this->CB_INVERSE->Name = L"CB_INVERSE";
			this->CB_INVERSE->Size = System::Drawing::Size(65, 16);
			this->CB_INVERSE->TabIndex = 5;
			this->CB_INVERSE->Text = L"Inverse";
			this->CB_INVERSE->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(689, 386);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 12);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Outer";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(689, 416);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(33, 12);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Inner";
			// 
			// NUM_INNER
			// 
			this->NUM_INNER->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->NUM_INNER->Location = System::Drawing::Point(747, 413);
			this->NUM_INNER->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {250, 0, 0, 0});
			this->NUM_INNER->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->NUM_INNER->Name = L"NUM_INNER";
			this->NUM_INNER->Size = System::Drawing::Size(77, 21);
			this->NUM_INNER->TabIndex = 4;
			this->NUM_INNER->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(689, 449);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(64, 12);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Thickness";
			// 
			// NUM_THICK
			// 
			this->NUM_THICK->Location = System::Drawing::Point(759, 440);
			this->NUM_THICK->Name = L"NUM_THICK";
			this->NUM_THICK->Size = System::Drawing::Size(77, 21);
			this->NUM_THICK->TabIndex = 4;
			this->NUM_THICK->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(692, 476);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(57, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"threshold";
			// 
			// NUM_BIN_THRESHOLD
			// 
			this->NUM_BIN_THRESHOLD->Location = System::Drawing::Point(759, 467);
			this->NUM_BIN_THRESHOLD->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {200, 0, 0, 0});
			this->NUM_BIN_THRESHOLD->Name = L"NUM_BIN_THRESHOLD";
			this->NUM_BIN_THRESHOLD->Size = System::Drawing::Size(77, 21);
			this->NUM_BIN_THRESHOLD->TabIndex = 4;
			this->NUM_BIN_THRESHOLD->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {128, 0, 0, 0});
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(692, 496);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(49, 12);
			this->label5->TabIndex = 6;
			this->label5->Text = L"small_x";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(692, 523);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(49, 12);
			this->label6->TabIndex = 6;
			this->label6->Text = L"small_y";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(692, 550);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(35, 12);
			this->label7->TabIndex = 6;
			this->label7->Text = L"big_x";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(692, 577);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(35, 12);
			this->label8->TabIndex = 6;
			this->label8->Text = L"big_y";
			// 
			// NUM_SMALL_X
			// 
			this->NUM_SMALL_X->DecimalPlaces = 2;
			this->NUM_SMALL_X->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->NUM_SMALL_X->Location = System::Drawing::Point(759, 494);
			this->NUM_SMALL_X->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->NUM_SMALL_X->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->NUM_SMALL_X->Name = L"NUM_SMALL_X";
			this->NUM_SMALL_X->Size = System::Drawing::Size(77, 21);
			this->NUM_SMALL_X->TabIndex = 4;
			this->NUM_SMALL_X->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 65536});
			// 
			// NUM_SMALL_Y
			// 
			this->NUM_SMALL_Y->DecimalPlaces = 2;
			this->NUM_SMALL_Y->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->NUM_SMALL_Y->Location = System::Drawing::Point(759, 521);
			this->NUM_SMALL_Y->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->NUM_SMALL_Y->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->NUM_SMALL_Y->Name = L"NUM_SMALL_Y";
			this->NUM_SMALL_Y->Size = System::Drawing::Size(77, 21);
			this->NUM_SMALL_Y->TabIndex = 4;
			this->NUM_SMALL_Y->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 131072});
			// 
			// NUM_BIG_X
			// 
			this->NUM_BIG_X->DecimalPlaces = 2;
			this->NUM_BIG_X->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->NUM_BIG_X->Location = System::Drawing::Point(759, 548);
			this->NUM_BIG_X->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->NUM_BIG_X->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->NUM_BIG_X->Name = L"NUM_BIG_X";
			this->NUM_BIG_X->Size = System::Drawing::Size(77, 21);
			this->NUM_BIG_X->TabIndex = 4;
			this->NUM_BIG_X->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {9, 0, 0, 65536});
			// 
			// NUM_BIG_Y
			// 
			this->NUM_BIG_Y->DecimalPlaces = 2;
			this->NUM_BIG_Y->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->NUM_BIG_Y->Location = System::Drawing::Point(759, 575);
			this->NUM_BIG_Y->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->NUM_BIG_Y->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->NUM_BIG_Y->Name = L"NUM_BIG_Y";
			this->NUM_BIG_Y->Size = System::Drawing::Size(77, 21);
			this->NUM_BIG_Y->TabIndex = 4;
			this->NUM_BIG_Y->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {8, 0, 0, 65536});
			// 
			// CB_USE_PIC
			// 
			this->CB_USE_PIC->AutoSize = true;
			this->CB_USE_PIC->Checked = true;
			this->CB_USE_PIC->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CB_USE_PIC->Location = System::Drawing::Point(830, 363);
			this->CB_USE_PIC->Name = L"CB_USE_PIC";
			this->CB_USE_PIC->Size = System::Drawing::Size(92, 16);
			this->CB_USE_PIC->TabIndex = 7;
			this->CB_USE_PIC->Text = L"test with pic";
			this->CB_USE_PIC->UseVisualStyleBackColor = true;
			// 
			// NUM_DILATE
			// 
			this->NUM_DILATE->Location = System::Drawing::Point(498, 567);
			this->NUM_DILATE->Name = L"NUM_DILATE";
			this->NUM_DILATE->Size = System::Drawing::Size(77, 21);
			this->NUM_DILATE->TabIndex = 8;
			this->NUM_DILATE->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(427, 569);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(47, 12);
			this->label9->TabIndex = 6;
			this->label9->Text = L"dilate수";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(944, 612);
			this->Controls->Add(this->NUM_DILATE);
			this->Controls->Add(this->CB_USE_PIC);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->CB_INVERSE);
			this->Controls->Add(this->NUM_SMALL_Y);
			this->Controls->Add(this->NUM_BIG_Y);
			this->Controls->Add(this->NUM_BIG_X);
			this->Controls->Add(this->NUM_SMALL_X);
			this->Controls->Add(this->NUM_BIN_THRESHOLD);
			this->Controls->Add(this->NUM_THICK);
			this->Controls->Add(this->NUM_INNER);
			this->Controls->Add(this->NUM_OUTER);
			this->Controls->Add(this->RB_RESULT2);
			this->Controls->Add(this->PB_NUM);
			this->Controls->Add(this->RB_RESULT);
			this->Controls->Add(this->PB_CAM);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PB_CAM))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PB_NUM))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_OUTER))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_INNER))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_THICK))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_BIN_THRESHOLD))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_SMALL_X))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_SMALL_Y))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_BIG_X))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_BIG_Y))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NUM_DILATE))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void myShowImage(IplImage *img, System::Windows::Forms::PictureBox^ picturebox){
			gShow_Img_flag=0;

			try
			{
				delete picturebox->Image;
				picturebox->Image = gcnew Bitmap(img->width,  img->height, img->widthStep, System::Drawing::Imaging::PixelFormat::Format24bppRgb, (IntPtr)img->imageData);
			
			}
			catch(Exception ^e){}

			gShow_Img_flag=1;
		 }

	private: System::Void timer_cam_Tick(System::Object^  sender, System::EventArgs^  e) {
			if(gRunCamState==1)
			{
				if(CB_USE_PIC->Checked)
					m_pImage =cvLoadImage("./digit/test.bmp", -1);
				else
					m_pImage = cvQueryFrame(capture);

				myShowImage (m_pImage, PB_CAM);	// show captured image of cam to screen
				
				if(m_pImage !=NULL)
				{
					if(gShow_Img_flag==1)
						detect_number();
				}
				Sleep(1);
			}
		}

	private: void detect_number()
	{
				 detect_show_number();	// total action
				 
				 //verify_numarea();
				 //detect_numboard();
		}

	//-------------------------------------------------------------------------
	//	void detect_show_number()
	//-------------------------------------------------------------------------
	void detect_show_number()
	{

		int  opt[100];
		gray = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
		bnw = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
		cvCvtColor(m_pImage, bnw, CV_RGB2GRAY);
		cvNamedWindow("gray");	cvShowImage("gray", bnw);

		int bin_thrh = (int)NUM_BIN_THRESHOLD->Value;

		/*
		if(CB_INVERSE->Checked)
		{
			try{ cvThreshold(bnw, bnw, bin_thrh, 255.0, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);	}catch(Exception ^e){}
		}
		else
		{
			try{cvThreshold(bnw, bnw, bin_thrh, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU); }catch(Exception ^e){}
		}
		*/
		
		IplImage* labeled = cvCreateImage( cvSize( bnw->width, bnw->height ), 8, 3 );//레이블링 이미지
		cvCvtColor(bnw, labeled, CV_GRAY2BGR );//그레이를 RGB로 바꿔준다.
		cvErode(bnw, bnw, NULL, 1);

		IplConvKernel *element;
		element = cvCreateStructuringElementEx(11, 11, 6, 6, CV_SHAPE_RECT, NULL); // 필터의 크기를 11x11로 설정
		//element = cvCreateStructuringElementEx(3, 3, 2, 2, CV_SHAPE_CROSS, NULL); // 필터의 크기를 11x11로 설정
		//cvMorphologyEx(bnw, gray, NULL, element, CV_MOP_TOPHAT, 1);
		cvMorphologyEx(bnw, gray, NULL, element, CV_MOP_BLACKHAT, 1);
		
		//cvDilate(gray, gray, NULL, 1);

		cvSub(bnw, gray, gray);
		
		
		
		if(CB_INVERSE->Checked)
		{
			try{ cvThreshold(gray, gray, bin_thrh, 255.0, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);	}catch(Exception ^e){}
		}
		else
		{
			try{cvThreshold(gray, gray, bin_thrh, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU); }catch(Exception ^e){}
		}

		

		
		// 레이블링 알고리즘 사용
		//CBlobLabeling^ blob = gcnew blobing::CBlobLabeling;
		
		//blob->SetParam(gray, (int)NUM_OUTER->Value );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정
		blob->SetParam(gray, (int)NUM_OUTER->Value );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정
		
		blob->DoLabeling(); //레이블링 실시
		
		
		
		// 잡영 제거 부분.
		int nMinWidth	= (int)((float)gray->width  * (float)NUM_SMALL_X->Value);
		int nMinHeight	= (int)((float)gray->width  * (float)NUM_SMALL_Y->Value);
		int nMaxWidth	= (int)((float)gray->width  * (float)NUM_BIG_X->Value);
		int nMaxHeight	= (int)((float)gray->height * (float)NUM_BIG_Y->Value);

		blob->BlobSmallSizeConstraint( nMinWidth, nMinHeight );
		blob->BlobBigSizeConstraint( nMaxWidth, nMaxHeight );
		
		RB_RESULT2->Text = "num of blob = "+blob->m_nBlobs.ToString()+"\n";

		try{cvThreshold(gray, gray, bin_thrh, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU); }catch(Exception ^e){}

		cvDilate(gray, gray, NULL, 1);

		cvNamedWindow("tophat");	cvShowImage("tophat", gray);

		for( int i=0; i < blob->m_nBlobs; i++ )
		{
			
			//레이블링된 각 위치값을 잡아주고
			CvPoint	pt1 = cvPoint(blob->m_recBlobs[i].x, blob->m_recBlobs[i].y);
			CvPoint pt2 = cvPoint(pt1.x + blob->m_recBlobs[i].width, pt1.y + blob->m_recBlobs[i].height);
			
			// 이미지 관심영역 설정
			cvSetImageROI(gray, blob->m_recBlobs[i]);

			IplImage* sub_gray = cvCreateImage(cvSize(blob->m_recBlobs[i].width, blob->m_recBlobs[i].height), 8, 1);
			
			if(CB_INVERSE->Checked)
			{
				try{cvThreshold(gray, sub_gray, 1, 255, CV_THRESH_BINARY_INV);	}catch(Exception ^e){}
				try{cvThreshold(gray, sub_gray, 1, 255, CV_THRESH_BINARY_INV);	}catch(Exception ^e){}
			}
			else
			{
				try{cvThreshold(gray, sub_gray, 1, 255, CV_THRESH_BINARY );	}catch(Exception ^e){}
			}
			
			// 관심영역 해제
			cvResetImageROI(gray);

			if(blob->m_nBlobs == 1)
			{
				;//cvNamedWindow("number board");	cvShowImage("number board",sub_gray);
				//myShowImage(sub_gray, PB_NUM);
			}

			////////////////////////////
			// 레이블링
			CBlobLabeling^ inner = gcnew blobing::CBlobLabeling;
			//inner->SetParam(sub_gray, 5);
			/*

			IplConvKernel *element;
			element = cvCreateStructuringElementEx (11, 11, 6, 6, CV_SHAPE_RECT, NULL); // 필터의 크기를 11x11로 설정

			cvDilate(sub_gray, sub_gray, element ,(int)NUM_DILATE->Value);
			cvErode(sub_gray, sub_gray, element ,(int)NUM_DILATE->Value);
			*/
			
			

			try{	cvThreshold(sub_gray, sub_gray, 1, 255, CV_THRESH_BINARY_INV);	}catch(Exception ^e){}

			inner->SetParam(sub_gray, (int)NUM_INNER->Value);

			inner->DoLabeling();
			/*

			int nSubMinWidth	= sub_gray->width * 5 / 10;
			int nSubMinHeight	= sub_gray->height  * 5 / 10;

			int nSubMaxWidth	= sub_gray->width  * 8 / 10;
			int nSubMaxHeight	= sub_gray->height * 8 / 10;

			inner->BlobSmallSizeConstraint( nSubMinWidth, nSubMinHeight );
			inner->BlobBigSizeConstraint( nSubMaxWidth, nSubMaxHeight );
			*/

			if(inner->m_nBlobs >4)
			{
				binary = cvCreateImage(cvSize( (sub_gray->width)*3, (int)(sub_gray->height)*3 ), IPL_DEPTH_8U, 1);
				cvResize(sub_gray,binary,CV_INTER_LINEAR );
				
				end1 = cvCreateImage( cvSize(binary->width, binary->height ), 8, 3 );//레이블링 이미지
				cvCvtColor(binary, end1, CV_GRAY2BGR );//그레이를 RGB로 바꿔준다.

				try{	cvThreshold(binary, binary, 1, 255, CV_THRESH_BINARY);	}catch(Exception ^e){}

				CBlobLabeling^ fire = gcnew blobing::CBlobLabeling;
				fire->SetParam(binary, 100);
				fire->DoLabeling();
				
				int nSwidth		= binary->width  * 5/100;	
				int nSheight	= binary->height * 30/100;	
				int nwidth	= binary->width  * 30/100;	
				int nheight	= binary->height * 70/100;	

				fire->BlobSmallSizeConstraint( nSwidth, nSheight );
				fire->BlobBigSizeConstraint( nwidth, nheight );
				
				int count = 0;

				for( int i=0; i < fire->m_nBlobs; i++ )
				{
					//이미지는 템플릿 사이즈로 조정해준다.
					//버퍼에 이미지를 받는다.
					//교환해서 리사이즈한다.
					//img[i] = cvCreateImage(cvSize(fire->m_recBlobs[i].width, fire->m_recBlobs[i].height), IPL_DEPTH_8U, 1);
					img[i] = cvCreateImage(cvSize(30, 45), IPL_DEPTH_8U, 1);
					//imgtemp[i] = cvCreateImage(cvSize(30, 45), IPL_DEPTH_8U, 1);

					cvSetImageROI(binary,cvRect(fire->m_recBlobs[i].x,fire->m_recBlobs[i].y,fire->m_recBlobs[i].width,fire->m_recBlobs[i].height));
					//imgtemp = (IplImage*)cvClone(binary);
					cvResize((IplImage*)cvClone(binary),img[i],CV_INTER_LINEAR );
					
					//img[i] = (IplImage*)cvClone(binary);
					opt[i] = fire->m_recBlobs[i].x;

					//레이블링된 각 위치값을 잡아주고
					CvPoint	fire_pt1 = cvPoint(	fire->m_recBlobs[i].x, fire->m_recBlobs[i].y );
					CvPoint fire_pt2 = cvPoint(	fire_pt1.x + fire->m_recBlobs[i].width, fire_pt1.y + fire->m_recBlobs[i].height );
					// 각 레이블 표시

					cvDrawRect(end1, fire_pt1, fire_pt2, myRedColor ,3);
				}
				fire->~CBlobLabeling();
				
				IplImage* temp1;
				int tem;
				gDetBlobCnt = fire->m_nBlobs; //번호판 블럽의 숫자 저장.

				//레이블링 된 이미지의 x점을 기준으로 정렬한다.
				for(int i=0; i < fire->m_nBlobs; i++)
				{
					for(int j=i+1; j < fire->m_nBlobs; j++)
					{
						if(opt[i] > opt[j])
						{
							tem = opt[i];
							opt[i] = opt[j];
							opt[j] = tem;
							temp1 = (IplImage*)cvClone(img[i]);
							img[i] = (IplImage*)cvClone(img[j]);
							img[j] = (IplImage*)cvClone(temp1);
						}
					}
				}
				myShowImage(end1, PB_NUM);
				cvReleaseImage( &temp1);
			}

			
			for( int j=0; j < inner->m_nBlobs; j++ )
			{
				int nThick = (int)NUM_THICK->Value;

				if(		inner->m_recBlobs[j].x < nThick
					||	inner->m_recBlobs[j].y < nThick
					||	( sub_gray->width - (inner->m_recBlobs[j].x + inner->m_recBlobs[j].width) ) < nThick
					||	( sub_gray->height - (inner->m_recBlobs[j].y + inner->m_recBlobs[j].height) ) < nThick )	continue;

				CvPoint	s_pt1 = cvPoint(	pt1.x + inner->m_recBlobs[j].x,			pt1.y + inner->m_recBlobs[j].y );
				CvPoint s_pt2 = cvPoint(	s_pt1.x + inner->m_recBlobs[j].width,	s_pt1.y + inner->m_recBlobs[j].height );

				// green
				cvDrawRect( labeled, s_pt1, s_pt2, myGreenColor, 1 );
			}
			
			// 각 레이블 표시

			cvDrawRect( labeled, pt1, pt2, myRedColor );
			cvReleaseImage( &sub_gray );
			inner->~CBlobLabeling();

		}

		cvNamedWindow("labeled");	cvShowImage("labeled", labeled);
		RB_RESULT2->Text += "detected digits are "+gDetBlobCnt.ToString()+"\n";

		cvReleaseImage( &labeled );
		if(blob->m_nBlobs>0)
			comp_show_number(gDetBlobCnt);

		
	}

	//-------------------------------------------------------------------------
	//	void get_num_as_pos(int pos)
	//-------------------------------------------------------------------------
	int get_num_as_pos(int pos)
	{
			 if(pos < 85)	return 0;
		else if(pos < 185)	return 1;
		else if(pos < 285)	return 2;
		else if(pos < 385)	return 3;
		else if(pos < 485)	return 4;
		else if(pos < 585)	return 5;
		else if(pos < 685)	return 6;
		else if(pos < 785)	return 7;
		else if(pos < 885)	return 8;
		else if(pos < 985)	return 9;
		else if(pos < 1085)	return 10;
		else if(pos < 1185)	return 11;
		else if(pos < 1285)	return 12;
		else if(pos < 1385)	return 13;
		else				return 14;
		
	}

	//-------------------------------------------------------------------------
	//	void comp_show_number(int number_of_det)
	//-------------------------------------------------------------------------
	void comp_show_number(int number_of_det)
	{
		
		// 템플릿 이미지를 받는다.
		int templet_x = 100;
		int templet_y = 150;
		char t_str[128]={0};

		IplImage* reimg[10]; 
		//IplImage* res[10];
		
		for(int i=0; i<number_of_det; i++)
		{
			reimg[i] = cvCreateImage(cvSize(templet_x,templet_y), IPL_DEPTH_8U, 1);
		}
		
		int count=0;
		int tempnum;
		//정렬을 위한 배열 2개
		int number_temp[10];
		int number_num[10];
		int number_temp_temp;
		int number_num_temp;

		int final[20];
		
		Thinning^ th = gcnew thin::Thinning;
		
		
		// 기준숫자이미지 이진화
		//for(int i=0; i <10; i++)	cvThreshold(res[i], res[i], 1, 255, CV_THRESH_BINARY);			

		// 번호 이미지 이진화
		for(int i=0; i <gDetBlobCnt; i++)
		{
			try{	cvThreshold(img[i], img[i], 1, 255, CV_THRESH_BINARY_INV);	}catch(Exception ^e){}
			// 번호 이미지 세선화 실시
			//img[i] = th->Thin(img[i]);	
			
			
		}

		double min, max;
		CvPoint left_top;
		
		IplImage* ref_img;
		//gTemplateImg = cvLoadImage("./digit/ten.bmp", -1);
		ref_img = cvCreateImage(cvSize(gTemplateImg->width,gTemplateImg->height), IPL_DEPTH_8U, 1);
		cvCvtColor(gTemplateImg, ref_img, CV_RGB2GRAY);

		/*
		if(CB_INVERSE->Checked==0)
			try{	cvThreshold(ref_img, ref_img, 1, 255, CV_THRESH_BINARY_INV);	}catch(Exception ^e){}
		else
			try{	cvThreshold(ref_img, ref_img, 1, 255, CV_THRESH_BINARY);	}catch(Exception ^e){}
		*/
		

		for(int a=0; a<gDetBlobCnt; a++)
		{

			cvResize(img[a], reimg[a], 1);
			//if(CB_INVERSE->Checked==0)	try{	cvThreshold( reimg[a],  reimg[a], 1, 255, CV_THRESH_BINARY_INV);	}catch(Exception ^e){}
			
#define DEBUG_WINDOW 1
#if 1
			{

				IplImage* temp_img = cvCreateImage( cvSize( ref_img->width - reimg[a]->width+1, ref_img->height - reimg[a]->height+1 ), IPL_DEPTH_32F, 1 ); // 상관계수를 구할 이미지(C)
				cvMatchTemplate(ref_img, reimg[a], temp_img, CV_TM_CCOEFF_NORMED); // 상관계수를 구하여 C 에 그린다.
				cvMinMaxLoc(temp_img, &min, NULL, &left_top, NULL, NULL);
				left_top.y=0;
#if DEBUG_WINDOW
				RB_RESULT2->Text += min.ToString()+"\n";
				RB_RESULT2->Text += left_top.x.ToString() + " " +left_top.y.ToString()+"\n";
				cvRectangle(ref_img, left_top, cvPoint(left_top.x + reimg[a]->width, left_top.y+a*20 + reimg[a]->height), CV_RGB(255,255,255), 3, 8, 0);
				cvNamedWindow("cor");	cvShowImage("cor", ref_img);
				switch(a)
				{
				case 0 : cvNamedWindow("0");	cvShowImage("0", reimg[0]); break;
				case 1 : cvNamedWindow("1");	cvShowImage("1", reimg[1]); break;
				case 2 : cvNamedWindow("2");	cvShowImage("2", reimg[2]); break;
				case 3 : cvNamedWindow("3");	cvShowImage("3", reimg[3]); break;
				case 4 : cvNamedWindow("4");	cvShowImage("4", reimg[4]); break;
				case 5 : cvNamedWindow("5");	cvShowImage("5", reimg[5]); break;
				case 6 : cvNamedWindow("6");	cvShowImage("6", reimg[6]); break;
				case 7 : cvNamedWindow("7");	cvShowImage("7", reimg[7]); break;
				case 8 : cvNamedWindow("8");	cvShowImage("8", reimg[8]); break;
				case 9 : cvNamedWindow("9");	cvShowImage("9", reimg[9]); break;
				}

#endif
				cvReleaseImage( &temp_img );
				final[a] = get_num_as_pos(left_top.x);
			}
#endif
			
		}

		RB_RESULT->Clear();

		for(int k=0; k<gDetBlobCnt; k++)
		{
			if(final[k]<10)			RB_RESULT->Text += final[k].ToString()+" ";
			else if(final[k]==10)	RB_RESULT->Text += "가 ";
			else if(final[k]==11)	RB_RESULT->Text += "나 ";
			else if(final[k]==12)	RB_RESULT->Text += "옷 ";
			else if(final[k]==13)	RB_RESULT->Text += "러 ";
			else					RB_RESULT->Text += "엥? ";
		}

		cvReleaseImage( &ref_img );
		//cvReleaseImage( &gTemplateImg); 
		for(int i=0; i<number_of_det; i++)
		{
			cvReleaseImage( &reimg[i]);
			//cvReleaseImage( &res[i]);
		}
		

		
	}

#if 0

	//------------------------------------------------------------------------------
	// detect_numboard()
	//------------------------------------------------------------------------------
	void detect_numboard()
	{
		gray = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
		bnw = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
		cvCvtColor(m_pImage, gray, CV_RGB2GRAY);
		cvCvtColor(m_pImage, bnw, CV_RGB2GRAY);


		if(CB_INVERSE->Checked)
		{
			
			cvErode(bnw, bnw, NULL, 20);//침식연산
			cvSub(gray, bnw, gray);//Top-Hat연산
			try{	cvThreshold(gray, gray, 128, 255.0, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);	}catch(Exception ^e){}

		}
		else
		{
			cvErode(bnw, bnw, NULL, 20);//침식연산
			cvSub(gray, bnw, gray);//Top-Hat연산

			try{	cvThreshold(gray, gray, 1, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);	}catch(Exception ^e){}
		}


		

		IplImage* labeled = cvCreateImage( cvSize( gray->width, gray->height ), 8, 3 );//레이블링 이미지
		cvCvtColor(gray, labeled, CV_GRAY2BGR );//그레이를 RGB로 바꿔준다.
		
		// 레이블링 알고리즘 사용
		CBlobLabeling^ blob = gcnew blobing::CBlobLabeling;

		int outer_pix = (int)NUM_OUTER->Value;

		//blob->SetParam(gray, outer_pix );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정
		blob->SetParam(labeled, outer_pix );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정
		//임의로 50 픽셀수 설정
		
		blob->DoLabeling(); //레이블링 실시
			
		int  opt[100];
		
		// 잡영 제거 부분.
		int nMaxWidth	= gray->width  * 3 / 10;	// 영상 가로 전체 크기의 50% 이상인 레이블은 제거
		int nMaxHeight	= gray->height * 3 / 10;	// 영상 세로 전체 크기의 50% 이상인 레이블은 제거
		blob->BlobSmallSizeConstraint( 15, 15 );
		blob->BlobBigSizeConstraint( nMaxWidth, nMaxHeight );
		// 픽셀 사이즈 30 미만은 제거 

		for( int i=0; i < blob->m_nBlobs; i++ )
		{
			//레이블링된 각 위치값을 잡아주고
			CvPoint	pt1 = cvPoint(blob->m_recBlobs[i].x, blob->m_recBlobs[i].y);
			CvPoint pt2 = cvPoint(pt1.x + blob->m_recBlobs[i].width, pt1.y + blob->m_recBlobs[i].height);
			
			// 이미지 관심영역 설정
			cvSetImageROI(gray, blob->m_recBlobs[i]);
			IplImage* sub_gray = cvCreateImage(cvSize(blob->m_recBlobs[i].width, blob->m_recBlobs[i].height), 8, 1);
			try{	cvThreshold(gray, sub_gray, 1, 255, CV_THRESH_BINARY_INV );	}catch(Exception ^){}
			
			// 관심영역 해제
			cvResetImageROI(gray);

			////////////////////////////
			// 레이블링
			CBlobLabeling^ inner = gcnew blobing::CBlobLabeling;

			int inner_pix = (int)NUM_OUTER->Value;

			inner->SetParam(sub_gray, inner_pix);

			inner->DoLabeling();

			int nSubMaxWidth	= sub_gray->width  * 8 / 10;
			int nSubMaxHeight	= sub_gray->height * 8 / 10;

			inner->BlobBigSizeConstraint( nSubMaxWidth, nSubMaxHeight );

			if(inner->m_nBlobs > 4)
			{			
				binary = cvCreateImage(cvSize( (sub_gray->width)*3, (int)(sub_gray->height)*3 ), IPL_DEPTH_8U, 1);
				cvResize(sub_gray,binary,CV_INTER_LINEAR );
				
				end1 = cvCreateImage( cvSize(binary->width, binary->height ), 8, 3 );//레이블링 이미지
				cvCvtColor(binary, end1, CV_GRAY2BGR );//그레이를 RGB로 바꿔준다.

				try{	cvThreshold(binary, binary, 1, 255, CV_THRESH_BINARY);	}catch(Exception ^){}

				CBlobLabeling^ fire = gcnew blobing::CBlobLabeling;
				fire->SetParam(binary, 100);
				fire->DoLabeling();
				
				int nwidth	= binary->width  * 5 / 10;	// 영상 가로 전체 크기의 80% 이상인 레이블은 제거
				int nheight	= binary->height * 7 / 10;	// 영상 세로 전체 크기의 80% 이상인 레이블은 제거

				fire->BlobSmallSizeConstraint( 10, 10 );
				fire->BlobBigSizeConstraint( nwidth, nheight );
				// 픽셀 사이즈 30 미만은 제거 
				int count = 0;

				for( int i=0; i < fire->m_nBlobs; i++ )
				{
					//이미지는 템플릿 사이즈로 조정해준다.
					//버퍼에 이미지를 받는다.
					//교환해서 리사이즈한다.
					//img[i] = cvCreateImage(cvSize(fire->m_recBlobs[i].width, fire->m_recBlobs[i].height), IPL_DEPTH_8U, 1);
					img[i] = cvCreateImage(cvSize(30, 45), IPL_DEPTH_8U, 1);
					//imgtemp[i] = cvCreateImage(cvSize(30, 45), IPL_DEPTH_8U, 1);

					cvSetImageROI(binary,cvRect(fire->m_recBlobs[i].x,fire->m_recBlobs[i].y,fire->m_recBlobs[i].width,fire->m_recBlobs[i].height));
					//imgtemp = (IplImage*)cvClone(binary);
					cvResize((IplImage*)cvClone(binary),img[i],CV_INTER_LINEAR );
					
					//img[i] = (IplImage*)cvClone(binary);
					opt[i] = fire->m_recBlobs[i].x;

					//레이블링된 각 위치값을 잡아주고
					CvPoint	fire_pt1 = cvPoint(	fire->m_recBlobs[i].x, fire->m_recBlobs[i].y );
					CvPoint fire_pt2 = cvPoint(	fire_pt1.x + fire->m_recBlobs[i].width, fire_pt1.y + fire->m_recBlobs[i].height );
					// 각 레이블 표시
					CvScalar color	= cvScalar( 0, 0, 255 );	
					cvDrawRect(end1, fire_pt1, fire_pt2, color );
				}
				
				IplImage* temp1;
				int tem;
				gDetBlobCnt = fire->m_nBlobs; //번호판 블럽의 숫자 저장.

				//레이블링 된 이미지의 x점을 기준으로 정렬한다.
				for(int i=0; i < fire->m_nBlobs; i++)
				{
					for(int j=i+1; j < fire->m_nBlobs; j++)
					{
						if(opt[i] > opt[j])
						{
							tem = opt[i];
							opt[i] = opt[j];
							opt[j] = tem;
							temp1 = (IplImage*)cvClone(img[i]);
							img[i] = (IplImage*)cvClone(img[j]);
							img[j] = (IplImage*)cvClone(temp1);
						}
					}
				}

				cvReleaseImage( &temp1);

				//레이블을 파일로 변환하는 부분//
				/*
				for( int i=0; i < fire->m_nBlobs; i++ )
				{
					char filename[512];
					sprintf(filename, "%d.bmp", count++);
					cvSaveImage(filename, img[i]);
				}
				
				cvReleaseImage(&sub_gray);
				cvReleaseImage(&temp1);
				*/

				if(end1!=NULL)	myShowImage (end1, PB_NUM);	// show captured image of cam to screen
			}
			else
			{
				RB_RESULT->Text += "no num board\n";
			}
		}	

		cvNamedWindow("");
		cvShowImage("", labeled);

		
				
		
	}


	void verify_numarea()
	{
		gray = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
		//bnw =  cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
		cvCvtColor(m_pImage, gray, CV_RGB2GRAY);
		//cvCvtColor(m_pImage, bnw, CV_RGB2GRAY);

		int bin_thrh = (int)NUM_BIN_THRESHOLD->Value;

		if(CB_INVERSE->Checked)
		{
			try{	cvThreshold(gray, gray, bin_thrh, 255.0, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);	}catch(Exception ^e){}

		}
		else
		{
			try{	cvThreshold(gray, gray, bin_thrh, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);	}catch(Exception ^e){}
		}
		
		IplImage* labeled = cvCreateImage( cvSize( gray->width, gray->height ), 8, 3 );//레이블링 이미지
		cvCvtColor(gray, labeled, CV_GRAY2BGR );//그레이를 RGB로 바꿔준다.
		
		// 레이블링 알고리즘 사용
		CBlobLabeling^ blob = gcnew blobing::CBlobLabeling;
		
		int outer_pix = (int)NUM_OUTER->Value;

		blob->SetParam(gray, outer_pix );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정
		//임의로 50 픽셀수 설정
		
		blob->DoLabeling(); //레이블링 실시
		
		// 잡영 제거 부분.

		int nMinWidth	= (int)((float)gray->width  * (float)NUM_SMALL_X->Value);
		int nMinHeight	= (int)((float)gray->width  * (float)NUM_SMALL_Y->Value);
		int nMaxWidth	= (int)((float)gray->width  * (float)NUM_BIG_X->Value);	// 영상 가로 전체 크기의 40% 이상인 레이블은 제거
		int nMaxHeight	= (int)((float)gray->height * (float)NUM_BIG_Y->Value);	// 영상 세로 전체 크기의 40% 이상인 레이블은 제거

		blob->BlobSmallSizeConstraint( nMinWidth, nMinHeight );
		blob->BlobBigSizeConstraint( nMaxWidth, nMaxHeight );

		
		
		RB_RESULT2->Text = "num of blob = "+blob->m_nBlobs.ToString()+"\n";

		for( int i=0; i < blob->m_nBlobs; i++ )
		{

			
			//레이블링된 각 위치값을 잡아주고
			CvPoint	pt1 = cvPoint(blob->m_recBlobs[i].x, blob->m_recBlobs[i].y);
			CvPoint pt2 = cvPoint(pt1.x + blob->m_recBlobs[i].width, pt1.y + blob->m_recBlobs[i].height);
			
			// 이미지 관심영역 설정
			cvSetImageROI(gray, blob->m_recBlobs[i]);

			IplImage* sub_gray = cvCreateImage(cvSize(blob->m_recBlobs[i].width, blob->m_recBlobs[i].height), 8, 1);
			
			if(CB_INVERSE->Checked)
			{
				try{	cvThreshold(gray, sub_gray, 1, 255, CV_THRESH_BINARY_INV);	}catch(Exception ^e){}
			}
			else
			{
				try{	cvThreshold(gray, sub_gray, 1, 255, CV_THRESH_BINARY_INV );	}catch(Exception ^e){}
			}
			
			// 관심영역 해제
			cvResetImageROI(gray);
			if(blob->m_nBlobs == 1)
			{
				cvNamedWindow("sub_");
				cvShowImage("sub_",sub_gray);
			}

			////////////////////////////
			// 레이블링
			CBlobLabeling^ inner = gcnew blobing::CBlobLabeling;
			//inner->SetParam(sub_gray, 5);
			
			int inner_pix = (int)NUM_INNER->Value;

			inner->SetParam(sub_gray, inner_pix);

			inner->DoLabeling();
			/*

			int nSubMinWidth	= sub_gray->width * 5 / 10;
			int nSubMinHeight	= sub_gray->height  * 5 / 10;

			int nSubMaxWidth	= sub_gray->width  * 8 / 10;
			int nSubMaxHeight	= sub_gray->height * 8 / 10;

			inner->BlobSmallSizeConstraint( nSubMinWidth, nSubMinHeight );
			inner->BlobBigSizeConstraint( nSubMaxWidth, nSubMaxHeight );
			*/

			for( int j=0; j < inner->m_nBlobs; j++ )
			{
				int nThick = (int)NUM_THICK->Value;

				if(		inner->m_recBlobs[j].x < nThick
					||	inner->m_recBlobs[j].y < nThick
					||	( sub_gray->width - (inner->m_recBlobs[j].x + inner->m_recBlobs[j].width) ) < nThick
					||	( sub_gray->height - (inner->m_recBlobs[j].y + inner->m_recBlobs[j].height) ) < nThick )	continue;

				CvPoint	s_pt1 = cvPoint(	pt1.x + inner->m_recBlobs[j].x,			pt1.y + inner->m_recBlobs[j].y );
				CvPoint s_pt2 = cvPoint(	s_pt1.x + inner->m_recBlobs[j].width,	s_pt1.y + inner->m_recBlobs[j].height );

				// green
				cvDrawRect( labeled, s_pt1, s_pt2, myGreenColor, 1 );
			}
			
			// 각 레이블 표시

			cvDrawRect( labeled, pt1, pt2, myRedColor );
			cvReleaseImage( &sub_gray );
		}

		cvNamedWindow("labeled");
		cvShowImage("labeled", labeled);


		

	}

#endif
	
	private: int find_valid_cam()
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
				
		if (SUCCEEDED(hr)) 
		{ 
			IEnumMoniker *pEnum; 
			System::String^ strUID="";
				
			hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum); 
			if (SUCCEEDED(hr)) 
			{ 
				strUID = DisplayDeviceInformation(pEnum); 

				if(UNLOCK==1)
				{
					MessageBox::Show("Connected UNLOCK");
					gCamFlag = 1;
				}
				else
				{
						gCamFlag = 1;
				}
			} 
			//RB_RESULT->Text += "\n"+gCamFlag.ToString()+"\n"+strUID;
			pEnum->Release(); 
			CoUninitialize(); 
		} 
		return gCamFlag;
	}


	HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum) 
	{ 
		// Create the System Device Enumerator. 
		ICreateDevEnum *pDevEnum; 
		HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,   
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum)); 
	 
		if (SUCCEEDED(hr)) 
		{ 
			// Create an enumerator for the category. 
			hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0); 
			if (hr == S_FALSE) 
			{ 
				hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error. 
			} 
			pDevEnum->Release(); 
		} 
		return hr; 
	}

	System::String^ DisplayDeviceInformation(IEnumMoniker *pEnum) 
	{ 
		IMoniker *pMoniker = NULL; 
		System::String^ strUsbId;
	 
		while (pEnum->Next(1, &pMoniker, NULL) == S_OK) 
		{ 
			IPropertyBag *pPropBag; 
			HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag)); 
			if (FAILED(hr)) 
			{ 
				pMoniker->Release(); 
				continue;   
			}  
	 
			VARIANT var; 
			VariantInit(&var); 
	 
			// Get description or friendly name. 
			hr = pPropBag->Read(L"Description", &var, 0); 
			if (FAILED(hr)) 
			{ 
				hr = pPropBag->Read(L"FriendlyName", &var, 0); 
			} 
			if (SUCCEEDED(hr)) 
			{ 
				System::String^ strUsbId;
				//strUsbId= var ; //var->bstrVal;
				strUsbId=gcnew System::String(var.bstrVal);
				//m_strResult.Append(strUsbId+"\r\n");
				//AfxMessageBox(strUsbId); 
				VariantClear(&var);  
			} 
	 
			hr = pPropBag->Write(L"FriendlyName", &var); 
	 
			/*
			// WaveInID applies only to audio capture devices. 
			hr = pPropBag->Read(L"WaveInID", &var, 0); 
			if (SUCCEEDED(hr)) 
			{ 
				CString strUsbId="";
				strUsbId.Format("WaveIn ID: %d",var.lVal);
				AfxMessageBox(strUsbId); 
				VariantClear(&var);  
			} 
			*/
	 
			hr = pPropBag->Read(L"DevicePath", &var, 0); 
			if (SUCCEEDED(hr)) 
			{ 
				// The device path is not intended for display. 
				strUsbId= gcnew System::String(var.bstrVal);//var.bstrVal;
				VariantClear(&var);  
			} 
	 
			pPropBag->Release(); 
			pMoniker->Release(); 
			return strUsbId;
		} 
	}

	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
 				timer_cam->Stop();			
				gRunCamState=0;
				cvReleaseCapture(&capture);

			 }
};
}

