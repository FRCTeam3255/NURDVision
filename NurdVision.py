import cv2
import numpy as np
import imutils

cap = cv2.VideoCapture(0)
while True:
    ret, image = cap.read()
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HLS)
    mask = cv2.inRange(hsv, (79, 46, 170), (92, 255, 255))
    output = cv2.bitwise_and(image, image, mask=mask)
    thresh = cv2.threshold(mask, 225, 255, cv2.THRESH_BINARY_INV)[1]


    (_, cnts, _) = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                    cv2.CHAIN_APPROX_SIMPLE)

    cnts = sorted(cnts, key=cv2.contourArea, reverse=True)[:10]
    cnts2 = sorted(cnts, key=cv2.contourArea, reverse=False)[:9]
    screenCnt = None
    screenCnt2 = None

    # loop over our contours
    for c in cnts:
        # approximate the contour
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, .012 * peri, True)

        # if our approximated contour has four points, then
        # we can assume that we have found our screen
        if len(approx) == 4:
            screenCnt = approx
            break
    for d in cnts2:
        # approximate the contour
        peri2 = cv2.arcLength(d, True)
        approx2 = cv2.approxPolyDP(d, .012 * peri2, True)

        # if our approximated contour has four points, then
        # we can assume that we have found our screen
        if len(approx2) == 4:
             screenCnt2 = approx
             break


    cv2.drawContours(image, [screenCnt], -1, (0, 0, 255), 10)


    print(cnts)
    cv2.imshow("3255-output", image)
    cv2.imshow("3255-mask", output)
    k=cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()