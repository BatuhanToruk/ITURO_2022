import cv2
import time
import numpy as np
from picamera.array import PiRGBArray
from picamera import PiCamera
import RPi.GPIO as GPIO
import time



GPIO.setmode(GPIO.BCM)

GPIO.setwarnings(False)
onlar=0
birler=0
ls=[0,0,0,0,0]
hata=0
hata0=0

tabelaKod={1:15,3:-12,2:12,11:-13,12:-14,21:13,22:14}

setpoint=262
led0 = 4
led1 = 18
led2 = 27
led3 = 23
led4 = 24
datapin=25

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(640, 480))
time.sleep(0.1)


data=0
GPIO.setup(led0, GPIO.OUT)
GPIO.setup(led1, GPIO.OUT)
GPIO.setup(led2, GPIO.OUT)
GPIO.setup(led3, GPIO.OUT)
GPIO.setup(led4, GPIO.OUT)
GPIO.setup(datapin, GPIO.IN)
# Turn the GPIO pin on
def cevir(sayi):
    if (sayi<0):
        ls[4]=1
    else:
        ls[4]=0
    GPIO.output(led4,ls[4])
    sayi=abs(sayi)
    ls[3]=sayi//8
    GPIO.output(led3,ls[3])
    sayi=(sayi-ls[3]*8)
    ls[2]=sayi//4
    GPIO.output(led2,ls[2])
    sayi=(sayi-ls[2]*4)
    ls[1]=sayi//2
    GPIO.output(led1,ls[1])
    sayi=(sayi-ls[1]*2)
    ls[0]=sayi%2
    GPIO.output(led0,ls[0])
    
def tab_oku():
    global onlar
    global birler
    
    camera.capture(rawCapture, format="bgr")
    image = rawCapture.array
    rawCapture.truncate(0)

    img2=image.copy()
#     cv2.imshow("image", image)
#     cv2.waitKey(0)
#     cv2.destroyWindow("image")
    
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    ret, thresh1 = cv2.threshold(gray, 90, 255, cv2.THRESH_OTSU | cv2.THRESH_BINARY)
#     cv2.imshow("thresh1", thresh1)
#     cv2.waitKey(0)
#     cv2.destroyWindow("thresh1")
    
    #circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1,20 )
    
    rows = gray.shape[0]
    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1, 100,param1=1, param2=100, minRadius=40, maxRadius=90)
    #print(circles)

    if not circles is None:
        circles = np.uint16(np.around(circles[0, :]))
        for (x, y, r) in circles:
            parca =  thresh1[y-(r):y+(r), x-(r):x+(r)]
            sonimg= image[y-(r):y+(r), x-(r):x+(r)]
            dar=r//3
            cv2.circle(img2, (x, y), (r-dar), (0, 255, 0), 2)
