/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkFEMSpatialObjectWriter_h
#define itkFEMSpatialObjectWriter_h

#include "itkSpatialObjectWriter.h"
#include "itkMetaFEMObjectConverter.h"

namespace itk
{
/** \class FEMSpatialObjectWriter
 *
 * \brief Write FEM SpatialObjects to a file
 *
 * \ingroup ITKFEM
 */
template <unsigned int VDimension = 3,
          typename PixelType = unsigned char,
          typename TMeshTraits = DefaultStaticMeshTraits<PixelType, VDimension, VDimension>>
class ITK_TEMPLATE_EXPORT FEMSpatialObjectWriter : public SpatialObjectWriter<VDimension, PixelType, TMeshTraits>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(FEMSpatialObjectWriter);

  using Self = FEMSpatialObjectWriter;
  using Superclass = SpatialObjectWriter<VDimension, PixelType, TMeshTraits>;
  using Pointer = SmartPointer<Self>;

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(FEMSpatialObjectWriter);

  /** Method for creation through the object factory */
  itkNewMacro(Self);

protected:
  std::string m_FileName{};

  FEMSpatialObjectWriter();
  ~FEMSpatialObjectWriter() override = default;
};

template <unsigned int VDimension, typename PixelType, typename TMeshTraits>
FEMSpatialObjectWriter<VDimension, PixelType, TMeshTraits>::FEMSpatialObjectWriter()
{
  this->RegisterMetaConverter("FEMObject", "FEMObjectSpatialObject", MetaFEMObjectConverter<VDimension>::New());
}

} // namespace itk

#endif // itkFEMSpatialObjectWriter_h
