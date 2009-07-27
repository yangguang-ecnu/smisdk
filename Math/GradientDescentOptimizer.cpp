// -*- C++ -*-
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// More details about the license can be found here: http://www.apache.org/licenses
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is 
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
//
// Filename:	GradientDescentOptimizer.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/GradientDescentOptimizer.h"

namespace PGMath
{

	template <class T>
	GradientDescentOptimizer<T>::GradientDescentOptimizer(const float iGradientStep/*=0.1f*/) 
	{
		m_gradientStep=iGradientStep; 
		m_iterations = 0;
		m_initGradientStep = 64.0f;
	};

	template <class T>
	bool GradientDescentOptimizer<T>::SetGradientFunction(const ObjectiveGradientFunction iGradientFunction)
	{
		m_gradientFunction	=	iGradientFunction;
		return (m_gradientFunction!=0);
	}

	template <class T>
	GradientDescentOptimizer<T>::~GradientDescentOptimizer(void) {};


	template <class T>
	bool GradientDescentOptimizer<T>::LineSearch(const std::vector<float>& iGradientDescentValues, float& ioCurrentFnValue)
	{
		return false;

		/*
		bool res = false;
		//if (m_initGradientStep<0.1f) 		return res;

		int iterFactor = m_iterations/8;
		float pGradientStep = m_initGradientStep, lastGradientStep=0.0f;;//1.0f;//128.0f;//16.0f/((float)(iterFactor+1));
		//pGradientStep = pGradientStep < 1 ? 1 : pGradientStep;

		float gradientStep = 0.1f * m_initGradientStep;//.0f;//1.0f/((float)(iterFactor+1));
		//gradientStep = gradientStep < 0.5f ? 0.5f : gradientStep;

		float tempValue = 0.0f, fnValue = ioCurrentFnValue, fnChange=-0.0001f, newfnValue=0.0f, lastFnValue=ioCurrentFnValue;//(*m_objectiveFn)(m_sourceModule, m_parameters, 0), 

		std::vector<float> tempParams, lastParams;// = m_parameters;
		int iterCount = 0, maxIterCount=200;

		bool foundOneValue=false;

		while (fnChange<0 && fabs(fnChange)<0.001 && iterCount<maxIterCount)
		{
			tempParams = m_parameters;

			for (int i=0; i<tempParams.size(); i++)
			{
				//if (iGradientDescentValues[i]<0.0f)
				{
					float nStep = pGradientStep*iGradientDescentValues[i]; 

					tempParams[i] -= nStep;				
				}
			}

			newfnValue = (*m_objectiveFn)(m_sourceModule, tempParams, 0);
			fnChange = (-fnValue + newfnValue);

			if (fnChange<0)
			{
				lastFnValue = newfnValue;
				lastParams = tempParams;
				lastGradientStep = pGradientStep;
				foundOneValue = true;
			}

			if (GetLogger()) 
				GetLogger()->Log("\t\tLineSearchFunctionValue[%d] = [%3.5f] [Last: %3.5f] [Change: %3.5f]", iterCount, newfnValue, 
				fnValue, fnChange);

			pGradientStep+= gradientStep;
			iterCount++;
		}

		res = (iterCount<maxIterCount && foundOneValue);
		if (res)
		{
			m_parameters = lastParams;
			ioCurrentFnValue = lastFnValue;
			m_gradientStep = lastGradientStep;
		}

		return res;
		*/
	}

