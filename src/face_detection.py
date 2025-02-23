import mediapipe as mp
import numpy as np
import cv2

class FaceDetector:
    def __init__(self):
        # MediaPipe의 얼굴 메시 모듈 초기화
        self.mp_face_mesh = mp.solutions.face_mesh
        
        # FaceMesh 객체 생성
        self.face_mesh = self.mp_face_mesh.FaceMesh(
            static_image_mode=False,    # False: 비디오용 최적화, True: 정적 이미지용
            max_num_faces=1,            # 감지할 최대 얼굴 수
            min_detection_confidence=0.5,  # 얼굴 감지 최소 신뢰도
            min_tracking_confidence=0.5,   # 랜드마크 추적 최소 신뢰도
            refine_landmarks=True       # 입과 눈 주변의 랜드마크를 더 정밀하게 처리
        )
        
        # MediaPipe 드로잉 유틸리티 (시각화용)
        self.mp_drawing = mp.solutions.drawing_utils
        self.mp_drawing_styles = mp.solutions.drawing_styles
        
        # 코 관련 랜드마크 인덱스 정의
        self.nose_indices = [1, 2, 3, 4, 5, 6, 168, 195, 197, 168]  # 코 부분 랜드마크

    def detect_landmarks(self, image):
        # MediaPipe는 RGB 형식을 사용하므로 BGR(OpenCV 기본)에서 변환
        image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        
        # 얼굴 랜드마크 감지 실행
        results = self.face_mesh.process(image_rgb)
        
        # 얼굴이 감지된 경우
        if results.multi_face_landmarks:
            # 첫 번째 얼굴의 랜드마크 가져오기
            face_landmarks = results.multi_face_landmarks[0]
            
            # 478개의 3D 좌표를 저장할 배열 생성
            points = np.zeros((478, 3))  # (x, y, z) 좌표
            nose_points = []  # 코 점들을 따로 저장
            uv_coords = []    # UV 좌표를 저장할 리스트
            
            # 각 랜드마크의 좌표 추출
            for idx, landmark in enumerate(face_landmarks.landmark):
                points[idx] = [
                    landmark.x * image.shape[1],  # x좌표: 0~1 값을 이미지 너비에 맞게 변환
                    landmark.y * image.shape[0],  # y좌표: 0~1 값을 이미지 높이에 맞게 변환
                    landmark.z * 760  # z좌표: 깊이 정보를 보기 좋게 스케일 조정
                ]
                
                # UV 좌표도 이미지 크기에 맞게 스케일 조정
                uv_coords.append([
                    landmark.x * image.shape[1],  # u 좌표를 이미지 너비에 맞춤
                    landmark.y * image.shape[0]   # v 좌표를 이미지 높이에 맞춤
                ])
                
                # 코 부분 랜드마크인 경우 따로 저장
                if idx in self.nose_indices:
                    nose_points.append(points[idx])
            
            return points, nose_points, np.array(uv_coords)
        return None, None, None

    def draw_landmarks(self, image, points, nose_points):
        if points is None:
            return image
        
        # 각 랜드마크를 이미지에 그리기
        for point in points:
            x, y = int(point[0]), int(point[1])  # 실수 좌표를 정수로 변환
            # 원으로 랜드마크 표시
            # (x,y): 중심점, 1: 반지름, (0,255,0): 초록색, -1: 원 내부 채우기
            cv2.circle(image, (x, y), 1, (0, 255, 0), -1)
            
            # 코 랜드마크는 빨간색으로 표시
            if any(np.array_equal(point, nose_point) for nose_point in nose_points):
                cv2.circle(image, (x, y), 2, (0, 0, 255), -1)
        
        return image