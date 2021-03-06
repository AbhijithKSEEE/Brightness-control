#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

#define LEDn    12
#define LEDpwm   5

const int output = 0;
String sliderValue = "0";
const char* PARAM_INPUT = "value";
const int no = 1024;
int led = 5;         
const int brightness = 0;    
int fadeAmount = 5; 

String data;





const char* ssid = "ledTest";;
const char* password = "12341234";


ESP8266WebServer server(80);

String webpage = R"***(
<!DOCTYPE html>
<html>
  <head>    
    <title>SWITCH LED</title>
  </head>
<body>
  <div style="background:#FFFFFF; width:60px; height:40px">
    <div style="background:#FFFFFF; width:100%; height:10%">
      <h5 align="center"><br/>ON</h5>
    </div>
    <div id="LEDn" style="background:#77FF77; width:100%; height:100%"></div> 
  </div>
  <div style="background:#FFFFFF; width:60px; height:40px">
    <div style="background:#FFFFFF; width:100%; height:10%">
      <h5 align="center"><br/>up</h5>
    </div>
    <div id="pwmplus" style="background:#00EE00; width:100%; height:100%"></div>
  </div>
  <div style="background:#FFFFFF; width:60px; height:40px">
    <div style="background:#FFFFFF; width:100%; height:10%">
      <h5 align="center"><br/>down</h5>
    </div>
    <div id="pwmminus" style="background:#FF0000; width:100%; height:100%"></div>
  </div>



<br/>
<br/>



 <h2>SWITCH LED</h2>

    <h2>LED1</h2>
    <p><a href=/ledon1>LED OFF</a></p>
    <p><a href=/ledoff1>LED ON</a></p>

    <h2>LED2</h2>
    <p><a href=/ledon2>LED ON</a></p>
    <p><a href=/ledoff2>LED OFF</a></p>



