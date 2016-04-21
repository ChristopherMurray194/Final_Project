// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class FINAL_PROJECT_API FuzzySet
{
	protected:
		// This will hold the degree of membership (DoM) in this set of a given value
		double m_dDOM;

		// This is the maximum of the set's membership function. For instance, if
		// the set is triangular then this will be the peak point of the triangle.
		// If the set has a plateau then this value will be the midpoint of the
		// plateau. This value is in the constructor to avoid run-time
		// calculation of midpoint values.
		double m_dRepresentativeValue;

	public:
		FuzzySet(double RepVal) :m_dDOM(0.0), m_dRepresentativeValue(RepVal){}

		// Return the degree of membership in this set of the given value. NOTE:
		// this does not set m_dDOM to the DOM of the value passed as the parameter.
		// This is because the centroid defuzzification method also uses this method
		// to determine the DOMs of the values it uses as its sample points.
		virtual double CalculateDOM(double val)const = 0;

		// If this fuzzy set is part of a consequent FLV and it is fired by a rule,
		// then this method sets the DOM (in this context, the DOM represents a 
		// confidence level) to the maximum of the parameter value or the set's
		// exisitng m_dDOM value
		void ORwithDOM(double val);

		// Accessor methods
		double GetRepresentativeVal()const;
		void ClearDOM(){ m_dDOM = 0.0; }
		double GetDOM()const{ return m_dDOM; }
		void SetDOM(double val);
	};

