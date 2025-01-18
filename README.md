# Face Rendering Program

이 프로그램은 실시간 얼굴 인식과 3D 렌더링을 결합한 애플리케이션입니다. OpenCV를 사용하여 얼굴을 감지하고 메쉬를 생성한 후, OpenGL을 사용하여 3D로 렌더링합니다.

## 프로젝트 구조
```
FACERENDERINGPROGRAM/
├── src/
│   ├── main.py              # 메인 Python 프로그램
│   ├── face_detection.py    # 얼굴 감지 클래스
│   └── mesh_processing.py   # 메쉬 처리 클래스
├── renderer/
│   ├── src/
│   │   ├── main.cpp        # OpenGL 렌더러 메인
│   │   ├── mesh.h/cpp      # 메쉬 클래스
│   │   ├── shader.h/cpp    # 셰이더 클래스
│   │   ├── camera.h/cpp    # 카메라 클래스
│   │   └── model.h/cpp     # 3D 모델 클래스
│   └── shaders/
│       ├── vertex.glsl     # 버텍스 셰이더
│       └── fragment.glsl   # 프래그먼트 셰이더
├── models/                  # 생성된 OBJ 파일 저장
└── build/                  # 빌드 디렉토리
```

## 주요 기능
1. **실시간 얼굴 인식**
   - 웹캠을 통한 실시간 얼굴 감지
   - 얼굴 특징점 추출
   - 실시간 메쉬 생성 및 시각화

2. **3D 모델 생성**
   - 감지된 얼굴에서 3D 메쉬 생성
   - OBJ 파일 형식으로 저장

3. **3D 렌더링**
   - OpenGL을 사용한 3D 모델 렌더링
   - 카메라 제어 기능
   - 기본 셰이딩

## 사용 방법
1. 프로그램 실행:
   ```bash
   python src/main.py
   ```

2. 얼굴 인식 모드:
   - 웹캠이 실행되며 얼굴을 감지합니다
   - 'S' 키를 눌러 현재 얼굴을 캡처하고 3D 모델로 저장
   - 'Q' 키를 눌러 종료

3. 3D 렌더링 모드:
   - build/FaceRenderer.exe 실행
   - WASD: 카메라 이동
   - 마우스: 시점 변경
   - ESC: 렌더러 종료

## 요구사항
- Python 3.11
- OpenCV
- NumPy
- C++ 17 이상
- OpenGL 3.3 이상
- GLEW
- GLFW
- GLM

## 빌드 방법
1. 렌더러 빌드:
   ```bash
   cd build
   cmake ..
   make
   ```

2. Python 의존성 설치:
   ```bash
   pip install -r requirements.txt
   ```

## 주의사항
- 웹캠이 필요합니다
- 충분한 조명이 필요합니다
- Windows 10/11 환경에서 테스트되었습니다

## 문제 해결
- **카메라가 작동하지 않을 경우**: 장치 관리자에서 웹캠이 정상적으로 인식되는지 확인하세요.
- **렌더러가 실행되지 않을 경우**: build 디렉토리에 FaceRenderer.exe가 제대로 생성되었는지 확인하세요.
- **얼굴이 감지되지 않을 경우**: 충분한 조명이 있는지 확인하고, 정면을 바라보세요.


## 향후 개선 사항
- [ ] 실시간 얼굴 렌더링
- [ ] 성능 최적화

## 버전 업데이트
0.3v -> 텍스쳐와 라이팅 추가 완료

