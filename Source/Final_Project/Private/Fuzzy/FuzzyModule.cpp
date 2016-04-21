#include "Final_Project.h"
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include "Variable.h"
#include "FuzzyRule.h"
#include "FuzzyModule.h"

FuzzyModule::~FuzzyModule()
{
	for (auto &it : m_Variables)
		delete it.second;

	for (auto &it : m_Rules)
		delete it;
}

Variable& FuzzyModule::CreateFLV(const std::string& VarName)
{
	m_Variables[VarName] = new Variable();

	return *m_Variables[VarName];
}

void FuzzyModule::AddRule(FuzzyTerm& antecedent, FuzzyTerm& consequence)
{
	// Adds a new rule to the Rules member vector
	m_Rules.push_back(new FuzzyRule(antecedent, consequence));
}
