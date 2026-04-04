# Face Rendering Program

This application integrates real-time face detection with 3D rendering. It utilizes **OpenCV** for facial landmark detection and mesh generation, followed by **OpenGL** for high-performance 3D visualization.

---
## Project Structure
```text
FACERENDERINGPROGRAM/
├── src/
│   ├── main.py              # Main Python entry point
│   ├── face_detection.py    # Face detection and landmark logic
│   └── mesh_processing.py   # Mesh generation and OBJ processing
├── renderer/
│   ├── src/
│   │   ├── main.cpp         # OpenGL renderer entry point
│   │   ├── mesh.h/cpp       # Mesh management classes
│   │   ├── shader.h/cpp     # Shader compilation and management
│   │   ├── camera.h/cpp     # Camera abstraction (WASD/Mouse)
│   │   └── model.h/cpp      # 3D model loading and handling
│   └── shaders/
│       ├── vertex.glsl      # Vertex shader source
│       └── fragment.glsl    # Fragment shader source
├── models/                  # Storage for generated OBJ files
└── build/                   # Build artifacts and binaries
```
---

## Key Features

### 1. Real-time Face Detection
- **Real-time Tracking**  
  Instant facial detection via webcam feed

- **Landmark Extraction**  
  Precise facial feature point extraction using OpenCV

- **Live Visualization**  
  Real-time mesh generation and wireframe rendering

---

### 2. 3D Model Generation
- **Mesh Construction**  
  Generates 3D meshes based on detected facial landmarks

- **Standard Export**  
  Saves captured models in the industry-standard `.OBJ` format

---

### 3. 3D Rendering
- **Modern Pipeline**  
  High-performance rendering powered by OpenGL 3.3+

- **Interactive Control**  
  Free-look camera system (WASD + Mouse)

- **Advanced Shading**  
  Supports basic shading, textures, and lighting

---

##  Usage Instructions

### 1. Execution
```bash
python src/main.py

## 2. Face Detection Mode
The webcam initializes automatically and starts face detection.

### Controls
- `S` : Capture the current face and export it as a `.obj` file  
- `Q` : Quit the application  

---

## 3. 3D Rendering Mode

### Run the renderer
```bash
build/FaceRenderer.exe   # Windows
./FaceRenderer           # Linux


### Controls
- `WASD` : Move camera  
- `Mouse` : Adjust view direction  
- `ESC` : Exit  

---

##  Requirements

### Environment
- Python: 3.11+
- C++ Compiler: C++17 or higher
- OpenGL: 3.3+

### Dependencies

#### Python
- opencv-python
- numpy

#### C++ Libraries
- GLEW
- GLFW
- GLM

---

##  Build Instructions

### 1. Build the Renderer
```bash
cd build
cmake ..
make


### 2. Install Python Dependencies
```bash
pip install -r requirements.txt

##  Troubleshooting

### Camera Failure
- Ensure the webcam is properly recognized by the OS  
- Make sure it is not being used by another process  

### Binary Issues
- Verify that the `FaceRenderer` executable is successfully generated in the `build` directory  

### Detection Accuracy
- Ensure good lighting conditions  
- Maintain a clear, frontal view of the face  

---

##  Roadmap & Versioning

### Future Enhancements
- [ ] Implement live-sync real-time 3D rendering  
- [ ] Optimize performance for low-latency data processing  

### Version History
- **v0.3**
  - Added texture mapping  
  - Implemented lighting system  

---

##  Notes
- This project has been primarily developed and tested on **Windows 10/11 and Linux environments**
