# Quadcopter_stm32_PID
Quadcopter flight controller project based on stm32f103C8T6.The goal of this project is to understand the principles of  PID controllers.
# Abstract
&nbsp; This research is the discussion and research of PID controller. Through the realization of the controller on the quadcopter, the actual influence and function of each parameter are observed, so as to be more familiar with the operation mode and principle of PID.  
&nbsp; The PID controller is a controller that is often used in the industry, mainly used to correct the error between the reference value or the ideal value. Discuss the operating principle of this controller and implement this controller on a quadcopter.  
&nbsp; A quadcopter is an aircraft with four sets of rotors. It is easy to control, can fly in a variety of postures, and can hover flexibly in the air. Although this kind of aircraft cannot have extremely high load capacity at present, it has many applications in real life. Like a common shooting in life, you can shoot angles and perspectives that people can't shoot. With a camera lens, you can enjoy the feeling of flying in the air. It is also often used in agriculture, mainly for spraying pesticides and observing the growth status of various crops.  
&nbsp; This university topic will describe the required research theories and principles, such as: PID controller theory and principles, and through additional program assistance, in order to better understand the theories and principles of various algorithms, and will see that these theories are used in in this topic. After that, I will describe the content and research process of this special research, and then show the results of the special research and list the problems and solutions encountered on the way. Considering the future of this special topic, I will continue to update the design of this special topic so that this topic can be become a more perfect topic.  
&nbsp; This topic uses the ARM controller to make a quadcopter. After setting the GPIO through STM32CubeMX, use Keil to plan the program and realize the Quadcopter, and use OrCAD and Allegro to design the PCB board to integrate the circuit of this topic, and produce a set of flight controller.  
&nbsp; After the training of this topic, you can not only learn to write and plan code, but also design a set of products by yourself. Through one-stop special production and implementation, you will gain the ability to discover and solve problems. You can also use this special study to communicate with teachers, solve problems encountered, and gain more experience and methods.  
# Theory and principle
PID Controller Theory and Principle
&nbsp; PID控制器基本工作原理為讀取感測器。再經由組成PID控制器的三種算法(比例、微分、積分)來修正誤差。PID控制器也是一種閉迴圈系統。並且PID控制器輸出為這三種算法相加之後的結果，輸入為其誤差值。若定義u(t)為控制輸出，PID演算法可以用下式表示：  

