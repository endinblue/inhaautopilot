#pragma once

#include <cv.h>
#include <highgui.h>
#include "msclr/auto_handle.h"

#define _DEF_MAX_BLOBS		10000 //�ִ�blob��
#define _DEF_MAX_LABEL		  100 //�ִ� ���̺� ��

namespace blobing
{
	typedef struct 
	{
		BOOL	bVisitedFlag; //�湮����� ��Ÿ��
		CvPoint ptReturnPoint;//��������Ʈ
	} Visited;

	public ref class  CBlobLabeling : public System::IDisposable
	{
	public:
		CBlobLabeling(void);
	public:
		~CBlobLabeling(void);

	public:
		IplImage*	m_Image;				// ���̺��� ���� �̹���
		Visited*	m_vPoint;				// ���̺��� �湮����
		CvRect*		m_recBlobs;				// �� ���̺� ����
		CvRect*		m_candidateBlobs;		//�ĺ��� ������ blob�� ������
	public:
		int			m_nThreshold;			// ���̺� ������� ��
		int			m_nBlobs;				// ���̺��� ����
		//int		m_candidateLabel[_DEF_MAX_LABEL];			// �ĺ��� ������ ���̺�
		int			*m_candidateLabel;			// �ĺ��� ������ ���̺�
		int			m_nCandidate;			//�ĺ��� ������ ���̺� ��
	//public:
	//	int			m_candidateLabel[_DEF_MAX_LABEL];			// �ĺ��� ������ ���̺�

	public:
		// ���̺� �̹��� ����
		void		SetParam(IplImage* image, int nThreshold);
		// ���̺�(����)
		void		DoLabeling();
		//���̺� ����
		IplImage* DeleteLabeling(IplImage* srcImage);
		//�ĺ����̺� ����
		CvRect*	GetCandidateBlob(CvRect* m_recBlobs);
	private:
		// ���̺�(����)
		int		 Labeling(IplImage* image, int nThreshold);

		// ����Ʈ �ʱ�ȭ
		void	 InitvPoint(int nWidth, int nHeight);
		void	 DeletevPoint();

		// ���̺� ��� ���
		void	 DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight);

		// ���̺�(���� �˰���)
		int		_Labeling(unsigned char *DataBuf, int nWidth, int nHeight, int nThreshold);
		
		// _Labling ���� ��� �Լ�
		int		__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY );
		int		__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel);

		//���̺� ����
		IplImage* _DeleteLabeling(IplImage* srcImage, int m_nBlobs, CvRect* m_recBlobs);
		//�ĺ����̺� ����
		CvRect* _GetCandidateBlob(CvRect* m_recBlobs, int* candidateLabel);


		// ����, ���� ũ�Ⱑ �񱳰� ���� ���� ���̺� ����
		// ������ ������ ���� ���̺� ����
		public:
			int	BlobSmallSizeConstraint(int nWidth, int nHeight);
		private:
			int	_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int nRecNumber);

			// ����, ���� ũ�Ⱑ �񱳰� ���� ū ���̺� ����
			// ������ ������ ū ���̺� ����
		public:
			int	BlobBigSizeConstraint(int nWidth, int nHeight);
		private:
			int	_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int nRecNumber);
	};
}