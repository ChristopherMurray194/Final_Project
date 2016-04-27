// Fill out your copyright notice in the Description page of Project Settings.

//  Author: Mat Buckland (www.ai-junkie.com)

#pragma once

#include "FuzzyTerm.h"
#include "FuzzySet.h"

/**
 * 
 */
class FINAL_PROJECT_API FzSet : public FuzzyTerm
{
private:
	// A reference to the fuzzy set this proxy represents
	FuzzySet& m_Set;

public:
	FzSet(FuzzySet& fs) :m_Set(fs){}

	FuzzyTerm* Clone()const{ return new FzSet(*this); }
	double GetDOM()const{ return m_Set.GetDOM(); }
	void ClearDOM(){ m_Set.ClearDOM(); }
	void ORwithDOM(double val){ m_Set.ORwithDOM(val); }
};