![image](https://user-images.githubusercontent.com/63340820/173550151-03a266da-7d05-40c6-9c01-24ec0528f51a.png)  

其中Kp為比例增益、Ki為積分增益、Kd為微分增益、e(t)為回授值與設定值之間的誤差、t為時間常數。在PID控制系統中，控制迴路可以分成以下三個部份:  
1. 感測器量測的結果
2. 控制器決策
3. 輸出到制動控制單元  

&nbsp; 在修正誤差的PID控制器需要三種算法(比例、微分、積分)，分別是為了消除當前誤差，平均過去誤差，透過所計算出的誤差來推算出未來所產生的誤差。在此舉一個具體的例子，今天有一溫控系統需要將設備保持在一定的溫度，透過加熱棒來保持設備溫度，可以透過電子溫度計來測量出現在溫度Tn。控制器輸入所設定需要到達並保持的溫度當作參考值T，在此假設為80度。當我們現在溫度Tn與參考溫度T產生誤差時，計算出誤差後經過PID計算出修正值，再經由控制器判斷加溫棒需要減少熱量的提供或是增加熱量的提供。提供熱量會使設備溫度上升，降低熱量會使設備溫度下降，以此達到穩定控制溫度的效果。
此系統在設定完成達成穩定的結果時會非常好用，但數值輸入不當導致輸出數值反覆震盪無法達到正常預設值。所以都必須經過一定時間的調校才可以達到理想中的狀態。
PID控制器的調整，我們可以透過EXCEL進行圖形化分析。透過圖形化分析可以更加了解如何調整PID個參數增益，也可以了解到各參數過大或過小所產生的影響，以利我們在未來可以調整出更加穩定的系統。
  
**比例控制項Kp:**  
比例控制Kp為考慮當下誤差，當系統數值與預設數值誤差為0時，這項的輸出為0。若比例過大時會導致系統不穩定震盪過大。反之當比例過小時，有干擾或誤差會因為補償不夠大導致無法及時修正誤差。
如下圖所示，此系統初始值為25，離目標值100存在75的誤差。橘色線條為Kp增益大小適中的結果，不僅可以快速修正誤差，也達到沒有過多震盪的結果。藍色線條為Kp增益過小結果，導致系統修正誤差緩慢。灰色線條為Kp增益過大的結果，導致系統震盪不穩定，更大的話會導致無法修正誤差。
![image](https://user-images.githubusercontent.com/63340820/173550508-f57fc39c-fdd1-428c-9a40-ae8636ff5dc3.png)  
**積分控制項Ki:**  
積分控制Ki為考慮過去所累積的誤差，將過去一段時間的誤差和乘上Ki。當系統只有一個比例系統時，會產生震盪，因為系統無法做額外糾正，此時加上一個”負”的平均值誤差，系統上的誤差就會慢慢減小並使他穩定下來。
積分系統會加速趨近預設值並且消除誤差，當積分增益過大，趨近速度過快造成補償過頭問題反而導致遠離預設值，過小跟比例增益一樣導致修正速度緩慢。
如圖所示，橘色為Ki增益適中的結果，沒有過多的震盪與過慢的修正。藍色線條為Ki增益適中過小的結果，修正緩慢提升。灰色線條為Ki增益過大的結果，出現明顯的震盪。在此系統可以發現，Ki對本系統影響不大，有點類似輔助的效果，因為Ki增益是隨著時間慢慢累積。若數值太大會導致震盪非常巨大，所以在本系統Ki增益本身就沒有很大。以此狀況可知，I與P的情況有些許類似，P對系統來說是粗調，I可以算是微調，主要存在目的為消除穩態誤差。
 ![image](https://user-images.githubusercontent.com/63340820/173550588-d9bfe882-4ac5-46bd-8228-88e91cd540cc.png)  
**微分控制項Kd:**  
微分控制Kd為考慮未來產生得誤差。這就是因為PID這系統稱作可預測控制器原因，此微分控制對需要快速做出修正的系統有很大的幫助，可以增加系統穩定性，但許多較緩慢的系統會省略這個控制項。但雖然可以增加穩定性也不宜過大，會導致系統修正時間越長。
如圖(四)所示，橘色曲線為Kd增益適中的結果，有效的解決修正過衝的問題。藍色曲線為Kd增益過小的結果，過小導致系統過度修正，花費更多時間來達到參考值。灰色曲線為Kd增益過大的結果，過大削減太過於嚴重，導致系統修正十分緩慢。從圖片所示，當系統離參考值很遠時卻發生震盪問題，而不是在參考值上下震盪，代表Kd增益過大需要小一點。
 ![image](https://user-images.githubusercontent.com/63340820/173550637-b223dcac-dd63-4698-a295-7828a1cf253b.png)  
表為各增益對上升時間、安定時間和穩定性的影響。根據以上的資料，與經過EXCEL圖形化分析後所得出的表[1]，從此可知個增益對這種狀態的影響，方便日後調整。圖(五)為PID控制器架構圖，Reciver為遙控接收器，接收到參考值後輸入到控制器之中，並經過PID計算。之後將計算出之結果經判斷後，將結果輸出到ESC馬達控制器來控制馬達，影響姿態後經由MPU6050陀螺儀讀取現在姿態後再回傳到控制器中與參考值進行比對計算。
![image](https://user-images.githubusercontent.com/63340820/173550799-88655f4d-0950-4863-ad00-ab5a4242c7c4.png)  
![image](https://user-images.githubusercontent.com/63340820/173550848-0d764636-d4c8-4630-b7e0-faa017166d42.png)  
