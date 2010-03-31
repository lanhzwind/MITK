/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.12 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#ifndef mitkVector3dPropertyDeserializer_h_included
#define mitkVector3dPropertyDeserializer_h_included

#include "mitkBasePropertyDeserializer.h"

#include "mitkProperties.h"

namespace mitk
{

class SceneSerialization_EXPORT Vector3DPropertyDeserializer : public BasePropertyDeserializer
{
  public:
    
    mitkClassMacro( Vector3DPropertyDeserializer, BasePropertyDeserializer );
    itkNewMacro(Self);

    virtual BaseProperty::Pointer Deserialize(TiXmlElement* element)
    {
      if (!element) return NULL;

      Vector3D v;
      if ( element->QueryFloatAttribute( "x", &v[0] ) != TIXML_SUCCESS ) return NULL;
      if ( element->QueryFloatAttribute( "y", &v[1] ) != TIXML_SUCCESS ) return NULL;
      if ( element->QueryFloatAttribute( "z", &v[2] ) != TIXML_SUCCESS ) return NULL;

     return Vector3DProperty::New( v ).GetPointer();
    }

  protected:

    Vector3DPropertyDeserializer() {}
    virtual ~Vector3DPropertyDeserializer() {}
};

} // namespace

// important to put this into the GLOBAL namespace (because it starts with 'namespace mitk')
MITK_REGISTER_SERIALIZER(Vector3DPropertyDeserializer);

#endif

