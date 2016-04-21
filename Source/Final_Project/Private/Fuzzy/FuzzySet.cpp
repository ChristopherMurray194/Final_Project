// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "FuzzySet.h"

double FuzzySet::GetRepresentativeVal() const{ return m_dRepresentativeValue; }

void FuzzySet::SetDOM(double val){ m_dDOM = val; }

void FuzzySet::ORwithDOM(double val)
{
	// If the DOM of the antecedent is greater than the consequent
	// set the consequent's DOM to the antecedent's
	if (val > m_dDOM) m_dDOM = val;
}
