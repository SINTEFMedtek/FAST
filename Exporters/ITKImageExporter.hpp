#ifndef ITKIMAGEEXPORTER_HPP_
#define ITKIMAGEEXPORTER_HPP_

#include "PipelineObject.hpp"
#include "Image2D.hpp"

#include <itkImageSource.h>
#include <itkImageRegionIterator.h>

namespace fast {

template<class TImage>
class ITKImageExporter: public itk::ImageSource<TImage>, public PipelineObject {
    FAST_OBJECT(ITKImageExporter)
    public:
        void setInput(Image2D::Ptr image);
    private:
        Image2D::Ptr mInput;

        ITKImageExporter();
        void execute() {
        }
        ;

        // Is called by ITK
        void GenerateData();

};

} // end namespace fast

template<class TImage>
inline void fast::ITKImageExporter<TImage>::setInput(Image2D::Ptr image) {
    mInput = image;
}

template<class TImage>
inline fast::ITKImageExporter<TImage>::ITKImageExporter() {
}

template<class TImage>
inline void fast::ITKImageExporter<TImage>::GenerateData() {
    typename TImage::Pointer output = this->GetOutput();
    typename TImage::RegionType region;
    typename TImage::IndexType start;
    start[0] = 0;
    start[1] = 0;

    typename TImage::SizeType size;
    size[0] = mInput->getWidth();
    size[1] = mInput->getHeight();

    region.SetSize(size);
    region.SetIndex(start);

    output->SetRegions(region);
    output->Allocate();

    // TODO support different data types
    mInput->update();
    ImageAccess2D access = mInput->getImageAccess(ACCESS_READ);
    float * fastPixelData = (float *) access.get();

    itk::ImageRegionIterator<TImage> imageIterator(output,
            output->GetLargestPossibleRegion());
    unsigned int width = mInput->getWidth();

    while (!imageIterator.IsAtEnd()) {
        unsigned int x = imageIterator.GetIndex()[0];
        unsigned int y = imageIterator.GetIndex()[1];
        unsigned int z = imageIterator.GetIndex()[2];
        imageIterator.Set(fastPixelData[x + y * width]);

        ++imageIterator;
    }
}

#endif // ITKIMAGEEXPORTER_HPP_