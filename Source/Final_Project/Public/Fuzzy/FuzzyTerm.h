// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class FINAL_PROJECT_API FuzzyTerm
{
public:
	virtual ~FuzzyTerm(){}

	// All terms must implement a virtual constructor
	virtual FuzzyTerm* Clone() const = 0;

	// Retrieves the degree of membership of the term
	virtual double GetDOM() const = 0;

	// Clears the degree of membership
	virtual void ClearDOM() = 0;

	// Method for updating the DOM of a consequent when a rule fires
	virtual void ORwithDOM(double val) = 0;
};

