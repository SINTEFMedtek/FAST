"""
This example demonstrates how a numpy ndarray image can be passed
on to FAST and visualized using FAST.
"""
import fast
import numpy as np
from PIL import Image

fast.downloadTestDataIfNotExists() # This will download the test data needed to run the example

# Open image from file using pillow and convert to numpy ndarray
numpy_image = np.asarray(Image.open(fast.Config.getTestDataPath() + 'US/US-2D.bmp'))

# Make FAST image from the numpy ndarray
fast_image = fast.Image.New()
fast_image.createFromArray(numpy_image)

# Display using FAST
renderer = fast.ImageRenderer.New()
renderer.addInputData(fast_image)

window = fast.SimpleWindow.New()
window.addRenderer(renderer)
window.set2DMode()
window.start()