	/*
	Algorithm 6.3 Gradient Descent
	Require: x0, Precision epsilon 
	n = 0
	repeat
	Compute g = f'(xn)
	Perform line search on f (xn - lambda * g) for optimal lambda.
	x(n+1) = xn - lamba * g
	n =  n + 1
	until f'(xn) < epsilon
	Output: xn
	*/
	template <class T>
	bool GradientDescentOptimizer<T>::Optimize()
	{
		return false;
		/*
		if (!m_objectiveFn || !m_gradientFunction || !m_sourceModule) return false;

		std::vector<float> fnGradientValue;
		fnGradientValue.resize(DOF_COUNT);
		float derivSum=0.0f;


		//float fnGradientValue = 1.0f;//kPgEpsilon;//(*m_gradientFunction)(m_sourceModule, m_parameters, 0);
		float tempValue=0.0f, fnValue = 0.0f, minFnValue=99999.9f, lastFnValue=0;
		int maxCount=64;
		m_iterations=0;

		fnValue = (*m_objectiveFn)(m_sourceModule, m_parameters, 0);
		fnGradientValue = (*m_gradientFunction)(m_sourceModule, m_parameters, fnValue, 0);
		for (int i=0; i<m_parameters.size(); i++)
		{
			//fnGradientValue[i] = -0.01f;
			derivSum+=fabs(fnGradientValue[i]);
		}

		if (GetLogger()) 
		{	
			GetLogger()->Log("[Iter: %d][Step: %3.3f] [Sim: %3.5f] | [T %3.4f %3.4f %3.4f][R %3.4f %3.4f %3.4f] [Sc %3.4f %3.4f %3.4f]",
				m_iterations, m_gradientStep, fnValue,	m_parameters[TRANS_X], m_parameters[TRANS_Y], m_parameters[TRANS_Z],
				m_parameters[ROT_X], m_parameters[ROT_Y], m_parameters[ROT_Z],
				m_parameters[SCALE_X], m_parameters[SCALE_Y], m_parameters[SCALE_Z]);			

			GetLogger()->Log("\tFunctionValue = [%f] [Gradient: %f %f %f %f %f %f %f %f %f %f %f %f]", fnValue, 
				fnGradientValue[0], fnGradientValue[1], fnGradientValue[2], fnGradientValue[3], fnGradientValue[4], fnGradientValue[5], 
				fnGradientValue[6], fnGradientValue[7], fnGradientValue[8], fnGradientValue[9], fnGradientValue[10], fnGradientValue[11]);
		}

		//while (fabs((float)fnGradientValue)>=kPgEpsilon)
		while (derivSum>0.00001 && m_iterations<maxCount)//kPgEpsilon)
		{

			if (GetLogger()) 
			{	
				GetLogger()->Log("[Iter: %d][Step: %3.3f] [Sim: %3.5f] | [T %3.4f %3.4f %3.4f][R %3.4f %3.4f %3.4f] [Sc %3.4f %3.4f %3.4f]",
					m_iterations, m_gradientStep, fnValue,	m_parameters[TRANS_X], m_parameters[TRANS_Y], m_parameters[TRANS_Z],
					m_parameters[ROT_X], m_parameters[ROT_Y], m_parameters[ROT_Z],
					m_parameters[SCALE_X], m_parameters[SCALE_Y], m_parameters[SCALE_Z]);			

				GetLogger()->Log("\tFunctionValue = [%f] [Gradient: %f %f %f %f %f %f %f %f %f %f %f %f]", fnValue, 
					fnGradientValue[0], fnGradientValue[1], fnGradientValue[2], fnGradientValue[3], fnGradientValue[4], fnGradientValue[5], 
					fnGradientValue[6], fnGradientValue[7], fnGradientValue[8], fnGradientValue[9], fnGradientValue[10], fnGradientValue[11]);
			}


			if (!LineSearch(fnGradientValue, fnValue)) 
			{
				LOG2("\t\tFailed to do line search. Current step: %3.5f [InitStep: %3.5f]", m_gradientStep, m_initGradientStep);
				m_initGradientStep*=0.5f;
				if (m_initGradientStep<0.001f) 
				{
					break;
				}
				continue;
				//break;
			}


			//fnValue = (*m_objectiveFn)(m_sourceModule, m_parameters, 0);

			//why does fnValue oscillate? it shd monotonically decrease!
			//	perform line search with lambda?

			//if (minIndex>=0 && minIndex<m_parameters.size())
			{
				//m_parameters[minIndex] -= nStep;

			} //else
			{
				//m_gradientStep+=4;
				//if (m_gradientStep>48) 
				//break;

				//continue;
			}


			// compute gradient
			fnGradientValue = (*m_gradientFunction)(m_sourceModule, m_parameters, fnValue, 0);

			derivSum = 0.0f;
			for (int i=0; i<fnGradientValue.size(); i++)
			{
				derivSum+= fabs(fnGradientValue[i]);

			}

			//lastFnValue = minFnValue;
			m_iterations++;
		}

		return true;
		*/
	}

	
#ifdef _PG_GENERATE_SDK_LIBS_
	template class GradientDescentOptimizer<float>; 	
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
