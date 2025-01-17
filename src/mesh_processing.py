import numpy as np
from scipy.spatial import Delaunay

class MeshProcessor:
    def __init__(self):
        pass

    def create_mesh(self, points):
        # 2D 점들로 Delaunay 삼각분할 수행
        # 모든 점에서 x, y 좌표만 추출
        points_2d = points[:, :2]
        # 들로네 삼각분할 수행 / 점들을 삼각형으로 연결하는 알고리즘
        tri = Delaunay(points_2d)
        
        # 삼각형 인덱스 반환
        return tri.simplices

    def save_to_obj(self, points, triangles, filepath, nose_points):
        try:
            with open(filepath, 'w') as f:
                # 헤더 정보 추가
                f.write("# Face mesh OBJ file\n")
                f.write(f"# Total vertices: {len(points)}\n")
                f.write(f"# Total triangles: {len(triangles)}\n")
                f.write(f"# Nose vertices: {len(nose_points)}\n\n")
                
                # vertices 저장
                for point in points:
                    f.write(f"v {point[0]} {point[1]} {point[2]}\n")
                
                # 코 vertices 정보 저장
                f.write("\n# Nose vertices indices (1-based):\n")
                for nose_point in nose_points:
                    # points 배열에서 nose_point의 인덱스 찾기
                    indices = np.where(np.all(points == nose_point, axis=1))[0]
                    if len(indices) > 0:
                        # OBJ는 1-based indexing이므로 1을 더함
                        f.write(f"# nose_vertex {indices[0] + 1}\n")
                
                # faces 저장
                f.write("\n# Faces\n")
                for triangle in triangles:
                    # OBJ는 1-based indexing이므로 각 인덱스에 1을 더함
                    f.write(f"f {triangle[0]+1} {triangle[1]+1} {triangle[2]+1}\n")

            print(f"OBJ 파일이 성공적으로 저장됨: {filepath}")
            print(f"저장된 vertices 수: {len(points)}")
            print(f"저장된 삼각형 수: {len(triangles)}")
            print(f"저장된 코 vertices 수: {len(nose_points)}")

        except Exception as e:
            print(f"OBJ 파일 저장 중 에러 발생: {str(e)}")
            raise 