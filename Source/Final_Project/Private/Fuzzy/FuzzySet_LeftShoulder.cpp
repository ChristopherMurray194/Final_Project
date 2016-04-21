// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "FuzzySet.h"
#include <iostream>
#include "FuzzySet_LeftShoulder.h"

double FuzzySet_LeftShoulder::CalculateDOM(double val)const
{
	std::cout << "LeftShoulder: " << std::endl;
	// Check for case where the offset may be zero
	if ((m_dRightOffset == 0.0f) && (val == m_dPeakPoint))
	{
		return 1.0;
	}

	//find DOM if right of center
	else if ((val >= m_dPeakPoint) && (val < (m_dPeakPoint + m_dRightOffset)))
	{
		double grad = 1.0 / -m_dRightOffset;

		return grad * (val - m_dPeakPoint) + 1.0;
	}

	//find DOM if left of center
	else if ((val < m_dPeakPoint) && (val >= m_dPeakPoint - m_dLeftOffset))
	{
		return 1.0;
	}

	//out of range of this FLV, return zero
	else
	{
		return 0.0;
	}
}
