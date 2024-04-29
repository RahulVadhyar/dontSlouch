import os
import cv2
import math
import mediapipe as mp
import csv
from sklearn.preprocessing import MinMaxScaler
import torch
from torch import nn
import numpy as np
from pickle import load
import time

class Backend:
    
    def __init__(self,device="cpu"):
        self.model=slouch_detection()
        self.model.load_state_dict(torch.load("../model/slouch_detector.pt"))
        self.model = self.model.to(device)
        self.cur_device=device
        self.mp_pose = mp.solutions.pose
        self.pose=self.mp_pose.Pose(min_detection_confidence=0.8,min_tracking_confidence=0.8)
        font = cv2.FONT_HERSHEY_SIMPLEX
        self.red = (50, 50, 255)
        self.green = (127, 255, 0)
        self.result=""
        self.res=-1
        self.x=0

    def draw_pose_landmarks(self,image, lm, color=(255, 0, 0), thickness=2):

        h, w = image.shape[:2]
        nose_x = lm.landmark[self.mp_pose.PoseLandmark.NOSE].x * w #0
        nose_y = lm.landmark[self.mp_pose.PoseLandmark.NOSE].y * h #1
        nose_z = lm.landmark[self.mp_pose.PoseLandmark.NOSE].z * h #2 # Calculating Z-axis position

        l_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].x * w #3
        l_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].y * h #4
        l_shldr_z = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].z * h #5
        r_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].x * w #6
        r_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].y * h #7
        r_shldr_z=lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].z * h #8


        if nose_x and nose_y:
            cv2.circle(image, (int(nose_x), int(nose_y)), 7, color, -1)
        if l_shldr_x and l_shldr_y:
            cv2.circle(image, (int(l_shldr_x), int(l_shldr_y)), 7, color, -1)
        if r_shldr_x and r_shldr_y:
            cv2.circle(image, (int(r_shldr_x), int(r_shldr_y)), 7, color, -1)

        # Draw lines between shoulders and nose (assuming you have detection logic)
        if l_shldr_x and l_shldr_y and r_shldr_x and r_shldr_y and nose_x and nose_y:
            cv2.line(image, (int(l_shldr_x), int(l_shldr_y)), (int(r_shldr_x), int(r_shldr_y)), color, 2)
            cv2.line(image, (int((l_shldr_x+r_shldr_x)/2), int((l_shldr_y+r_shldr_y)/2)), (int(nose_x), int(nose_y)), color, 2)
        return image

    def process_image(self,image, lm,draw_skeleton=False):
        if image is None:
            print(f"Failed to read image")
            return
        h, w = image.shape[:2]

        if lm is not None:
            nose_x = lm.landmark[self.mp_pose.PoseLandmark.NOSE].x * w #0
            nose_y = lm.landmark[self.mp_pose.PoseLandmark.NOSE].y * h #1
            nose_z = lm.landmark[self.mp_pose.PoseLandmark.NOSE].z * h #2 # Calculating Z-axis position

            l_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].x * w #3
            l_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].y * h #4
            l_shldr_z = lm.landmark[self.mp_pose.PoseLandmark.LEFT_SHOULDER].z * h #5
            r_shldr_x = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].x * w #6
            r_shldr_y = lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].y * h #7
            r_shldr_z=lm.landmark[self.mp_pose.PoseLandmark.RIGHT_SHOULDER].z * h #8
            # Write posture data to CSV file
            return np.array([nose_x, nose_y, nose_z, l_shldr_x,l_shldr_y,l_shldr_z,r_shldr_x,r_shldr_y,r_shldr_z],dtype=np.float32).reshape(1,-1)
        else:
            print(f"No landmarks detected in image ")
            return np.array([])
    def model_load(self,device):
        self.model=slouch_detection()
        self.model.load_state_dict(torch.load("model/slouch_detector.pt"))
        self.model = self.model.to(device)
        self.cur_device=device

    def slouch_detector(self,x,model):
        x=torch.tensor(x)
        with torch.inference_mode():
            model.eval()
            ans=model(x)
        return ans.item()


    def backend(self,image,device="cpu",skeleton=True):
        
        if(image is not None):
            if(self.cur_device!=device):
                model = self.model_load(device)
            self.mp_pose = mp.solutions.pose
            

            keypoints = self.pose.process(image)
            lm = keypoints.pose_landmarks

            x = self.process_image(image,lm,True)
            if(len(x.shape)!=1):
                self.res= self.slouch_detector(x,self.model)
                color=self.red
                if(self.res>0.3):
                    self.result="not slouching"
                    color=self.green
                else:
                    self.result="slouching"
                if(skeleton):
                    self.draw_pose_landmarks(image,lm,color=color)
        return (image,self.result,x,self.res)


class slouch_detection(nn.Module):
    def __init__(self):
        super(slouch_detection,self).__init__()
        self.layers = nn.Sequential(
            nn.LayerNorm(9),
            nn.Linear(9,36),
            nn.ReLU(),
            nn.Linear(36,15),
            nn.ReLU(),
            nn.Dropout(0.2),
            nn.Linear(15,12),
            nn.ReLU(),
            nn.Linear(12,6),
            nn.ReLU(),
            nn.Linear(6,3),
            nn.Dropout(0.2),
            nn.ReLU(),
            nn.Linear(3,1),
            nn.Sigmoid()
        )
    def forward(self,x):
        return self.layers(x)