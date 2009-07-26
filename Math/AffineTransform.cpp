// -*- C++ -*-
// © [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS TECHNICAL AND INTELLECTUAL PROPERTY OF
// SCIENCE.MEDICAL.IMAGING (groups.google.com/group/medicalimagingscience). 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND SCIENCE.MEDICAL.IMAGING GROUP
// MAKES NO WARRANTY REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY
// SPECIFIC USE. ENTIRE RISK TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	AffineTransform.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//


//////////////////////////////////////////////////////////////////////////

#include "Math/AffineTransform.h"

namespace PGMath
{

template <class T>
AffineTransform<T>::AffineTransform()
{
	AffineTransform(PGMath::Point3D<T>(0, 0, 0));	
};

template <class T>
AffineTransform<T>::AffineTransform(const PGMath::Point3D<T>& iOrigin)
{
	SetOrigin(iOrigin);
	// initialize dof array here
	// translation & rotation
	int i=0;
	for (i=0; i<6; i++)
	{
		m_dofs.SetParameter(i, 0.0f);
	}
	
	// scaling
	for (i=6; i<9; i++)
	{
		m_dofs.SetParameter(i, 1.0f);
	}
	
	// shear
	for (i=9; i<12; i++)
	{
		m_dofs.SetParameter(i, 0.0f);
	}	

	// update the matrix here
	updateMatrix();
};	

template <class T>
AffineTransform<T>::AffineTransform(const PGMath::Point3D<T>& iOrigin,
									const PGMath::DegreesOfFreedomBase<T>&	iDofs)
{
	SetOrigin(iOrigin);
	m_dofs = iDofs;
	// update the matrix here
	updateMatrix();
}
	

template <class T>
AffineTransform<T>::AffineTransform(const AffineTransform<T>& iAffineTransform)
{
	*this = iAffineTransform;	
}

template <class T>
AffineTransform<T>::AffineTransform(const PGMath::Matrix4x4<T>& iMatrix4x4)
{
	SetOrigin(PGMath::Point3D<T>(0, 0, 0));
	m_matrix4x4 = iMatrix4x4;	
}

template <class T>
AffineTransform<T>& AffineTransform<T>::operator=(const AffineTransform<T>& iAffineTransform)
{
	m_matrix4x4 = iAffineTransform.ConstMatrix4x4();
	return *this;
}

template <class T>
AffineTransform<T>::~AffineTransform(void) {};
	
template <class T>
const Matrix4x4<T>& AffineTransform<T>::ConstMatrix4x4() const 
	{		
		return m_matrix4x4;
	};

template <class T>
Matrix4x4<T>& AffineTransform<T>::Matrix4x4()  
	{		
		return m_matrix4x4;
	};

template <class T>
bool AffineTransform<T>::ConcatPost(const TransformBase<T> *iTransform)
{
	if (!iTransform)
	{
		return false;
	}	

	const PGMath::AffineTransform<T> *iAffineTransform = (static_cast<const PGMath::AffineTransform< T > *>(iTransform));
	if (!iAffineTransform)
	{
		return false;
	}

	m_matrix4x4.Multiply(iAffineTransform->ConstMatrix4x4(), m_matrix4x4); 
	
	return true;
}

template <class T>
bool AffineTransform<T>::ConcatPre(const TransformBase<T> *iTransform)
{
	if (!iTransform)
	{
		return false;
	}	

	const PGMath::AffineTransform<T> *iAffineTransform = (static_cast<const PGMath::AffineTransform< T > *>(iTransform));
	if (!iAffineTransform)
	{
		return false;
	}

	PGMath::Matrix4x4< T > iMatrix4x4 = iAffineTransform->ConstMatrix4x4();
		
	iMatrix4x4.Multiply(m_matrix4x4, m_matrix4x4);
	
	return true;
}

template <class T>
bool AffineTransform<T>::Invert(TransformBase<T> *oTransform/*=0*/)
{
	bool res = m_matrix4x4.Invert(m_matrix4x4);
	if (res && oTransform!=NULL)
	{
		PGMath::AffineTransform<T> *oAffineTransform = (static_cast<PGMath::AffineTransform< T > *>(oTransform));
		*oAffineTransform = PGMath::AffineTransform<T>(m_matrix4x4);
	}
	return res;
}


template <class T>
bool AffineTransform<T>::Apply(const Vector3D<T>& iVector, Vector3D<T>& oVector) const
{
	return m_matrix4x4.Multiply(iVector, oVector);
}

template <class T>
bool AffineTransform<T>::Apply(const Point3D<T>& iPoint, Point3D<T>& oPoint) const
{
	return m_matrix4x4.Multiply(iPoint, oPoint);
}

// private
template <class T>
bool AffineTransform<T>::updateMatrix()
{
	m_matrix4x4.Identity();

	// scale
	PGMath::Matrix4x4< T > scaleMatrix4x4;
	Point3D<T> scaleParams (m_dofs.GetParameter(kPgDegreeOfFreedomScalingX), 
				m_dofs.GetParameter(kPgDegreeOfFreedomScalingY),
				m_dofs.GetParameter(kPgDegreeOfFreedomScalingZ));
	createMatrixScaling(scaleParams, scaleMatrix4x4);

	// translate
	PGMath::Matrix4x4< T > transMatrix4x4;
	Point3D<T> transParams (m_dofs.GetParameter(kPgDegreeOfFreedomTranslationX), 
				m_dofs.GetParameter(kPgDegreeOfFreedomTranslationY),
				m_dofs.GetParameter(kPgDegreeOfFreedomTranslationZ));
	createMatrixTranslation(transParams, transMatrix4x4);

	// then translate to origin
	PGMath::Matrix4x4< T > transMatrix4x4ToOrg;
	Point3D<T> transToOrigin = -m_origin;	
	createMatrixTranslation(transToOrigin, transMatrix4x4ToOrg);

	// then rotate
	PGMath::Matrix4x4< T > rotationMatrix4x4;
	Point3D<T> rotationParams (m_dofs.GetParameter(kPgDegreeOfFreedomRotationX), 
				m_dofs.GetParameter(kPgDegreeOfFreedomRotationY),
				m_dofs.GetParameter(kPgDegreeOfFreedomRotationZ));
	createMatrixRotation(rotationParams, rotationMatrix4x4);
	
	// then shear

	// translate off from origin
	PGMath::Matrix4x4< T > transMatrix4x4FromOrg;
	Point3D<T> transFromOrigin = m_origin;	
	createMatrixTranslation(transFromOrigin, transMatrix4x4FromOrg);

	// concatenate these transforms
	// scale --> transtoorigin --> rotate --> transfromorg --> translate from dof
	m_matrix4x4.Multiply(transMatrix4x4,		m_matrix4x4);	
	m_matrix4x4.Multiply(scaleMatrix4x4,		m_matrix4x4);		
	m_matrix4x4.Multiply(transMatrix4x4ToOrg,	m_matrix4x4);	
	m_matrix4x4.Multiply(rotationMatrix4x4,		m_matrix4x4);	
	m_matrix4x4.Multiply(transMatrix4x4FromOrg,	m_matrix4x4);
	

	return true;
}

template <class T>
bool AffineTransform<T>::createMatrixTranslation	(const PGMath::Point3D<T>&  iTranslationParams,
								 PGMath::Matrix4x4<T>&	oMatrix4x4) // units ?
{
	oMatrix4x4.Identity();
	oMatrix4x4.SetElement(0, 3, iTranslationParams.X());
	oMatrix4x4.SetElement(1, 3, iTranslationParams.Y());
	oMatrix4x4.SetElement(2, 3, iTranslationParams.Z());
	return true;
}

template <class T>
bool AffineTransform<T>::createMatrixRotation	(const PGMath::Point3D<T>&  iRotationParams,
								 PGMath::Matrix4x4<T>&	oMatrix4x4) // units : degrees 
{

#ifdef PG_USE_QUATERNIONS
	_quatX.initAxisAngle(1, 0, 0, rx);
	_quatY.initAxisAngle(0, 1, 0, ry);
	_quatZ.initAxisAngle(0, 0, 1, rz);

	_quatX.multiply(&_quatY);
	_quatX.multiply(&_quatZ);

	_quatX.getMatrix(outMat);
#else 
	T rx = iRotationParams.X();
	T ry = iRotationParams.Y();
	T rz = iRotationParams.Z();

	T cosrx = cos(rx*kMathConstantPI/180.0f);
	T cosry = cos(ry*kMathConstantPI/180.0f);
	T cosrz = cos(rz*kMathConstantPI/180.0f);
	T sinrx = sin(rx*kMathConstantPI/180.0f);
	T sinry = sin(ry*kMathConstantPI/180.0f);
	T sinrz = sin(rz*kMathConstantPI/180.0f);

	oMatrix4x4.Identity();	
	oMatrix4x4.SetElement(0, 0, cosry*cosrz);
	oMatrix4x4.SetElement(0, 1, cosry*sinrz);
	oMatrix4x4.SetElement(0, 2, -sinry);
	oMatrix4x4.SetElement(1, 0, (sinrx*sinry*cosrz-cosrx*sinrz));
	oMatrix4x4.SetElement(1, 1, (sinrx*sinry*sinrz+cosrx*cosrz));
	oMatrix4x4.SetElement(1, 2, sinrx*cosry);
	oMatrix4x4.SetElement(2, 0, (cosrx*sinry*cosrz+sinrx*sinrz));
	oMatrix4x4.SetElement(2, 1, (cosrx*sinry*sinrz-sinrx*cosrz));
	oMatrix4x4.SetElement(2, 2, cosrx*cosry);
#endif
	return true;
}


template <class T>
bool AffineTransform<T>::createMatrixScaling		(const PGMath::Point3D<T>&  iScalingParams,
								 PGMath::Matrix4x4<T>&	oMatrix4x4) // units : none 
{
	oMatrix4x4.Identity();
	oMatrix4x4.SetElement(0, 0, iScalingParams.X());	
	oMatrix4x4.SetElement(1, 1, iScalingParams.Y());
	oMatrix4x4.SetElement(2, 2, iScalingParams.Z());	
	return true;
}

template <class T>
bool AffineTransform<T>::createMatrixShear		(const PGMath::Point3D<T>&  iShearParams,
								 PGMath::Matrix4x4<T>&	oMatrix4x4) // units : ?
{
	return false;
}
 

#ifdef _PG_GENERATE_SDK_LIBS_
	template class AffineTransform<float>; 	
#endif

};
//////////////////////////////////////////////////////////////////////////
// EOF

