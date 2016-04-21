// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FuzzySet.h"
#include "FzSet.h"

/**
 * 
 */
class FINAL_PROJECT_API Variable
{
	private:
		typedef std::map<std::string, FuzzySet*> MemberSets;

	private:
		// Disallow copies
		Variable(const Variable&);
		Variable& operator=(const Variable&); // Assignment operator

	private:
		// A map of the fuzzy sets that comprise this variable
		MemberSets m_MemberSets;

		// The minimum and maximum value of the range of this variable
		double m_dMinRange;
		double m_dMaxRange;

		// This method is called with the upper and lower bound of a set each time a
		// new set is added to adjust the upper and lower range values accordingly
		void AdjustRangeToFit(double min, double max);

		// A client retrieves a reference to a fuzzy variable when an instance is
		// created via FuzzyModule::CreateFLV(). To prevent the client from deleting
		// the instance the Varaible destructor is made pivate and the FuzzyModule
		// class made a friend.
		~Variable();

		// The friendly modifier allows the designated class access to protected data
		// it cannot however modify this data as a subclass could.
		friend class FuzzyModule;

	public:
		Variable() :m_dMinRange(0.0), m_dMaxRange(0.0){}

		// The following methods create instances of the sets named in the method
		// name and adds them to the member set map. Each time a set of any type is
		// added the m_dMinRange and m_dMaxRange are adjusted accordingly. All of the
		// methods return a proxy class representing the newly created instance. This
		// proxy set can be used as an operand when creating the rule base.
		FzSet AddLeftShoulderSet(std::string name,
			double minBound,
			double peak,
			double maxBound);

		FzSet AddRightShoulderSet(std::string name,
			double minBound,
			double peak,
			double maxBound);

		FzSet AddTriangleSet(std::string name,
			double minBound,
			double peak,
			double maxBound);

		// Fuzzify a value by calculating its DOM in each of this variable's subsets
		void Fuzzify(double val);

		// Defuzzify the variable using the MaxAv method
		double DeFuzzifyMaxAv() const;

		// Defuzzify the variable using the centroid method
		double DeFuzzifyCentroid(int NumSamples)const;
	};

