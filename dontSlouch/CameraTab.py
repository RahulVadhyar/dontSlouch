from tkinter import *
from tkinter import ttk
import cv2
from PIL import Image, ImageTk 
from notifypy import Notify

class CameraTab:
    def __init__(self, notebook, backend, device):
        self.cameraFrame = ttk.Frame(notebook)
        self.cameraLabel = ttk.Label(self.cameraFrame, text = 'Camera', font= ('Arial', 40))
        self.cameraLabel.pack(padx=10, pady=10, anchor = 'w')
        self.label_widget = ttk.Label(self.cameraFrame)
        self.label_widget.pack()
        self.slouching_label = ttk.Label(self.cameraFrame)
        self.slouching_label.pack()
        self.showSkeletonButton  = ttk.Button(self.cameraFrame, text = 'Show Skeleton', command = self.showSkeleton)
        self.showSkeletonButton.pack()
        self.showSkeleton = True
        self.notificationCounter = 10
        
        self.device = device
        self.webcam = cv2.VideoCapture(0)
        self.width, self.height = 800, 600
        self.webcam.set(cv2.CAP_PROP_FRAME_WIDTH, self.width)
        self.webcam.set(cv2.CAP_PROP_FRAME_HEIGHT, self.height)
        self.backend = backend
        self.openCamera()
        self.result = ""
        self.modelValue = 0
        self.position = []
    
    def showSkeleton(self):
        self.showSkeleton = not self.showSkeleton
    
    def openCamera(self):
        _, frame = self.webcam.read()
        frame, result, x, res = self.backend.backend(frame, device = self.device.get(), skeleton = self.showSkeleton)
        self.result = result
        self.modelValue = x
        self.position = res
        
        opencv_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)
        captured_image = Image.fromarray(opencv_image)
        photo_image = ImageTk.PhotoImage(image=captured_image)
        
        self.label_widget.photo_image = photo_image
        self.label_widget.configure(image=photo_image)
        self.label_widget.after(10, self.openCamera)
        
        if self.result == "slouching":
            self.slouching_label.config(text='You are slouching at this position')
        else:
            self.slouching_label.config(text='Congrats! You are not slouching')
        
        if self.result == "slouching" and self.notificationCounter == 0:
            self.notificationCounter = 10
            notification = Notify()
            notification.title = "Dont Slouch!"
            notification.message = "You are slouching! Please correct your posture."
            notification.send()
            print("inside")
        elif self.result == "slouching":
            self.notificationCounter -= 1