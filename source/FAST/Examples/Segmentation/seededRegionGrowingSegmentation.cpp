/**
 * Examples/Segmentation/seededRegionGrowingSegmentation.cpp
 *
 * If you edit this example, please also update the wiki and source code file in the repository.
 */
#include <FAST/Tools/CommandLineParser.hpp>
#include "FAST/Importers/ImageFileImporter.hpp"
#include "FAST/Algorithms/SeededRegionGrowing/SeededRegionGrowing.hpp"
#include "FAST/Algorithms/SurfaceExtraction/SurfaceExtraction.hpp"
#include "FAST/Visualization/TriangleRenderer/TriangleRenderer.hpp"
#include "FAST/Visualization/SimpleWindow.hpp"

using namespace fast;

int main(int argc, char** argv) {
    CommandLineParser parser("Seeded region growing");
    parser.addPositionVariable(1, "filename", Config::getTestDataPath() + "CT/CT-Abdomen.mhd");
    parser.addVariable("min-intensity", "150");
    parser.addVariable("max-intensity", "5000");
    parser.parse(argc, argv);

    // Import CT image
    auto importer = ImageFileImporter::New();
    importer->setFilename(parser.get("filename"));

    // Perform region growing segmentation
    auto segmentation = SeededRegionGrowing::New();
    segmentation->setInputConnection(importer->getOutputPort());
    segmentation->addSeedPoint(223,282,387);
    segmentation->addSeedPoint(251,314,148);
    segmentation->setIntensityRange(parser.get<int>("min-intensity"), parser.get<int>("max-intensity"));

    // Extraction surface mesh from segmentation
    auto extraction = SurfaceExtraction::New();
    extraction->setInputConnection(segmentation->getOutputPort());

    // Render and visualize the mesh
    auto surfaceRenderer = TriangleRenderer::New();
    surfaceRenderer->setInputConnection(extraction->getOutputPort());
    surfaceRenderer->enableRuntimeMeasurements();

	auto window = SimpleWindow::New();
    window->addRenderer(surfaceRenderer);
#ifdef FAST_CONTINUOUS_INTEGRATION
	// This will automatically close the window after 5 seconds, used for CI testing
    window->setTimeout(5*1000);
#endif
    window->start();
}
