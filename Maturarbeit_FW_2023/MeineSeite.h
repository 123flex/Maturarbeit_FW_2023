extern NavigationController navigationController;

class MeineSeite : public Page{
    protected: 
        void Setup() override{
            Page::Setup();

            SetNumButtons(1);

            buttons[0] = Button(20, 55, 280, 100, true);
            buttons[0].BorderColor = ILI9341_DARKGREEN;
            buttons[0].Color = ILI9341_RED;
            buttons[0].SetFontSize(3);
            buttons[0].SetTextPos(25, 15);
            buttons[0].SetTextColor(ILI9341_WHITE);
            buttons[0].SetText(F("Click to turn \n\n      on LED"));

            SetNumLabels(2);
                labels[0] = Label(26, 170, 2);
                labels[0].TextColor = ILI9341_RED;
                labels[0].SetText(F(""));
                labels[1] = Label(150, 195, 2);
                labels[1].TextColor = ILI9341_LIGHTGREY;
                labels[1].SetText(F(""));              
        }


        void ExecuteButton(int index) override{
            Serial.println("LED anschalten"); 
            digitalWrite(13, HIGH);
//            labels[0].SetText(F("Pay attention: \n   LED/Laser is running! \n   Wait for stabilisation."));
            labels[0].SetText("Wait for stabilisation.");
            labels[1].SetText("0%");  
                for(int j=1; j<11; j++){
                delay(1000);
                labels[1].SetText((String(j*10)+"%"));
                }     
            navigationController.NavigateTo(1);
        }



 
};