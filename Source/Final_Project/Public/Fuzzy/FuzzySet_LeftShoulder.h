// Fill out your copyright notice in the Description page of Project Settings.

//  Author: Mat Buckland (www.ai-junkie.com)

#pragma once

#include <iostream>
#include "FuzzySet.h"

/**
 * 
 */
class FINAL_PROJECT_API FuzzySet_LeftShoulder : public FuzzySet
{
	public:
		~FuzzySet_LeftShoulder();

	private:
		// The values that define the shape of this FLV
		double m_dPeakPoint;
		double m_dLeftOffset;
		double m_dRightOffset;

	public:
		FuzzySet_LeftShoulder(double Peak,
			double LeftOffset,
			double RightOffset) :
			FuzzySet((LeftOffset + Peak) / 2),
			m_dPeakPoint(Peak),
			m_dLeftOffset(LeftOffset),
			m_dRightOffset(RightOffset)
		{}

		// This method calculates the degree of membership for a particular value
		double CalculateDOM(double val)const override;
	};

