# NOTES/TO DO

* Camera_Zoom is ignored by Mode 2. Ideally you should be able to 
  increase or decrease the scale of a render in both modes without it 
  necessarily having an effect on other parameters such as Camera_Angle. 
  However, changing this behavior will break old scenes. -mjh
* Outputted Camera_Look_At is incorrect when using Mode 2 and an 
  orthographic camera. Not sure if this can be fixed. Not sure if it is 
  even possible to determine/calculate the look_at point when using 
  Mode 2 and an perspective camera. (I was able to do so when when 
  using an orthographic camera, however.) -mjh
* In my opinion, Camera_Angle and Camera_Zoom should ignore the camera's 
  aspect ratio. Rather, Camera_Aspect_Ratio should be dealt with 
  elsewhere in the code. Altering this behavior could break old scenes,
  however. -mjh
* The crosshairs in the demo scene look very strange when using an 
  orthographic camera. I have no idea why. -mjh
