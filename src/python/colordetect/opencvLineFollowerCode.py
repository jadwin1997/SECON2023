import cv2
import numpy as np
import time
import serial
from PIL import Image, ImageColor
im = Image.new('1', (352,240)) # create the Image of size 1 pixel 
#mapping function used to map our resolution parameters to smaller range (so arduino doesnt have to)
def _map(x, in_min, in_max, out_min, out_max):
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

#Start serial connection with arduino. COM port may change with each board!! look in arduino IDE to find correct COM port. 
arduino = serial.Serial(port='COM6', baudrate=9600, timeout=0.1)

#capture video through camera 0. (I found CAP_DSHOW sped up opening the camera on my windows computer)
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW) #alternatively, cv2.VideoCapture(0)


#Check whether user selected camera is opened successfully. Try again with 1 second delay if not. 
while(not(cap.isOpened())):

    print('Could not open video device')
    print('Trying again..')
    cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    time.sleep(1)


#Set the resolution to 720P. Lowering this may increase framerate.
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 352)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

#Set upper and lower bounds for HSV mask to find contours to track. Refer to HSV color chart to get correct color (THIS IS NOT RGB)
lower = [100, 100, 100] #[25,50,0] lower bound for yellow
upper = [180, 255, 255]#[40,255,255] upper bound for yellow

# create NumPy arrays from the color boundaries given
lower = np.array(lower, dtype="uint8")
upper = np.array(upper, dtype="uint8")

def compute_mass_center(contour):
    moments = cv2.moments(contour)
    m00 = moments['m00']
    if m00 == 0:
        return -1, -1
    m01 = moments['m01']
    m10 = moments['m10']
    x_m = m10/m00
    y_m = m01/m00
    return x_m, y_m

while(True):
    
    #get camera frame
    ret, frame = cap.read()

    #apply a gaussian blur to the frame. Im not sure how much this helps. 
    blurry = cv2.GaussianBlur(frame, (7,7),0)

    #break if there is no frame. Maybe put arduino into failsafe here?? 
    if frame is None:
        break
    
    # find the colors within the specified boundaries and apply
    # the mask
    mask = cv2.inRange(cv2.cvtColor(blurry,cv2.COLOR_BGR2HSV), lower, upper)
    
    #find the contours with the mask we created
    contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    #Check if there is more than 1 contour. May be problematic bc there rarely is 1 contour. 
    if(len(contours)>1):
        #Find the largest contour
        biggest_contour = max(contours,key = cv2.contourArea)
        cnt = biggest_contour
        #set a bounding box around the largest contour and store it's x,y,w,h position
        x,y,w,h = cv2.boundingRect(cnt)
        x1 = compute_mass_center(cnt)
        #add rectange to the frame
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 0, 255), 2)
        cv2.rectangle(frame, (int(x1[0]), int(x1[1])), (int(x1[0])+2, int(x1[1])+2), (0, 0, 255), 2)
        received=0

        #Check if bounding box area is above 1000 pixels. needs experimenting. 
        if(w*h>1000):
            
                #create data to send. I found that sending the data with letters between integers was easy.
                # For example, "X20Y50W30" This will be explained in the arduino code more.
                data = 'x'+str(_map(x,0,352,0,62))#Just using x coordinate right now. For this purpose, I had 62 LEDs I was controlling on arduino.

                #Write data to arduino
                arduino.write(data.encode('utf-8'))#STRING NEEDS TO BE ENCODED BEFORE SENDING

                #Print the coordinates of the bounding box, and whatever we read back from arduino
                print("x,y,w,h,r:",_map(x1[0],0,1280,0,62),360-y,w,h,arduino.readline())

                #create the final frame to display
                #output = cv2.drawContours(frame, contours, -1, (0, 0, 255),3)
                output = frame
        else:
            #create the frame without contour if none is big enough
            output = frame
    else:
        #draw the single contour if that case is satisfied. (rarely is)
        output = cv2.drawContours(frame, contours, -1, (0, 0, 255),3)

    #Finally, show the camera output with our wonder contours/bounding box
    cv2.imshow('preview',output)
    #CAN CONTROL ROBOT WITH THIS (ord' (wasd keys)')
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


# After the loop release the cap object
cap.release()
# Destroy all the windows
cv2.destroyAllWindows()


