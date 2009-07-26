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
#ifndef __gdcmModule_h
#define __gdcmModule_h

#include "gdcmTypes.h"
#include "gdcmTag.h"
#include "gdcmModuleEntry.h"

#include <map>

namespace gdcm
{
/**
 * \brief Class for representing a Module
 * \note bla
 * Module: A set of Attributes within an Information Entity or Normalized IOD which
 * are logically related to each other.
 * \sa Dict
 */
class GDCM_EXPORT Module
{
public:
  typedef std::map<Tag, ModuleEntry> MapModuleEntry;
  typedef MapModuleEntry::const_iterator ConstIterator;
  typedef MapModuleEntry::iterator Iterator;
  ConstIterator Begin() const { return ModuleInternal.begin(); }
  Iterator Begin() { return ModuleInternal.begin(); }
  ConstIterator End() const { return ModuleInternal.end(); }
  Iterator End() { return ModuleInternal.end(); }

  Module() {}
  friend std::ostream& operator<<(std::ostream& _os, const Module &_val);

  void Clear() { ModuleInternal.clear(); }

  void AddModuleEntry(const Tag& tag, const ModuleEntry & module )
    {
    ModuleInternal.insert(
      MapModuleEntry::value_type(tag, module));
    }
  const ModuleEntry& GetModuleEntry(const Tag &tag) const 
    {
    MapModuleEntry::const_iterator it = ModuleInternal.find(tag);
    assert( it->first == tag );
    return it->second;
    }
private:
  //Module &operator=(const Module &_val); // purposely not implemented
  //Module(const Module &_val); // purposely not implemented

  MapModuleEntry ModuleInternal;
};
//-----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& _os, const Module &_val)
{
  Module::MapModuleEntry::const_iterator it = _val.ModuleInternal.begin();
  for(;it != _val.ModuleInternal.end(); ++it)
    {
    const Tag &t = it->first;
    const ModuleEntry &de = it->second;
    _os << t << " " << de << '\n';
    }

  return _os;
}

typedef Module Macro;
//class GDCM_EXPORT Macro : public Module {};

} // end namespace gdcm

#endif //__gdcmModule_h

