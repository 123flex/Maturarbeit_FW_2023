extern NavigationController navigationController;

class MainPage : public Page{
    protected: 
        void Setup() override{
            Page::Setup();

            SetNumButtons(3);

            buttons[0] = Button(25, 15, 270, 60, true);
            buttons[0].BorderColor = ILI9341_DARKGREEN;
            buttons[0].Color = ILI9341_GREEN;
            buttons[0].SetFontSize(3);
            buttons[0].SetTextPos(23, 20);
            buttons[0].SetTextColor(ILI9341_DARKGREEN);
            buttons[0].SetText(F("Calibration"));

    
            buttons[1] = Button(25, 90, 270, 60, true);
            buttons[1].BorderColor = ILI9341_DARKCYAN;
            buttons[1].Color = ILI9341_CYAN;
            buttons[1].SetFontSize(3);
            buttons[1].SetTextPos(23, 20);
            buttons[1].SetTextColor(ILI9341_DARKCYAN);
            buttons[1].SetText(F("Measurement"));

            
            buttons[2] = Button(25, 165, 270, 60, true);
            buttons[2].BorderColor = ILI9341_DARKCYAN;
            buttons[2].Color = ILI9341_RED;
            buttons[2].SetFontSize(3);
            buttons[2].SetTextPos(23, 20);
            buttons[2].SetTextColor(ILI9341_DARKCYAN);
            buttons[2].SetText(F("LED Off"));
        }

        void ExecuteButton(int index) override{
            switch(index){
                case 0:
                    CalibrationBtn_OnClick();
                    break;
                case 1:
                    MeasurementBtn_OnClick();
                    break;
                case 2:
                    TurnOffBtn_OnClick();
                    break;
            }
        }

        void CalibrationBtn_OnClick()
        {
            navigationController.NavigateTo(1);
        }

        void MeasurementBtn_OnClick()
        {
            navigationController.NavigateTo(4);
        }

        void TurnOffBtn_OnClick()
        {
            Serial.println("LED ausschalten"); 
            digitalWrite(13, LOW);
            navigationController.NavigateTo(5);
        }
};