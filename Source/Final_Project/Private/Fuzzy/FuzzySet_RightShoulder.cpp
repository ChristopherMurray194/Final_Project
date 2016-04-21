// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "FuzzySet_RightShoulder.h"


double FuzzySet_RightShoulder::CalculateDOM(double val)const
{
	std::cout << "RightShoulderSet: " << std::endl;
	// Check for case where the offset may be zero
	if ((m_dLeftOffset == 0.0) && (val == m_dPeakPoint))
	{
		return 1.0;
	}

	//find DOM if left of center
	else if ((val <= m_dPeakPoint) && (val > (m_dPeakPoint - m_dLeftOffset)))
	{
		double grad = 1.0 / m_dLeftOffset;

		return grad * (val - (m_dPeakPoint - m_dLeftOffset));
	}
	//find DOM if right of center and less than center + right offset
	else if ((val > m_dPeakPoint) && (val <= m_dPeakPoint + m_dRightOffset))
	{
		return 1.0;
	}

	else
	{
		return 0;
	}
}