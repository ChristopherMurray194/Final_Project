// Fill out your copyright notice in the Description page of Project Settings.

//  Author: Mat Buckland (www.ai-junkie.com)

#pragma once

#include "FuzzySet.h"
#include <iostream>

/**
 * 
 */
class FINAL_PROJECT_API FuzzySet_Triangle : public FuzzySet
{
	public:
		~FuzzySet_Triangle();

	private:
		// The values that define the shape of this FLV
		double m_dPeakPoint;
		double m_dLeftOffset;
		double m_dRightOffset;

	public:
		FuzzySet_Triangle(double mid,
			double left,
			double right) :FuzzySet(mid),
			m_dPeakPoint(mid),
			m_dLeftOffset(left),
			m_dRightOffset(right)
		{}

		// This method calculates the degree of membership for a particular value
		double CalculateDOM(double val)const override;
	};

