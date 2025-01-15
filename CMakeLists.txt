cmake_minimum_required(VERSION 3.10)
project(FaceRenderingProgram)

# Python 부분
# Python은 CMake로 관리할 필요가 없으므로 제거하거나,
# 필요한 경우 Python 관련 설정만 추가

# C++ 렌더러 부분을 별도 디렉토리로 분리
add_subdirectory(renderer)