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

#ifndef _SPILL_TREE_H_
#define _SPILL_TREE_H_

#include <vector>

#include <cv.h>
#include "base.h"

#include "Structures/FeaturePoint.h"
#include "Algorithms/SearchTree.h"

namespace windage
{
	namespace Algorithms
	{
		class DLLEXPORT Spilltree : public SearchTree
		{
		private:
			CvMat* descriptorStorage;
			CvFeatureTree* spilltree;
			int eMax;

		public:
			Spilltree() : SearchTree()
			{
				this->descriptorStorage = NULL;
				this->spilltree = NULL;
				this->eMax = 20;
			}
			~Spilltree()
			{
				if(this->descriptorStorage) cvReleaseMat(&this->descriptorStorage);
				if(this->spilltree) cvReleaseFeatureTree(this->spilltree);
			}

			inline void SetEMax(int emax){this->eMax = emax;};
			inline int GetEMax(){return this->eMax;};

			bool Training(std::vector<windage::FeaturePoint>* pointList);
			int Matching(windage::FeaturePoint point, double* difference = NULL);
		};
	}
}

#endif