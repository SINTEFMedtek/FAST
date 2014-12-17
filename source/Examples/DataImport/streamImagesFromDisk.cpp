/**
 * Examples/DataImport/streamImagesFromDisk.cpp
 *
 * If you edit this example, please also update the wiki and source code file in the repository.
 */
#include "ImageFileStreamer.hpp"
#include "ImageRenderer.hpp"
#include "SimpleWindow.hpp"

using namespace fast;

int main() {
    // Import images from files using the ImageFileStreamer
    ImageFileStreamer::pointer streamer = ImageFileStreamer::New();
    // The hashtag here will be replaced with an integer, starting with 0 as default
    streamer->setFilenameFormat(std::string(FAST_TEST_DATA_DIR)+"/US-2Dt/US-2Dt_#.mhd");

    // Renderer image
    ImageRenderer::pointer renderer = ImageRenderer::New();
    renderer->addInput(streamer->getOutput());
    SimpleWindow::pointer window = SimpleWindow::New();
    window->addRenderer(renderer);
    window->setTimeout(5*1000); // automatically close window after 5 seconds
    window->start();
}
