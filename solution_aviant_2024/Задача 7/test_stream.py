import cv2
import subprocess
import time

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print("GO script RTP stream testing")
    #subprocess.Popen(["go", "run", "./main.go"])
    #time.sleep(5)
    cap = cv2.VideoCapture("rtp-forwarder.sdp")
    while(True):
        ret, frame = cap.read()
        if ret:
            #print(frame.size)
            cv2.imshow("test_image", frame)
            cv2.waitKey(10)

