/* ========================================================================
 * PROJECT: windage Library
 * ========================================================================
 * This work is based on the original windage Library developed by
 *   Woonhyuk Baek
 *   Woontack Woo
 *   U-VR Lab, GIST of Gwangju in Korea.
 *   http://windage.googlecode.com/
 *   http://uvr.gist.ac.kr/
 *
 * Copyright of the derived and new portions of this work
 *     (C) 2009 GIST U-VR Lab.
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 *   Woonhyuk Baek
 *   <windage@live.com>
 *   GIST U-VR Lab.
 *   Department of Information and Communication
 *   Gwangju Institute of Science and Technology
 *   1, Oryong-dong, Buk-gu, Gwangju
 *   South Korea
 * ========================================================================
 ** @author   Woonhyuk Baek
 * ======================================================================== */

#include <vector>

#include "Algorithms/WSURFdetector.h"
#include "Structures/WSURFpoint.h"

#include "Algorithms/windageSURF/fast.h"
#include "Algorithms/windageSURF/wfastsurf.h"

using namespace windage;
using namespace windage::Algorithms;

bool WSURFdetector::DoExtractKeypointsDescriptor(IplImage* grayImage)
{
	if(grayImage == NULL)
		return false;
	if(grayImage->nChannels != 1)
		return false;

	this->keypoints.clear();

	// Extract FAST corners;
	CvSeq* descriptors;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* keypointsSeq = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvSURFPoint), storage );

	int cornerCount = 0;
	xy* cornerPoints = NULL;
	cornerPoints = fast9_detect_nonmax((const byte*)grayImage->imageData, grayImage->width, grayImage->height, grayImage->widthStep, cvRound(this->threshold), &cornerCount);

	for(int i=0; i<cornerCount; i++)
	{
		CvSURFPoint point = cvSURFPoint(cvPoint2D32f(cornerPoints[i].x, cornerPoints[i].y), 0, 15, 0, 0);
		cvSeqPush(keypointsSeq, &point);
	}
	if(cornerPoints) delete[] cornerPoints;

	// Generate Descriptor
	CvSURFParams params = cvSURFParams(this->threshold, 0);
	wExtractFASTSURF(grayImage, NULL, &keypointsSeq, &descriptors, storage, params, 1);
//	cvExtractSURF(grayImage, NULL, &keypointsSeq, &descriptors, storage, params);
	
	CvSeqReader reader;
	cvStartReadSeq(descriptors, &reader, 0);

	windage::WSURFpoint point;
	for(int i=0; i<keypointsSeq->total; i++)
	{
		CvSURFPoint* surfPT = (CvSURFPoint*)cvGetSeqElem(keypointsSeq, i);

		point.SetPoint(windage::Vector3(surfPT->pt.x, surfPT->pt.y, 1.0));
		point.SetSize(surfPT->size);
		point.SetDir(surfPT->dir);

		const float* vec = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);

		for(int j=0; j<point.DESCRIPTOR_DIMENSION; j++)
		{
			point.descriptor[j] = vec[j];
		}

		this->keypoints.push_back(point);
	}

	cvReleaseMemStorage(&storage);

	return true;
}