#             cv2.imshow("circle", img2)
#             cv2.waitKey(0)
#             cv2.destroyWindow("circle")
            mask = np.zeros(parca.shape[:2], dtype="uint8")                   
            cv2.circle(mask, (parca.shape[0]//2, parca.shape[1]//2), (r-dar), 255, -1)
            ret, thresh2 = cv2.threshold(parca, 0, 255, cv2.THRESH_OTSU | cv2.THRESH_BINARY_INV)          
            masked = cv2.bitwise_and(thresh2, thresh2, mask=mask)
#             cv2.imshow("masked1", masked)
#             cv2.waitKey(0)
# 
#             cv2.destroyWindow("masked1")

            cnts, hierarchy = cv2.findContours(masked, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            hullImage = np.zeros(masked.shape[:2], dtype="uint8")
           # print (len(cnts))
            
        for (i, c) in enumerate(cnts):         
            area = cv2.contourArea(c)
            (x, y, w, h) = cv2.boundingRect(c)
            if(h==0):
                h=1
            if(w==0):
                w=1

            aspectRatio = w / float(h)
            extent = area / float(w * h)
            hull = cv2.convexHull(c)
            hullArea = cv2.contourArea(hull)
            if(hullArea==0):
                hullArea=1
            solidity = area / float(hullArea)
            cv2.drawContours(hullImage, [hull], -1, 255, -1)
            cv2.drawContours(sonimg, [c], -1, (240, 0, 159), 3)
#             cv2.imshow("cntr", sonimg)
#             cv2.waitKey(0)
#             cv2.destroyWindow("cntr")
            time.sleep(1)
            shape = ""

            if aspectRatio >= 1.5 or solidity <= 0.55 :
                yarim = True
            else:
                yarim = False

            if aspectRatio >= 0.52 and aspectRatio <= 0.7 and solidity >= 0.65 and solidity <= 0.75:
                shape = "Yukari OK"
                birler=1
                onlar=0

                print(shape)

     
            elif solidity > 0.90 and aspectRatio < 1 and extent < 0.7:
                shape = "A"
                onlar=10
                
            elif aspectRatio > 0.8 and extent < 0.5:
                shape = ""
        
            elif solidity > 0.90 and aspectRatio < 1 and extent > 0.7:
                shape = "B"
                onlar=20
            elif aspectRatio > 0.95 and aspectRatio < 1.3:
                shape = ""
               
            if yarim == True:
                left =  masked[0:, 0:x + int(w / 2)]
                right = masked[0:, x + int(w / 2):]


                rightCnts, hierarchy = cv2.findContours(right.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
                leftCnts, hierarchy  = cv2.findContours(left.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

                areaRight = cv2.contourArea(rightCnts[0])
                areaLeft = cv2.contourArea(leftCnts[0])
                (x1, y1, w1, h1) = cv2.boundingRect(leftCnts[0])
                (x2, y2, w2, h2) = cv2.boundingRect(rightCnts[0])
                if(h==0):
                    h=1
                if(h1==0):
                    h1=1
                if(h2==0):
                    h2=1
                if(w1==0):
                    w1=1
                if(w2==0):
                    w2=1
                aspectRatioLeft = w1 / float(h)
                aspectRatioRight = w2 / float(h)       
                extentLeft = areaLeft / float(w1 * h1)
                extentRight = areaRight / float(w2 * h2)    
                hullLeft = cv2.convexHull(leftCnts[0])
                hullRight = cv2.convexHull(rightCnts[0])
                hullAreaLeft = cv2.contourArea(hullLeft)
                hullAreaRight = cv2.contourArea(hullRight)
                if(hullAreaLeft==0):
                    hullAreaLeft=1
                if(hullAreaRight==0):
                    hullAreaRight=1  
                solidityLeft = area / float(hullAreaLeft)
                solidityRight = area / float(hullAreaRight)
                print("Contour Left #{} -- aspect_ratio={:.2f}, extent={:.2f}, solidity={:.2f}"
                      .format(i + 1, aspectRatioLeft, extentLeft, solidityLeft))

                print("Contour Right #{} -- aspect_ratio={:.2f}, extent={:.2f}, solidity={:.2f}"
                      .format(i + 1, aspectRatioRight, extentRight, solidityRight))
                if (solidityLeft > solidityRight and aspectRatio>1.5):
                    shape = "Sag OK"
                    birler=2
                    onlar=0
                    print(shape)

                elif (solidityLeft < solidityRight and aspectRatio>1.5):
                    shape = "Sol OK"
                    birler=3
                    onlar=0
                    print(shape)
                
                elif solidityLeft < 1:
                    shape = "1"
                    birler=1
        
                elif solidityRight>3:
                    shape = "2"
                    birler=2

                yarim = False


            cv2.putText(sonimg, shape, (x, y - 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (240, 240, 0), 2)
            print("Contour #{} -- aspect_ratio={:.2f}, extent={:.2f}, solidity={:.2f}".format(i + 1, aspectRatio, extent, solidity))        
        toplam=onlar+birler
        cevir(0)
        time.sleep(1)
        print(toplam)
        print(tabelaKod[toplam])
        cevir(tabelaKod[toplam])
        
        tabelaSens = GPIO.input(datapin)
        while(tabelaSens==1):
            tabelaSens = GPIO.input(datapin)
            print("datapini 0 bekliyor")
            time.sleep(0.2)
    else:
       print("kirmizi isk")
       time.sleep(9)
       cevir(0)
       time.sleep(1)
       cevir(0)
  
     

   
def cizgi_takip():
    global hata0
    global cx
    global cy
    x_last = 320
    y_last= 180

    roi = img[260:440,100:625]
   # viraj=img[0:200,300:630]
    roi2=roi.copy()
    low_b=np.uint8([70,70,70])
    high_b=np.uint8([0,0,0])
    mask=cv2.inRange(roi,high_b,low_b)
    k=-10
    contours_blk , hierarchy_blk = cv2.findContours(mask,1,cv2.CHAIN_APPROX_NONE)
    for i in range(25,526,25):
        
        cv2.line(roi2,(i,0),(i,480),(255,0,0),1)
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(roi2,str(k),(i-23,50), font, 0.3,(0,0,255),1,cv2.LINE_AA)
        k+=1

    if len(contours_blk) > 0:
        hata=0
        c=max(contours_blk,key=cv2.contourArea)
        M=cv2.moments(c)
        if M["m00"]!=0:
            cx=int(M['m10']/M['m00'])
            cy=int(M['m01']/M['m00'])
            #print("CX:"+str(cx)+" CY:"+str(cy))
            cv2.circle(roi2,(cx,cy),5,(255,0,0),-1)
        cv2.drawContours(roi2 , c,-1,(0,255,0),2)
        hata=(cx-setpoint)//25
        hata0=hata
    else:
        if(hata0>0):
            hata=11
        else:
            hata=-11
        
    
    if(hata<-11):
        hata=-11
    elif (hata>11):
        hata=11
    cevir(hata)
    #cevir(-15)
    print("hata:",hata)
    cv2.putText(roi2,str(hata),(260,150), font, 1,(255,0,255),2,cv2.LINE_AA)
    #ser.write(bytes('{}'.format(hata), 'utf-8'))
    rawCapture.truncate(0)
    #ser.write(bytes('{}'.format(hata), 'utf-8'))
    cv2.imshow('roi2',roi2)
   # cv2.imshow('viraj',viraj)
    #time.sleep(5)

   
    


for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    tabelaSens = GPIO.input(datapin)

    rawCapture.truncate(0)
    
    
    #tabelaSens =1
    if(tabelaSens):
        cevir(-15)
        print("tabela okunuyor")
        time.sleep(2)
        

        tab_oku()
    else:
        frame = frame.array
        img=frame
        cizgi_takip()
        
   
        
    if cv2.waitKey(1) & 0xFF == ord('q'):
        cevir(-15)
        break
cv2.destroyAllWindows()




       