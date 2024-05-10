import cv2
import mediapipe as mp
import torch
from torch import nn
import numpy as np

class Backend:
    def __init__(self,device="cpu"):
        self.model=slouch_detection()
        self.model.load_state_dict(torch.load("dontSlouch/slouch_detector.pt"))
        self.model = self.model.to(device)
        self.cur_device=device
        self.mp_pose = mp.solutions.pose
        self.pose=self.mp_pose.Pose(min_detection_confidence=0.8,min_tracking_confidence=0.8)
        self.red = (50, 50, 255)
        self.green = (127, 255, 0)
        self.result=""
        self.res=-1
        self.x=0

    def drawPoseLandmarks(self, image, lm, color=(255, 0, 0), thickness=2):
        """
        Draw pose landmarks on the image.

        Args:
            image (numpy.ndarray): The input image.
            lm (mediapipe.framework.formats.landmark_pb2.NormalizedLandmarkList): The pose landmarks.
            color (tuple, optional): The color of the landmarks. Defaults to (255, 0, 0).
            thickness (int, optional): The thickness of the lines. Defaults to 2.

        Returns:
            numpy.ndarray: The image with pose landmarks drawn.
        """
        h, w = image.shape[:2]
    
        # Extracting coordinates for nose, left shoulder, and right shoulder
        nose_x = lm.landmark[self.mp_pose.PoseLandmark.NOSE].x * w
        nose_y = lm.landmark[self.mp_pose.PoseLandmark.NOSE].y * h
        l_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].x * w
        l_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].y * h
        r_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].x * w
        r_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].y * h
        
        # Drawing nose, left shoulder, and right shoulder
        if nose_x and nose_y:
            cv2.circle(image, (int(nose_x), int(nose_y)), 7, color, -1)
        if l_shldr_x and l_shldr_y:
            cv2.circle(image, (int(l_shldr_x), int(l_shldr_y)), 7, color, -1)
        if r_shldr_x and r_shldr_y:
            cv2.circle(image, (int(r_shldr_x), int(r_shldr_y)), 7, color, -1)
        
        # Drawing lines between shoulders and nose
        if l_shldr_x and l_shldr_y and r_shldr_x and r_shldr_y and nose_x and nose_y:
            cv2.line(image, (int(l_shldr_x), int(l_shldr_y)), (int(r_shldr_x), int(r_shldr_y)), color, 2)
            cv2.line(image, (int((l_shldr_x+r_shldr_x)/2), int((l_shldr_y+r_shldr_y)/2)), (int(nose_x), int(nose_y)), color, 2)
        
        # Extracting coordinates for eyes and lips
        left_eye_x = lm.landmark[self.mp_pose.PoseLandmark.LEFT_EYE].x * w
        left_eye_y = lm.landmark[self.mp_pose.PoseLandmark.LEFT_EYE].y * h
        right_eye_x = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_EYE].x * w
        right_eye_y = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_EYE].y * h
        left_lip_x = lm.landmark[self.mp_pose.PoseLandmark.MOUTH_LEFT].x * w
        left_lip_y = lm.landmark[self.mp_pose.PoseLandmark.MOUTH_LEFT].y * h
        right_lip_x = lm.landmark[self.mp_pose.PoseLandmark.MOUTH_RIGHT].x * w
        right_lip_y = lm.landmark[self.mp_pose.PoseLandmark.MOUTH_RIGHT].y * h
        
        # Drawing eyes and lips
        if left_eye_x and left_eye_y:
            cv2.circle(image, (int(left_eye_x), int(left_eye_y)), 7, color, -1)
        if right_eye_x and right_eye_y:
            cv2.circle(image, (int(right_eye_x), int(right_eye_y)), 7, color, -1)
        if left_lip_x and left_lip_y:
            cv2.circle(image, (int(left_lip_x), int(left_lip_y)), 7, color, -1)
        if right_lip_x and right_lip_y:
            cv2.circle(image, (int(right_lip_x), int(right_lip_y)), 7, color, -1)
        
        # Drawing lines between eyes and lips
        if left_eye_x and left_eye_y and right_eye_x and right_eye_y:
            cv2.line(image, (int(left_eye_x), int(left_eye_y)), (int(right_eye_x), int(right_eye_y)), color, 2)
        if left_lip_x and left_lip_y and right_lip_x and right_lip_y:
            cv2.line(image, (int(left_lip_x), int(left_lip_y)), (int(right_lip_x), int(right_lip_y)), color, 2)
        
        return image

    def processImage(self,image, lm, draw_skeleton=False):
        
        if image is None:
            print(f"Failed to read image")
            return
        h, w = image.shape[:2]

        if lm is not None:
            nose_x = lm.landmark[self.mp_pose.PoseLandmark.NOSE].x * w
            nose_y = lm.landmark[self.mp_pose.PoseLandmark.NOSE].y * h
            nose_z = lm.landmark[self.mp_pose.PoseLandmark.NOSE].z * h  # Calculating Z-axis position

            l_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].x * w
            l_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].y * h
            l_shldr_z = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].z * h
            r_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].x * w
            r_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].y * h
            r_shldr_z=lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].z * h
            l_shldr_x -= r_shldr_x
            l_shldr_y -= r_shldr_y
            nose_x -=r_shldr_x
            nose_y -= r_shldr_y

            left_eye_x=lm.landmark[self.mp_pose.PoseLandmark.LEFT_EYE].x * w- r_shldr_x
            left_eye_y=lm.landmark[self.mp_pose.PoseLandmark.LEFT_EYE].y * h- r_shldr_y
            left_eye_z=lm.landmark[self.mp_pose.PoseLandmark.LEFT_EYE].z * h
            right_eye_x=lm.landmark[self.mp_pose.PoseLandmark.RIGHT_EYE].x * w- r_shldr_x
            right_eye_y=lm.landmark[self.mp_pose.PoseLandmark.RIGHT_EYE].y * h- r_shldr_y
            right_eye_z=lm.landmark[self.mp_pose.PoseLandmark.RIGHT_EYE].z * h
            left_lip_x=lm.landmark[self.mp_pose.PoseLandmark.MOUTH_LEFT].x * w- r_shldr_x
            left_lip_y=lm.landmark[self.mp_pose.PoseLandmark.MOUTH_LEFT].y * h- r_shldr_y
            left_lip_z=lm.landmark[self.mp_pose.PoseLandmark.MOUTH_LEFT].z * h
            right_lip_x=lm.landmark[self.mp_pose.PoseLandmark.MOUTH_RIGHT].x * w- r_shldr_x
            right_lip_y=lm.landmark[self.mp_pose.PoseLandmark.MOUTH_RIGHT].y * h- r_shldr_y
            right_lip_z=lm.landmark[self.mp_pose.PoseLandmark.MOUTH_RIGHT].z * h

            r_shldr_x = 0
            r_shldr_y = 0


            return np.array([nose_x, nose_y, nose_z, l_shldr_x,l_shldr_y,l_shldr_z,r_shldr_z,left_eye_x,left_eye_y,left_eye_z,right_eye_x,right_eye_y,right_eye_z,left_lip_x,left_lip_y,left_lip_z,right_lip_x,right_lip_y,right_lip_z],dtype=np.float32).reshape(1,-1)
        else:
            #print(f"No landmarks detected in image ")
            return np.array([])
    def modelLoad(self,device):
        self.model=slouch_detection()
        self.model.load_state_dict(torch.load("dontSlouch/slouch_detector.pt"))
        self.model = self.model.to(device)
        self.cur_device=device

    def slouchDetector(self,x,model):
        x=torch.tensor(x).to(self.cur_device)
        with torch.inference_mode():
            model.eval()
            ans=model(x)
        return ans.item()


    def backend(self,image,device="cpu",skeleton=True):
        
        if(image is not None):
            if(self.cur_device!=device):
                self.modelLoad(device)
            self.mp_pose = mp.solutions.pose
            

            keypoints = self.pose.process(image)
            lm = keypoints.pose_landmarks

            x = self.processImage(image,lm,True)
            if(len(x.shape)!=1):
                self.res= self.slouchDetector(x,self.model)
                color=self.red
                print(self.res)
                if(self.res>0.37):
                    self.result="not slouching"
                    color=self.green
                else:
                    self.result="slouching"
                if(skeleton):
                    self.drawPoseLandmarks(image,lm,color=color)
            return (image,self.result,x,self.res)
        else:
            raise ValueError("Webcam not found")


class slouch_detection(nn.Module):
    def __init__(self):
        super(slouch_detection,self).__init__()
        self.layers = nn.Sequential(
        nn.LayerNorm(19),
        nn.Linear(19,64),
        nn.SiLU(),
        nn.Dropout(0.2),
        nn.LayerNorm(64),
        nn.Linear(64,1),
        nn.Sigmoid()
    )
    def forward(self,x):
        return self.layers(x)