import cv2
import numpy as np

def main():
    cap = cv2.VideoCapture(0)
    lower_blue = np.array([110,50,50])
    upper_blue = np.array([130,255,255])

    #yellow : 60, 100, 90
    lower_yellow = np.array([20, 30, 30])
    upper_yellow = np.array([30, 255, 255])

    detectColor(cap, lower_yellow, upper_yellow)



def detectColor(cap: cv2.VideoCapture, lower: np.ndarray, upper: np.ndarray):
    while(True):
        _, frm = cap.read()

        hsv_frm = cv2.cvtColor(frm, cv2.COLOR_BGR2HSV)

        mask = cv2.inRange(hsv_frm, lower, upper)

        res = cv2.bitwise_and(frm, mask, mask=mask)

        #cv2.imshow("frame", frm)
        #cv2.imshow("mask", mask)
        cv2.imshow("res", res)

        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break
    
    cv2.destroyAllWindows()
    cap.release()


main()