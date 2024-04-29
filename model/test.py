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
mp_pose = mp.solutions.pose

def findDistance(x1, y1, x2, y2):
    dist = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
    return dist

def findAngle(x1, y1, x2, y2):
    if (x2 - x1) == 0 or y1 == 0:
        return 0
    theta = math.acos((y2 - y1) * (-y1) / (math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2) * y1))
    degree = int(180 / math.pi) * theta
    return degree

def process_image(image, pose):
    if image is None:
        print(f"Failed to read image")
        return

    h, w = image.shape[:2]
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    keypoints = pose.process(image_rgb)
    lm = keypoints.pose_landmarks

    if lm is not None:
        nose_x = lm.landmark[mp_pose.PoseLandmark.NOSE].x * w
        nose_y = lm.landmark[mp_pose.PoseLandmark.NOSE].y * h
        nose_z = lm.landmark[mp_pose.PoseLandmark.NOSE].z * h  # Calculating Z-axis position

        l_shldr_x = lm.landmark[mp_pose.PoseLandmark.LEFT_SHOULDER].x * w
        l_shldr_y = lm.landmark[mp_pose.PoseLandmark.LEFT_SHOULDER].y * h
        l_shldr_z = lm.landmark[mp_pose.PoseLandmark.LEFT_SHOULDER].z * h
        r_shldr_x = lm.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER].x * w
        r_shldr_y = lm.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER].y * h
        r_shldr_z=lm.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER].z * h

        # Write posture data to CSV file
        return np.array([nose_x, nose_y, nose_z, l_shldr_x,l_shldr_y,l_shldr_z,r_shldr_x,r_shldr_y,r_shldr_z],dtype=np.float32).reshape(1,-1)
    else:
        print(f"No landmarks detected in image ")

class slouch_detection(nn.Module):
    def __init__(self):
        super(slouch_detection,self).__init__()
        self.layers = nn.Sequential(
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
    
def slouch_detector(x):
    scaler=load(open('scaler.pkl', 'rb'))
    x=scaler.fit_transform(x)
    x=torch.tensor(x)
    model=slouch_detection()
    model.load_state_dict(torch.load("slouch_detector.pt"))
    model = model.to('cpu')
    ans=model(x)
    print(ans.item())
    return ans.item()

font = cv2.FONT_HERSHEY_SIMPLEX
blue = (255, 127, 0)
red = (50, 50, 255)
green = (127, 255, 0)
dark_blue = (127, 20, 0)
light_green = (127, 233, 100)
yellow = (0, 255, 255)
pink = (255, 0, 255)

def main():
    # Initialize the webcam
    cap = cv2.VideoCapture(0) # this is the magic!

    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
    
    # Check if the webcam is opened correctly
    if not cap.isOpened():
        print("Error: Unable to access the webcam.")
        return
    
    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()
        
        # Check if the frame is captured correctly
        if not ret:
            print("Error: Unable to capture frame.")
            break
        
        pose=mp_pose.Pose(min_detection_confidence=0.6,min_tracking_confidence=0.6)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        keypoints = pose.process(frame)
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        fps = cap.get(cv2.CAP_PROP_FPS)
        h, w = frame.shape[:2]
        
        lm = keypoints.pose_landmarks
        lmPose = mp_pose.PoseLandmark
        l_shldr_x = int(lm.landmark[lmPose.LEFT_SHOULDER].x * w)
        l_shldr_y = int(lm.landmark[lmPose.LEFT_SHOULDER].y * h)
        r_shldr_x = int(lm.landmark[lmPose.RIGHT_SHOULDER].x * w)
        r_shldr_y = int(lm.landmark[lmPose.RIGHT_SHOULDER].y * h)
        l_ear_x = int(lm.landmark[lmPose.LEFT_EAR].x * w)
        l_ear_y = int(lm.landmark[lmPose.LEFT_EAR].y * h)
        l_hip_x = int(lm.landmark[lmPose.LEFT_HIP].x * w)
        l_hip_y = int(lm.landmark[lmPose.LEFT_HIP].y * h)
        offset = findDistance(l_shldr_x, l_shldr_y, r_shldr_x, r_shldr_y)
        if offset < 100:
            cv2.putText(frame, str(int(offset)) + ' Aligned', (w - 150, 30), font, 0.9, green, 2)
        else:
            cv2.putText(frame, str(int(offset)) + ' Not Aligned', (w - 150, 30), font, 0.9, red, 2)
        neck_inclination = findAngle(l_shldr_x, l_shldr_y, l_ear_x, l_ear_y)
        torso_inclination = findAngle(l_hip_x, l_hip_y, l_shldr_x, l_shldr_y)
        cv2.circle(frame, (l_shldr_x, l_shldr_y), 7, yellow, -1)
        cv2.circle(frame, (l_ear_x, l_ear_y), 7, yellow, -1)
        cv2.circle(frame, (l_shldr_x, l_shldr_y - 100), 7, yellow, -1)
        cv2.circle(frame, (r_shldr_x, r_shldr_y), 7, pink, -1)
        cv2.circle(frame, (l_hip_x, l_hip_y), 7, yellow, -1)
        cv2.circle(frame, (l_hip_x, l_hip_y - 100), 7, yellow, -1)
        
        
        cv2.line(frame, (l_shldr_x, l_shldr_y), (l_ear_x, l_ear_y), green, 4)
        cv2.line(frame, (l_shldr_x, l_shldr_y), (l_shldr_x, l_shldr_y - 100), green, 4)
        cv2.line(frame, (l_hip_x, l_hip_y), (l_shldr_x, l_shldr_y), green, 4)
        cv2.line(frame, (l_hip_x, l_hip_y), (l_hip_x, l_hip_y - 100), green, 4)


        x = process_image(frame,pose)
        res= slouch_detector(x)
        # Display the captured frame
        cv2.imshow('Webcam', frame)
        
        # Break the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    # Release the webcam and close all OpenCV windows
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()