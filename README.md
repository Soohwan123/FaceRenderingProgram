#Face Rendering Program
This application integrates real-time face detection with 3D rendering. It utilizes OpenCV for facial detection and mesh generation, followed by OpenGL for high-performance 3D rendering.

Project Structure

FACERENDERINGPROGRAM/
├── src/
│   ├── main.py              # Main Python entry point
│   ├── face_detection.py    # Face detection logic/classes
│   └── mesh_processing.py   # Mesh generation and processing
├── renderer/
│   ├── src/
│   │   ├── main.cpp         # OpenGL renderer entry point
│   │   ├── mesh.h/cpp       # Mesh management classes
│   │   ├── shader.h/cpp     # Shader compilation and management
│   │   ├── camera.h/cpp     # Camera abstraction
│   │   └── model.h/cpp      # 3D model loading and handling
│   └── shaders/
│       ├── vertex.glsl      # Vertex shader source
│       └── fragment.glsl    # Fragment shader source
├── models/                  # Storage for generated OBJ files
└── build/                   # Build artifacts and binaries

##Key Features
Real-time Face Detection

-Real-time detection via webcam feed.

-Facial landmark extraction.

-Dynamic mesh generation and visualization.

3D Model Generation

-Constructs 3D meshes based on detected facial landmarks.

-Exports models to the industry-standard .OBJ format.

3D Rendering

-High-performance rendering using OpenGL 3.3+.

-Interactive camera controls (Free-look).

-Basic lighting and shading implementation.

## Usage Instructions
1. Execute Program:

**python src/main.py**

2. Face Detection Mode:
The webcam initializes and starts detecting faces automatically.

'S' Key: Capture the current face and export it as a 3D model (.obj).

'Q' Key: Terminate the application.

3. 3D Rendering Mode:
Execute the binary: build/FaceRenderer.exe (or ./FaceRenderer on Unix-like systems).

WASD: Move camera position.

Mouse: Change look-at direction.

ESC: Exit the renderer.

##Requirements
Software Environment
-Python: 3.11+

-C++: C++17 Standard or higher

-OpenGL: Version 3.3 or higher

##Dependencies
Python: OpenCV, NumPy

C++: GLEW, GLFW, GLM

##Build Instructions
1. Build the Renderer:
cd build
cmake ..
make

3. Install Python Dependencies:
**pip install -r requirements.txt

##Troubleshooting
Camera Initialization Failure: Ensure the webcam is correctly recognized in the Device Manager and not occupied by another process.

Binary Execution Issues: Verify that FaceRenderer.exe was successfully generated in the build directory after the make command.

Detection Accuracy: Low-light environments significantly degrade detection performance. Ensure adequate lighting and a clear frontal view of the face.

##Future Roadmap
[ ] Implement real-time 3D face rendering (live sync).

[ ] Optimize mesh processing performance for lower latency.

##Version History
v0.3: Added support for Textures and Lighting.

Note: This project was primarily developed and tested on Windows 10/11 environments.
