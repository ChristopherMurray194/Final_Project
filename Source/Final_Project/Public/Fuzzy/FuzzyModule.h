// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include "Variable.h"
#include "FuzzyRule.h"

/**
 * 
 */
class FINAL_PROJECT_API FuzzyModule
{
public:

private:
	typedef std::map<std::string, Variable*> VarMap;

public:
	// A client must pass one of these values to the defuzzify method.
	// This module only supports the MaxAverage and centroid methods.
	enum DefuzzifyType{ max_av, centroid };

	// When calculating the centroid of the fuzzy manifold this value is used
	// to determine how many cross sections should be sampled
	enum { NumSamplesToUseForCentroid = 10 };

private:
	// A map of all the fuzzy variables this module uses
	VarMap m_Variables;

	// A vector containing all the fuzzy rules
	std::vector<FuzzyRule*> m_Rules;

	// Zeros the DOMs of the consequents of each rule. Used by Defuzzify()
	inline void FuzzyModule::SetConfidencesOfConsequentsToZero()
	{
		for (FuzzyRule* i : m_Rules)
			i->SetConfidenceOfConsequenceToZero();
	}

public:

	~FuzzyModule();

	// Creates a new "empty" fuzzy variable and returns a reference to it.
	Variable& CreateFLV(const std::string& VarName);

	// Adds a rule to the module
	void AddRule(FuzzyTerm& antecedent, FuzzyTerm& consequence);

	// This method calls the fuzzify method of the named FLV
	inline void FuzzyModule::Fuzzify(const std::string& NameOfFLV, double val)
	{
		// First make sure the named FLV exists in this module
		assert((m_Variables.find(NameOfFLV) != m_Variables.end()) &&
			"<FuzzyModule::Fuzzify>:key not found");

		// Call the fuzzify method
		m_Variables[NameOfFLV]->Fuzzify(val);
	}

	// Given a fuzzy variable and a defuzzification method this returns a
	// crisp value
	inline double FuzzyModule::Defuzzify(const std::string& NameOfFLV, DefuzzifyType method)
	{
		// First make sure the named FLV exists in this module
		assert((m_Variables.find(NameOfFLV) != m_Variables.end()) &&
			"<FuzzyModule::DeFuzzifyMaxAv>:key not found");

		// Clear the DOMs of all the consequents
		SetConfidencesOfConsequentsToZero();

		// Process the rules
		for (auto currentRule : m_Rules)
		{
			currentRule->Calculate();
		}

		// Now defuzzify the resultant conclusion using the specified method
		switch (method)
		{
		case centroid:
			return m_Variables[NameOfFLV]->DeFuzzifyCentroid(NumSamplesToUseForCentroid);
		case max_av:
			return m_Variables[NameOfFLV]->DeFuzzifyMaxAv();
		}
		return 0;
	}
};


