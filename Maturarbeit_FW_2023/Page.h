extern Adafruit_ILI9341 tft;
// WEF Benutzung der Touchscreen (ts) Utility auf 
// WEF Basis der "TouchScreen" Library, welche in der "Adafruit ILI9341" 
// WEF Library enthalten ist, und nicht auf Basis der "URTouch Library"  
// WEF (vgl. auch Anpassungen und Kommentare in "waterhardness.ino"
// WEF (auskommentiert): extern URTouch ts;
// WEF daher die nachfolgende neuen Zeilen (kopiert aus example "breachtouchpaint"):
extern TouchScreen ts;
// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 20
#define TS_MAXX 920
#define TS_MAXY 920
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define PENRADIUS 3
// WEF Ende der Anpassungen

class Page {
    friend class NavigationController;
    protected:
        bool isVisible = false;

        int numButtons;
        Button* buttons;

        int numLabels;
        Label* labels;

        virtual void Setup(){

        }


// WEF "dataAvailable" existiert nur in "URTouch" Library, aber nicht
// WEF in "Adafruit ILI9341" Library deshalb wird die folgende Funktion zum Prüfen, 
// WEF ob die "Anweisungsbox" berührt wurde, auskommentiert und in Anlehnung an das 
// WEF Example "breakouttouchpaint" eine neue Funktion definiert:
/*
        virtual void Loop(){
          if (ts.dataAvailable())
            {
                int x, y;
                ts.read();
                x = ts.getX();
                y = ts.getY();
                for(int i=0; i<numButtons; i++){
                    if(buttons[i].CheckIfClicked(x, y))
                        ExecuteButton(i);
                }
            }
        } 
*/

// WEF Neue Funktion zum Prüfen, ob eine "Befehlsbox" berührt wurde in Anlehnung an 
// WEF Example "breakouttouchpaint":
//
        virtual void Loop(){
          
          // Retrieve a point  
          TSPoint p = ts.getPoint();
            
          // Prüfen, ob der Touchscreen berührt wurde
          // we have some minimum pressure we consider 'valid'
          // pressure of 0 means no pressing!
          if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
            return;
          }
// WEF           
//          Serial.println("==================================================================");
//          Serial.print(F("Touchscreen wurde berührt: "));
//          Serial.print("\tPressure = "); Serial.println(p.z);
// WEF 
          // WEF Falls berührt, werden die Koordinaten der Berührung ausgelesen 
          // WEF und in das Koordinatensystem des tft Bildschirms umgewandelt 
          // 
          // Scale from ~0->1000 to tft.width using the calibration #'s
          // p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
          // p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
           //
          // WEF Da der TFT Screen in waterhardness.ino um 270 Grad gedreht 
          // WEF wurde, muss px und py vertauscht und das neue px invertiert 
          // werden.
          int x, y;
          bool test;
          y = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
          x = tft.width() - map(p.y, TS_MINY, TS_MAXY, 0, tft.width());

// WEF           
//          Serial.print("X' = "); Serial.print(x);
//          Serial.print("\tY' = "); Serial.println(y);
//          Serial.print("tft Breite (Xmax)= "); Serial.print(tft.width());
//          Serial.print("\ttft_Höhe (Ymax) = "); Serial.println(tft.height());
//          Serial.println("");
//          // WEF Prüfen, ob einer der "Funktionssbuttons auf dem TFT
//          // berührt wurden
//          Serial.println(F("Prüfen ob einer der Funktionsbuttons auf dem TFT gedrückt wurde:"));
//          Serial.println("");
// WEF 
          for(int i=0; i<numButtons; i++){
              //Serial.print(F("Prüfen ob Button Nr. "));
              //Serial.print(i);
              //Serial.print(F(" berührt wurde (0=Nein/1=Ja)? "));
              //Serial.println(buttons[i].CheckIfClicked(x, y));
              if(buttons[i].CheckIfClicked(x, y)){
// WEF
//                  Serial.print(F("===> Button Nr. "));
//                  Serial.print(i);
//                  Serial.println(F(" wurde berührt! "));
//                  Serial.println("");
//                  //Serial.print("x = "); Serial.print(x);
//                  //Serial.println("y = "); Serial.println(y);
// WEF                  
                  ExecuteButton(i);
                  return;
              }
// WEF              
//              Serial.print(F("===> Button Nr. "));
//              Serial.print(i);
//              Serial.println(F(" wurde nicht berührt!"));
//              Serial.println("");
// WEF               
              }
          }

        virtual void ExecuteButton(int index){}

    public:     
        int BackgroundColor = 0xFFFF;
        
        void SetNumButtons(int numButtons){
            this->numButtons = numButtons;
            buttons = new Button[numButtons];
        }

        void SetNumLabels(int numLabels){
            this->numLabels = numLabels;
            labels = new Label[numLabels]; 
        }

        void AddButton(Button b, int index){
            buttons[index] = b;
        }

        void AddLabel(Label l, int index){
            labels[index] = l;
        }

        void Show(){
            isVisible = true;
            tft.fillScreen(BackgroundColor);

            Setup();

            for(int i=0; i<numLabels; i++){
                labels[i].Show();
            }
        
            for(int i=0; i<numButtons; i++){
                buttons[i].Show();
            }
        }

        void Hide(){
            isVisible = false;
        }
};