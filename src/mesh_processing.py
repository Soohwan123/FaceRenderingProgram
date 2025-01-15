import numpy as np
from scipy.spatial import Delaunay

class MeshProcessor:
    def __init__(self):
        pass

    def create_mesh(self, points):
        # 2D 점들로 Delaunay 삼각분할 수행
        # 모든 점에서 x, y 좌표만 추출
        points_2d = points[:, :2]
        # Delaunay 삼각분할 수행 / 점들을 삼각형으로 연결하는 알고리즘
        tri = Delaunay(points_2d)
        
        # 삼각형 인덱스 반환
        return tri.simplices

    def save_to_obj(self, points, triangles, filename):
        with open(filename, 'w') as f:
            # 버텍스 저장
            for point in points:
                f.write(f'v {point[0]} {point[1]} {point[2]}\n')
            
            # 삼각형 저장 (OBJ는 1-based 인덱싱 사용)
            for triangle in triangles:
                f.write(f'f {triangle[0]+1} {triangle[1]+1} {triangle[2]+1}\n') 