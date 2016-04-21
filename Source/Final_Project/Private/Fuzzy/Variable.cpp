// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Variable.h"

Variable::~Variable()
{
	for (auto &it : m_MemberSets)
		delete it.second;
}

void Variable::AdjustRangeToFit(double min, double max)
{
	if (min < m_dMinRange) m_dMinRange = min;
	if (max > m_dMaxRange) m_dMaxRange = max;
}

FzSet Variable::AddLeftShoulderSet(std::string name,
	double minBound,
	double peak,
	double maxBound)
{
	m_MemberSets[name] = new FuzzySet_LeftShoulder(peak, minBound, maxBound);
	AdjustRangeToFit(minBound, maxBound);

	return FzSet(*m_MemberSets[name]);
}

FzSet Variable::AddTriangleSet(std::string name,
	double minBound,
	double peak,
	double maxBound)
{
	m_MemberSets[name] = new FuzzySet_Triangle(peak, minBound, maxBound);
	AdjustRangeToFit(minBound, maxBound);

	return FzSet(*m_MemberSets[name]);
}

FzSet Variable::AddRightShoulderSet(std::string name,
	double minBound,
	double peak,
	double maxBound)
{
	m_MemberSets[name] = new FuzzySet_RightShoulder(peak, minBound, maxBound);
	AdjustRangeToFit(minBound, maxBound);

	return FzSet(*m_MemberSets[name]);
}

void Variable::Fuzzify(double val)
{
	for (auto &it : m_MemberSets)
	{
		// Calculate the DOM of the given value to the current set
		double CalcedDOM = it.second->CalculateDOM(val);
		// Set the DOM with the current set for the given value
		it.second->SetDOM(CalcedDOM);
		std::cout << "InVal: " << val << std::endl << "CalcedDOM: " << CalcedDOM << std::endl << std::endl;
	}
}

double Variable::DeFuzzifyMaxAv() const
{
	double numerator = 0.0, denominator = 0.0;
	for (auto &it : m_MemberSets)
	{
		// Calc numerator: SUM of representative value X confidence level
		numerator += (it.second->GetRepresentativeVal() * it.second->GetDOM());
		// Calc denominator: SUM of confidence level
		denominator += it.second->GetDOM();
	}

	// Calculate crisp value
	return numerator / denominator;
}

double Variable::DeFuzzifyCentroid(int NumSamples) const
{
	double numerator = 0.0, denominator = 0.0;
	// Get the value at each sample point(s), represented by i
	for (double i = m_dMinRange; i <= m_dMaxRange; i += NumSamples)
		for (auto &it : m_MemberSets)
		{
		// Calc numerator: SUM of s X DOM(s)
		numerator += (i * it.second->CalculateDOM(i));
		// Calc denominator: SUM of DOM(S)
		denominator += it.second->CalculateDOM(i);
		}

	// Calculate the crisp value
	return numerator / denominator;

}