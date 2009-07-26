/*=========================================================================

  Program: GDCM (Grass Root DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2008 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef __gdcmIPPSorter_h
#define __gdcmIPPSorter_h

#include "gdcmSorter.h"

#include <vector>
#include <string>

namespace gdcm
{
/**
 * \brief IPPSorter
 * Implement a simple Image Position (Patient) sorter, along the Image Orientation 
 * (Patient) direction.
 * This algorithm does NOT support duplicate and will FAIL in case of duplicate
 * IPP.
 * WARNING: See special note for SetZSpacingTolerance when computing the ZSpacing
 * from the IPP of each DICOM files
 */
class GDCM_EXPORT IPPSorter : public Sorter
{
public:
  IPPSorter();
  ~IPPSorter();

  // FIXME: I do not like public virtual function...
  // Main entry point to the sorter.
  // It will execute the filter, option should be set before
  // running this function (SetZSpacingTolerance, ...)
  virtual bool Sort(std::vector<std::string> const & filenames);

  // Functions related to Z-Spacing computation
  // Set to true when sort algorithm should also perform a regular
  // Z-Spacing computation using the Image Position (Patient)
  // Potential reason for failure:
  // 1. ALL slices are taken into account, if one slice if 
  // missing then ZSpacing will be set to 0 since the spacing
  // will not be found to be regular along the Series
  void SetComputeZSpacing(bool b) { ComputeZSpacing = b; }
  // 2. Another reason for failure is that that Z-Spacing is only 
  // slightly changing (eg 1e-3) along the serie, a human can determine
  // that this is ok and change the tolerance from its default value: 1e-6
  void SetZSpacingTolerance(double tol) { ZTolerance = tol; }

  // Read-only function to provide access to the computed value for the Z-Spacing
  // The ComputeZSpacing must have been set to true before execution of
  // sort algorithm
  // Z-Spacing will be 0 on 2 occsasions:
  // 1. Sorting simply failed, potentially duplicate IPP => ZSpacing = 0
  // 2. ZSpacing could not be computed (Z-Spacing is not constant, or ZTolerance is too low)
  double GetZSpacing() const { return ZSpacing; }

protected:
  bool ComputeZSpacing;
  double ZSpacing;
  double ZTolerance;
};


} // end namespace gdcm

#endif //__gdcmIPPSorter_h