<img alt="" src="data:image/jpeg;base64,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AAAAAAAAAAAAAD+AAAA4QklNBA0AAAAAAAQAAAAeOEJJTQQZAAAAAAAEAAAAHjhCSU0D8wAAAAAACQAAAAAAAAAAAQA4QklNJxAAAAAAAAoAAQAAAAAAAAACOEJJTQP1AAAAAABIAC9mZgABAGxmZgAGAAAAAAABAC9mZgABAKGZmgAGAAAAAAABADIAAAABAFoAAAAGAAAAAAABADUAAAABAC0AAAAGAAAAAAABOEJJTQP4AAAAAABwAAD/////////////////////////////A+gAAAAA/////////////////////////////wPoAAAAAP////////////////////////////8D6AAAAAD/////////////////////////////A+gAADhCSU0ECAAAAAAAEAAAAAEAAAJAAAACQAAAAAA4QklNBB4AAAAAAAQAAAAAOEJJTQQaAAAAAANbAAAABgAAAAAAAAAAAAAAMgAAADIAAAATAEkATQBHAF8AMgAwADIAMgAwADEAMgA5AF8AMQA4ADQANAAzADgAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAADIAAAAyAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAEAAAAAAABudWxsAAAAAgAAAAZib3VuZHNPYmpjAAAAAQAAAAAAAFJjdDEAAAAEAAAAAFRvcCBsb25nAAAAAAAAAABMZWZ0bG9uZwAAAAAAAAAAQnRvbWxvbmcAAAAyAAAAAFJnaHRsb25nAAAAMgAAAAZzbGljZXNWbExzAAAAAU9iamMAAAABAAAAAAAFc2xpY2UAAAASAAAAB3NsaWNlSURsb25nAAAAAAAAAAdncm91cElEbG9uZwAAAAAAAAAGb3JpZ2luZW51bQAAAAxFU2xpY2VPcmlnaW4AAAANYXV0b0dlbmVyYXRlZAAAAABUeXBlZW51bQAAAApFU2xpY2VUeXBlAAAAAEltZyAAAAAGYm91bmRzT2JqYwAAAAEAAAAAAABSY3QxAAAABAAAAABUb3AgbG9uZwAAAAAAAAAATGVmdGxvbmcAAAAAAAAAAEJ0b21sb25nAAAAMgAAAABSZ2h0bG9uZwAAADIAAAADdXJsVEVYVAAAAAEAAAAAAABudWxsVEVYVAAAAAEAAAAA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AAAPAEEAZABvAGIAZQAgAFAAaABvAHQAbwBzAGgAbwBwAAAAFABBAGQAbwBiAGUAIABQAGgAbwB0AG8AcwBoAG8AcAAgADIAMAAyADIAAAABADhCSU0EBgAAAAAAB//8AAAAAQEA/+ENqGh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8APD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgNy4xLWMwMDAgNzkuZWRhMmIzZiwgMjAyMS8xMS8xNC0xMjozMDo0MiAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wTU09Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9tbS8iIHhtbG5zOnN0RXZ0PSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvc1R5cGUvUmVzb3VyY2VFdmVudCMiIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iIHhtcE1NOkRvY3VtZW50SUQ9IkZCNDhENDFCMEZCNjcwRjFEMzQ4MTIwM0Q5MTUxOENEIiB4bXBNTTpJbnN0YW5jZUlEPSJ4bXAuaWlkOmI4ZmIyNWYyLTFkMjYtMmU0YS04Y2U1LTY5MjMzZWU5NjdhNSIgeG1wTU06T3JpZ2luYWxEb2N1bWVudElEPSJGQjQ4RDQxQjBGQjY3MEYxRDM0ODEyMDNEOTE1MThDRCIgZGM6Zm9ybWF0PSJpbWFnZS9qcGVnIiBwaG90b3Nob3A6Q29sb3JNb2RlPSIzIiBwaG90b3Nob3A6SUNDUHJvZmlsZT0ic1JHQiIgeG1wOkNyZWF0ZURhdGU9IjIwMjItMDEtMzBUMTI6NTM6NTgrMDU6MzAiIHhtcDpNb2RpZnlEYXRlPSIyMDIyLTAyLTEwVDAwOjI2OjU1KzA1OjMwIiB4bXA6TWV0YWRhdGFEYXRlPSIyMDIyLTAyLTEwVDAwOjI2OjU1KzA1OjMwIj4gPHhtcE1NOkhpc3Rvcnk+IDxyZGY6U2VxPiA8cmRmOmxpIHN0RXZ0OmFjdGlvbj0ic2F2ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6Y2M1NDdjNjAtNzc4OS04YTRlLWI2NmUtNDZmZmQ1Y2Y3MWM1IiBzdEV2dDp3aGVuPSIyMDIyLTAyLTA5VDIwOjI4OjA0KzA1OjMwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgMjMuMSAoV2luZG93cykiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPHJkZjpsaSBzdEV2dDphY3Rpb249InNhdmVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmI4ZmIyNWYyLTFkMjYtMmU0YS04Y2U1LTY5MjMzZWU5NjdhNSIgc3RFdnQ6d2hlbj0iMjAyMi0wMi0xMFQwMDoyNjo1NSswNTozMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDIzLjEgKFdpbmRvd3MpIiBzdEV2dDpjaGFuZ2VkPSIvIi8+IDwvcmRmOlNlcT4gPC94bXBNTTpIaXN0b3J5PiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA8P3hwYWNrZXQgZW5kPSJ3Ij8+/+ICKElDQ19QUk9GSUxFAAEBAAACGAAAAAACEAAAbW50clJHQiBYWVogAAAAAAAAAAAAAAAAYWNzcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAPbWAAEAAAAA0y0AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAJZGVzYwAAAPAAAAB0clhZWgAAAWQAAAAUZ1hZWgAAAXgAAAAUYlhZWgAAAYwAAAAUclRSQwAAAaAAAAAoZ1RSQwAAAaAAAAAoYlRSQwAAAaAAAAAod3RwdAAAAcgAAAAUY3BydAAAAdwAAAA8bWx1YwAAAAAAAAABAAAADGVuVVMAAABYAAAAHABzAFIARwBCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABYWVogAAAAAAAAb6IAADj1AAADkFhZWiAAAAAAAABimQAAt4UAABjaWFlaIAAAAAAAACSgAAAPhAAAts9wYXJhAAAAAAAEAAAAAmZmAADypwAADVkAABPQAAAKWwAAAAAAAAAAWFlaIAAAAAAAAPbWAAEAAAAA0y1tbHVjAAAAAAAAAAEAAAAMZW5VUwAAACAAAAAcAEcAbwBvAGcAbABlACAASQBuAGMALgAgADIAMAAxADb/7gAOQWRvYmUAZIAAAAAB/9sAhAAgISEzJDNRMDBRQi8vL0InHBwcHCciFxcXFxciEQwMDAwMDBEMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMASIzMzQmNCIYGCIUDg4OFBQODg4OFBEMDAwMDBERDAwMDAwMEQwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAz/wAARCAAyADIDASIAAhEBAxEB/90ABAAE/8QBGwAAAwEBAQEBAQEBAQAAAAAAAQACAwQFBgcICQoLAQEBAQEBAQEBAQEBAQAAAAAAAQIDBAUGBwgJCgsQAAICAQMCAwQHBgMDBgIBNQEAAhEDIRIxBEFRIhNhcTKBkbFCoQXRwRTwUiNyM2LhgvFDNJKishXSUyRzwmMGg5Pi8qNEVGQlNUUWJnQ2VWWzhMPTdePzRpSkhbSVxNTk9KW1xdXl9VZmdoaWprbG1ub2EQACAgAFAQYGAQMBAwUDBi8AARECIQMxQRJRYXGBkSITMvChsQTB0eHxQlIjYnIUkjOCQySisjRTRGNzwtKDk6NU4vIFFSUGFiY1ZEVVNnRls4TD03Xj80aUpIW0lcTU5PSltcXV5fVWZnaG/9oADAMBAAIRAxEAPwDloGG8+L04cwxRN8PnxkRGj8LOWyYgf6f6pIHRkzCchKVCvhx//XX/AMTdv2mEzoBGRH7/APmRzh02IGpyEj/WubHijHfirdD44xlu8n9v1P8AzGgEGWMWO/lcDi0sf8l9DPh9LGPe+fIkHRAH5KyqB//Q87f5dvg7Y6sGXbzuBAFF2FGJvTRA6PT6fFLfX+j4/wD0C6Rw4QCIgefyc/8Aef7t4vU9H4x/M8v8zb6nqYv/ABmV1leTz4xsh7vT9bqP+/8AR/f1MqB29adYx/1f++3yJ/E9u+WWXm5+F5cgESQgZUVZtUD/0eDaZ61oEWQbHY/9BMsmtfv5GoUQgdkSOo10sfFj/f8A3X/d5Gc+T/djttlP/wCt/wDzH/vXjlAE6/v/ALz/ANCY24R8ONUAiru2JwPxfEP4v/giJ6e//wAi6YZbrB4KBy2rWw/fsVA//9LyZ8/v4f8Aux2wcPnqgepPn/k/+/Vx/CHy1QO/qO3w9/7f/v1rA+cqB630fEr5KoH/2Q==" />
    


