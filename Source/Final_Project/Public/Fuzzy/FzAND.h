// Fill out your copyright notice in the Description page of Project Settings.

//  Author: Mat Buckland (www.ai-junkie.com)

#pragma once

#include <vector>
#include "FuzzyTerm.h"

/**
 * 
 */
class FINAL_PROJECT_API FzAND : public FuzzyTerm
{
	private:
		// An instance of this class may AND together up to 4 terms
		std::vector<FuzzyTerm*> m_Terms;
		
		// Disallow assignment
		FzAND& operator=(const FzAND&);

	public:
		~FzAND();

		// Copy constructor
		FzAND(const FzAND& fa);

		// Constructor accepting fuzzy terms
		FzAND(FuzzyTerm& op1, FuzzyTerm& op2);
		FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3);
		FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4);

		// virtual constructor
		FuzzyTerm* Clone()const{ return new FzAND(*this); }

		double GetDOM() const;
		void ClearDOM();
		void ORwithDOM(double val);
	};

