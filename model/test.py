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

def process_image(image, pose,draw_skeleton=False):
    if image is None:
        print(f"Failed to read image")
        return
    h, w = image.shape[:2]
    #image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    keypoints = pose.process(image)
    lm = keypoints.pose_landmarks

    if(draw_skeleton):
        if nose_x and nose_y:
            cv2.circle(image, (int(nose_x), int(nose_y)), 7, pink, -1)
        if l_shldr_x and l_shldr_y:
            cv2.circle(image, (int(l_shldr_x), int(l_shldr_y)), 7, red, -1)
        if r_shldr_x and r_shldr_y:
            cv2.circle(image, (int(r_shldr_x), int(r_shldr_y)), 7, red, -1)

        # Draw lines between shoulders and nose (assuming you have detection logic)
        if l_shldr_x and l_shldr_y and r_shldr_x and r_shldr_y and nose_x and nose_y:
            cv2.line(image, (int(l_shldr_x), int(l_shldr_y)), (int(r_shldr_x), int(r_shldr_y)), green, 2)
            cv2.line(image, (int(l_shldr_x), int(l_shldr_y)), (int(nose_x), int(nose_y)), green, 2)
            cv2.line(image, (int(r_shldr_x), int(r_shldr_y)), (int(nose_x), int(nose_y)), green, 2)

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
        if(not draw_skeleton):
            return np.array([nose_x, nose_y, nose_z, l_shldr_x,l_shldr_y,l_shldr_z,r_shldr_x,r_shldr_y,r_shldr_z],dtype=np.float32).reshape(1,-1)
        else:
            return image,np.array([nose_x, nose_y, nose_z, l_shldr_x,l_shldr_y,l_shldr_z,r_shldr_x,r_shldr_y,r_shldr_z],dtype=np.float32).reshape(1,-1)
    else:
        print(f"No landmarks detected in image ")

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
    
def slouch_detector(x):
    """  scaler=load(open('model/scaler.pkl', 'rb'))
    x=scaler.fit_transform(x) """
    x=torch.tensor(x)
    model=slouch_detection()
    model.load_state_dict(torch.load("model/slouch_detector.pt"))
    model = model.to('cpu')
    with torch.inference_mode():
        model.eval()
        ans=model(x)
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
    pose=mp_pose.Pose(min_detection_confidence=0.8,min_tracking_confidence=0.8)
    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()
        
        # Check if the frame is captured correctly
        if not ret:
            print("Error: Unable to capture frame.")
            break
        
        
        """ lm = keypoints.pose_landmarks
        lmPose = mp_pose.PoseLandmark
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
        offset = findDistance(l_shldr_x, l_shldr_y, r_shldr_x, r_shldr_y)
        if offset < 100:
            cv2.putText(frame, str(int(offset)) + ' Aligned', (w - 150, 30), font, 0.9, green, 2)
        else:
            cv2.putText(frame, str(int(offset)) + ' Not Aligned', (w - 150, 30), font, 0.9, red, 2) """
        #neck_inclination = findAngle(l_shldr_x, l_shldr_y, nose_x, nose_y)
        #torso_inclination = findAngle(l_hip_x, l_hip_y, l_shldr_x, l_shldr_y)

        """ cv2.circle(frame, (l_shldr_x, l_shldr_y - 100), 7, yellow, -1)
        cv2.circle(frame, (r_shldr_x, r_shldr_y), 7, pink, -1) """
        
        
        

        #cv2.line(frame, (l_shldr_x, l_shldr_y), (r_shldr_x, r_shldr_y), green, 4)

        # Draw circles on detected keypoints (assuming you have detection logic)
        


        x = process_image(frame,pose)
        if(x is not None):
            #time.sleep(0.1)
            res= slouch_detector(x)
            #print(round(res,3))
            if(res>0.3):
                print("not slouching")
            else:
                print("slouching")
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