</body>







<script>
  window.addEventListener('load', function(){
    var n = document.getElementById('LEDn') 
    var minus = document.getElementById('pwmminus') 
    var plus = document.getElementById('pwmplus') 
    
    var xhr = new XMLHttpRequest();
    n.onmousedown = function(){
      xhr.open("GET", "LEDon", true);
      xhr.send();  
    }
    n.onmouseup = function(){
      xhr.open("GET", "LEDoff", true);
      xhr.send();  
    }
    n.addEventListener('touchstart', function(e){
      xhr.open("GET", "LEDon", true);
      xhr.send();
    }, false)
    n.addEventListener('touchend', function(e){
      xhr.open("GET", "LEDoff", true);
      xhr.send();
    }, false)
    plus.onmousedown = function(){
      xhr.open("GET", "plus", true);
      xhr.send();  
    }
    plus.onmouseup = function(){
      xhr.open("GET", "stop", true);
      xhr.send();  
    }
    plus.addEventListener('touchstart', function(e){
      xhr.open("GET", "plus", true);
      xhr.send();
    }, false)
    plus.addEventListener('touchend', function(e){
      xhr.open("GET", "stop", true);
      xhr.send();
    }, false)
    minus.onmousedown = function(){
      xhr.open("GET", "minus", true);
      xhr.send();  
    }
    minus.onmouseup = function(){
      xhr.open("GET", "stop", true);
      xhr.send();  
    }
    minus.addEventListener('touchstart', function(e){
      xhr.open("GET", "minus", true);
      xhr.send();
    }, false)
    minus.addEventListener('touchend', function(e){
      xhr.open("GET", "stop", true);
      xhr.send();
    }, false)
  },false)
</script>
</html>
)***";



void handleRoot() {
  server.send(200, "text/html", webpage);
  
}



int pwm;
int pwmCount;

void ledON(){
  led=1;
}

void ledOFF(){
  led=0;  
}

void plus(){
  pwm=3;  
}

void minus(){
  pwm=-3;  
}

void stopPWM(){
  pwm=0;  
}








void ledon1()
{
  digitalWrite(2, 1);
  server.send(200, "text/html", webpage);
}
void ledoff1()
{
  digitalWrite(2, 0);
  server.send(200, "text/html", webpage);
}


void ledon2()
{
  digitalWrite(5, 1);
  server.send(200, "text/html", webpage);
}
void ledoff2()
{
  digitalWrite(5, 0);
  server.send(200, "text/html", webpage);
}








void setup() {
 pinMode(2, OUTPUT); 
 pinMode(5, OUTPUT); 
 pinMode(led, OUTPUT);
 delay(1000);
  Serial.begin(9600);

  pinMode(LEDn,OUTPUT);
  pinMode(LEDpwm,OUTPUT);

  WiFi.mode(WIFI_AP);
  IPAddress apLocalIp(192,168,4,1);
  IPAddress apSubnetMask(255,255,255,0);
  WiFi.softAPConfig(apLocalIp,apLocalIp,apSubnetMask);
  WiFi.softAP(ssid, password);



  server.on("/ledon1", ledon1);
  server.on("/ledoff1", ledoff1);


  server.on("/ledon2", ledon2);
  server.on("/ledoff2", ledoff2);

  
  server.begin();

  server.on("/", handleRoot);
  server.on("/LEDon", ledON);     
  server.on("/LEDoff", ledOFF);   
  server.on("/minus", minus);   
  server.on("/plus", plus);   
  server.on("/stop", stopPWM);   

  pwm=0;
  pwmCount=0;
  led=0;
}

void loop() {
  server.handleClient();

  digitalWrite(LEDn, led);
  
  pwmCount+=pwm;

  if(pwmCount>1024)pwmCount=1024;
  if(pwmCount<0)pwmCount=0;

  analogWrite(LEDpwm, pwmCount);
  delay(10);
}
