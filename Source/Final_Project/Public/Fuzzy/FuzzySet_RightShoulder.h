// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FuzzySet.h"

/**
 * 
 */
class FINAL_PROJECT_API FuzzySet_RightShoulder : public FuzzySet
{
	public:
		~FuzzySet_RightShoulder();

	private:
		// The values that define the shape of this FLV
		double m_dPeakPoint;
		double m_dLeftOffset;
		double m_dRightOffset;

	public:
		FuzzySet_RightShoulder(double Peak,
			double LeftOffset,
			double RightOffset) :
			FuzzySet((Peak + RightOffset) / 2),
			m_dPeakPoint(Peak),
			m_dLeftOffset(LeftOffset),
			m_dRightOffset(RightOffset)
		{}

		// This method calculates the degree of membership for a particular value
		double CalculateDOM(double val)const override;
	};