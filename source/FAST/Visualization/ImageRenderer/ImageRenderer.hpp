#ifndef IMAGERENDERER_HPP_
#define IMAGERENDERER_HPP_

#include "FAST/Visualization/Renderer.hpp"
#include "FAST/Data/Image.hpp"

namespace fast {

class FAST_EXPORT  ImageRenderer : public Renderer {
    FAST_OBJECT(ImageRenderer)
    public:
        void loadAttributes() override;
        void setIntensityLevel(float level);
        float getIntensityLevel();
        void setIntensityWindow(float window);
        float getIntensityWindow();
    protected:
        ImageRenderer();
        void draw();
        void draw2D(
                cl::Buffer PBO,
                uint width,
                uint height,
                Affine3f pixelToViewportTransform,
                float PBOspacing,
                Vector2f translation
        );

        std::unordered_map<uint, uint> mTexturesToRender;
        std::unordered_map<uint, Image::pointer> mImageUsed;
        std::unordered_map<uint, uint> mVAO;

        cl::Kernel mKernel;

        // Level and window intensities
        float mWindow;
        float mLevel;

};

}




#endif /* IMAGERENDERER_HPP_ */
