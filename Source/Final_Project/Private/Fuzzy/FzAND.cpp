// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include <vector>
#include "FzAND.h"

FzAND::FzAND(const FzAND& fa)
{
	for (auto &currentTerm : fa.m_Terms)
	{
		m_Terms.push_back(currentTerm->Clone());
	}
}

FzAND::FzAND(FuzzyTerm& op1, FuzzyTerm& op2)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
}

FzAND::FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
	m_Terms.push_back(op3.Clone());
}

FzAND::FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
	m_Terms.push_back(op3.Clone());
	m_Terms.push_back(op4.Clone());
}

FzAND::~FzAND()
{
	for (auto &currentTerm : m_Terms)
	{
		delete currentTerm;
	}
}

double FzAND::GetDOM()const
{
	double smallest = std::numeric_limits<double>::max();
	for (auto &it : m_Terms)
	{
		if (it->GetDOM() < smallest)
		{
			smallest = it->GetDOM();
		}
	}
	return smallest;
}

void FzAND::ORwithDOM(double val)
{
	for (auto &it : m_Terms)
	{
		it->ORwithDOM(val);
	}
}

void FzAND::ClearDOM()
{
	for (auto &it : m_Terms)
		it->ClearDOM();
}