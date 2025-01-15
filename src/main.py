import cv2
from face_detection import FaceDetector
from mesh_processing import MeshProcessor
import traceback
import os
import subprocess

def main():
    try:
        print("프로그램 시작...")
        
        # models 폴더 생성 (없는 경우)
        models_dir = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'models')
        if not os.path.exists(models_dir):
            os.makedirs(models_dir)
            print("models 폴더 생성됨")
        
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("카메라를 열 수 없습니다!")
            return
            
        print("카메라 초기화 완료")
        
        detector = FaceDetector()
        processor = MeshProcessor()
        print("detector와 processor 초기화 완료")
        
        save_requested = False  # OBJ 저장 플래그
        renderer_start = False  # 렌더러 시작 플래그
        
        while True:
            ret, frame = cap.read()
            if not ret:
                print("프레임을 읽을 수 없습니다!")
                break
                
            # 얼굴 특징점 감지
            print("얼굴 특징점 감지 시작")
            points = detector.detect_landmarks(frame)
            
            if points is not None:
                print(f"특징점 {len(points)}개 감지됨")
                
                # 특징점 그리기
                for point in points:
                    x, y = int(point[0]), int(point[1])
                    cv2.circle(frame, (x, y), 1, (0, 255, 0), -1)
                
                # 메쉬 생성
                print("메쉬 생성 시작")
                triangles = processor.create_mesh(points)
                print(f"삼각형 {len(triangles)}개 생성됨")
                
                # 삼각형 그리기
                for triangle in triangles:
                    pt1 = (int(points[triangle[0]][0]), int(points[triangle[0]][1]))
                    pt2 = (int(points[triangle[1]][0]), int(points[triangle[1]][1]))
                    pt3 = (int(points[triangle[2]][0]), int(points[triangle[2]][1]))
                    
                    cv2.line(frame, pt1, pt2, (255, 255, 255), 1)
                    cv2.line(frame, pt2, pt3, (255, 255, 255), 1)
                    cv2.line(frame, pt3, pt1, (255, 255, 255), 1)
                
                # 's' 키를 누르면 OBJ 파일로 저장
                if save_requested:
                    obj_path = os.path.join(models_dir, 'face.obj')
                    print(f"OBJ 파일 저장 중... ({obj_path})")
                    processor.save_to_obj(points, triangles, obj_path)
                    print("OBJ 파일 저장 완료")
                    save_requested = False
                    renderer_start = True
                    break  # 카메라 캡처 종료
            else:
                print("얼굴이 감지되지 않음")
            
            # 화면에 안내 텍스트 추가
            cv2.putText(frame, "Press 's' to save OBJ file and start renderer", 
                       (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            
            # 결과 표시
            cv2.imshow('Face Mesh', frame)
            
            # 키 입력 처리
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                print("사용자가 'q'를 눌러 종료")
                break
            elif key == ord('s'):
                save_requested = True
                print("OBJ 저장 요청됨")
    
    except Exception as e:
        print("에러 발생:")
        print(traceback.format_exc())
    
    finally:
        print("프로그램 종료 중...")
        cap.release()
        cv2.destroyAllWindows()

        # 렌더러 시작
        if renderer_start:
            try:
                print("렌더러 시작 중...")
        
                current_dir = os.path.dirname(__file__)  # src 디렉토리
                project_root = os.path.dirname(current_dir)  # FACERENDERINGPROGRAM 디렉토리
                renderer_path = os.path.join(project_root, 'build', 'renderer', 'FaceRenderer.exe')
                
                print(f"렌더러 경로: {renderer_path}")
                subprocess.run([renderer_path])
            except Exception as e:
                print("렌더러 실행 중 에러 발생:")
                print(str(e))

if __name__ == "__main__":
    main()