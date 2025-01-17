import numpy as np
from scipy.spatial import Delaunay
import os

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

    def save_to_obj(self, points, triangles, filepath, nose_points, uv_coords):
        try:
            with open(filepath, 'w') as f:
                # 헤더 정보 추가
                f.write("# Face mesh OBJ file\n")
                f.write(f"# Total vertices: {len(points)}\n")
                f.write(f"# Total triangles: {len(triangles)}\n")
                f.write(f"# Nose vertices: {len(nose_points)}\n")
                f.write(f"# UV coordinates: {len(uv_coords)}\n\n")
                
                # MTL 파일 참조 추가
                # MTL 파일은 텍스처 정보를 담고 있는 파일
                mtl_path = filepath.replace('.obj', '.mtl')
                f.write(f"mtllib {os.path.basename(mtl_path)}\n")
                f.write("usemtl material0\n\n")
                
                # vertices(v) 저장
                # 3D 공간에서의 정점 위치
                for point in points:
                    f.write(f"v {point[0]} {point[1]} {point[2]}\n")
                
                # UV 좌표(vt) 저장
                # 텍스처 매핑을 위한 2D 좌표 (0~1 범위)
                f.write("\n# Texture coordinates\n")
                for u, v in uv_coords:
                    # OpenGL은 V좌표가 위아래 반전되어 있으므로 1-v 사용
                    f.write(f"vt {u} {1-v}\n")
                
                # 코 vertices 정보 저장 (주석으로)
                f.write("\n# Nose vertices indices (1-based):\n")
                for nose_point in nose_points:
                    # points 배열에서 nose_point의 인덱스 찾기
                    indices = np.where(np.all(points == nose_point, axis=1))[0]
                    if len(indices) > 0:
                        # OBJ는 1-based indexing이므로 1을 더함
                        f.write(f"# nose_vertex {indices[0] + 1}\n")
                
                # faces(f) 저장
                # 삼각형을 이루는 정점들의 인덱스
                f.write("\n# Faces\n")
                for triangle in triangles:
                    # vertex/texture/normal indices 형식으로 저장
                    # 여기서는 normal은 생략하고 vertex와 texture만 사용
                    # OBJ는 1-based indexing이므로 각 인덱스에 1을 더함
                    f.write(f"f {triangle[0]+1}/{triangle[0]+1} {triangle[1]+1}/{triangle[1]+1} {triangle[2]+1}/{triangle[2]+1}\n")

                # MTL 파일 생성
                with open(mtl_path, 'w') as mtl:
                    mtl.write("newmtl material0\n")  # 재질 이름 정의
                    mtl.write("Ka 1.000000 1.000000 1.000000\n")  # 주변광 반사 (ambient)
                    mtl.write("Kd 1.000000 1.000000 1.000000\n")  # 확산 반사 (diffuse)
                    mtl.write("Ks 0.000000 0.000000 0.000000\n")  # 경면 반사 (specular)
                    mtl.write("Tr 0.000000\n")  # 투명도
                    mtl.write("illum 1\n")  # 조명 모델
                    mtl.write(f"map_Kd skin_texture.jpg\n")  # 텍스처 파일 참조

            print(f"OBJ 파일이 성공적으로 저장됨: {filepath}")
            print(f"저장된 vertices 수: {len(points)}")
            print(f"저장된 UV 좌표 수: {len(uv_coords)}")
            print(f"저장된 삼각형 수: {len(triangles)}")
            print(f"저장된 코 vertices 수: {len(nose_points)}")

        except Exception as e:
            print(f"OBJ 파일 저장 중 에러 발생: {str(e)}")
            raise 