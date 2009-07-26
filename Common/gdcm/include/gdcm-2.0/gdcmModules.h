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
#ifndef __gdcmModules_h
#define __gdcmModules_h

#include "gdcmTypes.h"
#include "gdcmModule.h"

#include <map>

namespace gdcm
{
/**
 * \brief Class for representing a Modules
 * \note bla
 * \sa Module
 */
class GDCM_EXPORT Modules
{
public:
  typedef std::map<std::string, Module> ModuleMapType;

  Modules() {}
  friend std::ostream& operator<<(std::ostream& _os, const Modules &_val);

  void Clear() { ModulesInternal.clear(); }

  void AddModule(const char *name , const Module & module )
    {
    ModulesInternal.insert(
      ModuleMapType::value_type(name, module));
    }
  const Module &GetModule(const char *name) const
    {
    ModuleMapType::const_iterator it = ModulesInternal.find( name );
    assert( it->first == name );
    return it->second;
    }

  bool IsEmpty() const { return ModulesInternal.empty(); }

private:
  ModuleMapType ModulesInternal;
};
//-----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& _os, const Modules &_val)
{
  Modules::ModuleMapType::const_iterator it = _val.ModulesInternal.begin();
  for(;it != _val.ModulesInternal.end(); ++it)
    {
    const std::string &name = it->first;
    const Module &m = it->second;
    _os << name << " " << m << '\n';
    }

  return _os;
}

typedef Modules Macros;


} // end namespace gdcm

#endif //__gdcmModules_h

