/* ========================================================================
 * PROJECT: Numerical Optimization for windage Library
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

#include "GoldenSectionSearch2D.h"

#include <vector>

using namespace windage;
bool GoldenSectionSearch2D::Calculate(long double* solution1, long double* solution2)
{
	return false;
}

bool GoldenSectionSearch2D::Calculate(Vector2* solution1, Vector2* solution2)
{
	this->repeat = 1;
	std::vector<double> params;			params.resize(2);

	Vector2 localXMin = this->xMin;
	Vector2 localXMax = this->xMax;
	Vector2 direction = localXMax - localXMin;
	direction /= direction.getLength();

	double length = (localXMax - localXMin).getLength();
	Vector2 x1 = localXMin + direction * (TAU * length);
	Vector2 x2 = localXMax - direction * (TAU * length);

	params[0] = x1.x; params[1] = x1.y;
	double solutionX1 = function(&params, 2);
	params[0] = x2.x; params[1] = x2.y;
	double solutionX2 = function(&params, 2);

	bool processing = true;
	while(processing)
	{
		// calculate
		length = (localXMax - localXMin).getLength();
		Vector2 tempX1 = localXMin + direction * ( (1 - TAU) * length );
		Vector2 tempX2 = localXMin + direction * ( TAU * length );

		// always x1 != x2
		if(tempX1 == tempX2)
		{
			tempX2.x += windage::DELTA;
			tempX2.y += windage::DELTA;
		}
//*
		if(tempX1 == x1)
		{
			x1 = tempX1;
			x2 = tempX2;

//			solutionX1 = function(x1);
			params[0] = x2.x;	params[1] = x2.y;
			solutionX2 = function(&params, 2);
		}
		else if(tempX2 == x2)
		{
			x1 = tempX1;
			x2 = tempX2;

			params[0] = x1.x;	params[1] = x1.y;
			solutionX1 = function(&params, 2);
//			solutionX2 = function(x2);
		}
		else if(tempX1 == x2)
		{
			x1 = tempX1;
			x2 = tempX2;

			solutionX1 = solutionX2;
			params[0] = x2.x;	params[1] = x2.y;
			solutionX2 = function(&params, 2);
		}
		else if(tempX2 == x1)
		{
			x1 = tempX1;
			x2 = tempX2;

			solutionX2 = solutionX1;
			params[0] = x1.x;	params[1] = x1.y;
			solutionX1 = function(&params, 2);
		}
		else
//*/
		{
			x1 = tempX1;
			x2 = tempX2;

			params[0] = x1.x;	params[1] = x1.y;
			solutionX1 = function(&params, 2);
			params[0] = x2.x;	params[1] = x2.y;
			solutionX2 = function(&params, 2);
		}

		if(solutionX1 >= solutionX2)
		{
			localXMin = x1;
		}
		else if(solutionX1 <= solutionX2)
		{
			localXMax = x2;
		}

		if(abs((localXMin - localXMax).getLength()) < LEAST_ERROR_RANGE)
		{
			(*solution1) = localXMin;
			(*solution2) = localXMax;
			return true;
		}
		
		this->repeat++;
		if(this->repeat > MAX_INTERATE_TIME)
		{
			(*solution1) = x1;
			(*solution2) = x2;
			return false;
		}
	}

	(*solution1) = localXMin;
	(*solution2) = localXMax;
	return true;
}