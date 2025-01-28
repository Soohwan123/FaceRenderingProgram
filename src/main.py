import cv2
from face_detection import FaceDetector
from mesh_processing import MeshProcessor
import traceback
import os

def main():
    try:
        print("프로그램 시작...")
        
        # 출력 폴더 생성
        if not os.path.exists('models'):
            os.makedirs('models')
        
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("카메라를 열 수 없습니다!")
            return
            
        print("카메라 초기화 완료")
        
        detector = FaceDetector()
        processor = MeshProcessor()
        print("detector와 processor 초기화 완료")
        
        save_requested = False
        
        while True:
            ret, frame = cap.read()
            if not ret:
                print("프레임을 읽을 수 없습니다!")
                break
                
            # 얼굴 특징점 감지 (이제 UV 좌표도 함께 반환)
            points, nose_points, uv_coords = detector.detect_landmarks(frame)
            
            if points is not None:
                print(f"특징점 {len(points)}개 감지됨 (코 특징점: {len(nose_points)}개, UV 좌표: {len(uv_coords)}개)")
                
                # 특징점 그리기
                frame = detector.draw_landmarks(frame, points, nose_points)
                
                # 메쉬 생성
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
                
                # 's' 키를 누르면 OBJ 파일 저장
                if save_requested:
                    current_dir = os.path.dirname(os.path.abspath(__file__))  # src 폴더
                    project_root = os.path.dirname(current_dir)  # src의 상위 폴더
                    build_dir = os.path.join(project_root, 'build')  # build 폴더
                    renderer_dir = os.path.join(build_dir, 'renderer')  # renderer 폴더
                    models_dir = os.path.join(renderer_dir, 'models')  # models 폴더
                    obj_path = os.path.join(models_dir, 'face.obj')

                    # 기존 파일 삭제
                    if os.path.exists(obj_path):
                        try:
                            os.remove(obj_path)
                            print(f"기존 OBJ 파일 삭제됨: {obj_path}")
                        except Exception as e:
                            print(f"기존 파일 삭제 중 에러 발생: {str(e)}")

                    # OBJ 파일 저장 (이제 UV 좌표도 함께 저장)
                    print(f"OBJ 파일 저장 중... ({obj_path})")
                    processor.save_to_obj(points, triangles, obj_path, nose_points, uv_coords)
                    print("OBJ 파일 저장 완료")
                    save_requested = False
            else:
                print("얼굴이 감지되지 않음")
            
            # 화면에 안내 텍스트 추가
            cv2.putText(frame, "Press 's' to save OBJ file", (10, 30), 
                       cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            
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

if __name__ == "__main__":
    main()